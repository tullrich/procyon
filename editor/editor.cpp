/*
===========================================================================

Procyon, a 2D game.

Copyright (C) 2015 Tim Ullrich.

This file is part of Procyon.

Procyon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Procyon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Procyon.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "Camera.h"

#include "editor.h"
#include "ui_editor.h"
#include "ProcyonQtUtil.h"
#include "ProcyonCanvas.h"
#include "LogOutputTarget.h"
#include "MapDocument.h"
#include "EditorSettings.h"
#include "RecentFileList.h"

#include <thread>
#include <chrono>

// in milliseconds
#define PROCYON_MSG_TIMEOUT 4000

#define PROCYON_RECENT_FILE_MAX 5

using namespace Procyon;

Editor::Editor( QWidget *parent )
    : QMainWindow( parent )
    , mSettings( new EditorSettings( this ) )
    , mUi( new Ui::Editor )
    , mCanvas( NULL )
    , mCameraLabel( NULL )
    , mTabBar( NULL )
    , mActiveDocument( NULL )
    , mRecentFiles( NULL )
{
    mUi->setupUi( this );

    // Init tab bar
    mTabBar = new QTabBar( mUi->centralwidget );
    mTabBar->setMovable( true );
    mTabBar->setChangeCurrentOnDrag( true );
    mTabBar->setTabsClosable( true );
    mTabBar->setExpanding( false );
    mUi->centralwidget->layout()->addWidget( mTabBar );
    connect( mTabBar, SIGNAL( currentChanged( int ) ), this, SLOT( SetActiveDocument( int ) ) );
    connect( mTabBar, SIGNAL( tabMoved( int, int ) ), this, SLOT( TabMoved( int, int ) ) );
    connect( mTabBar, &QTabBar::tabCloseRequested, [this]( int tab ) { CloseDocument( mDocuments[ tab ] ); } );
    connect( mTabBar, &QTabBar::tabBarDoubleClicked, [this]( int tab ) { if ( tab == -1 ) NewDocument(); } );

    // Init central canvas
    mCanvas = new ProcyonCanvas( mUi->centralwidget );
    mUi->centralwidget->layout()->addWidget( mCanvas );
    connect( this, SIGNAL( ActiveDocumentChanged(MapDocument*) ), mCanvas, SLOT( OnActiveDocumentChanged(MapDocument*) ) );
    connect( mCanvas, SIGNAL( CameraChanged( const Procyon::Camera2D* ) )
           , this, SLOT( CanvasCameraChanged( const Procyon::Camera2D* ) ) );

    // Init simple menu actions
    connect( mUi->actionNew, SIGNAL( triggered() ), this, SLOT( NewDocument() ) );
    connect( mUi->actionOpen, SIGNAL( triggered() ), this, SLOT( OpenDocument() ) );
    connect( mUi->actionSave, SIGNAL( triggered() ), this, SLOT( SaveDocument() ) );
    connect( mUi->actionSave_As, SIGNAL( triggered() ), this, SLOT( SaveDocumentAs() ) );
    connect( mUi->actionClose, SIGNAL( triggered() ), this, SLOT( CloseDocument() ) );
    connect( mUi->actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
    connect( mUi->actionReset, SIGNAL( triggered() ), this, SLOT( ResetViewport() ) );

    // Setup the grid show/hide action
    mUi->actionDisableGrid->setCheckable( true );
    mUi->actionDisableGrid->setChecked( EditorSettings::Get().GetGridVisible() );
    connect( mUi->actionDisableGrid, &QAction::toggled, []( bool checked ) {
        EditorSettings::Get().SetGridVisible( checked );
    } );

    // Setup grid size actions (radio button functionality)
    QActionGroup* group = new QActionGroup( this );
    group->setExclusive( true );
    mUi->action1x1->setCheckable( true );
    mUi->action1x1->setChecked( (int)EditorSettings::Get().GetGridSize() == 1 );
    mUi->action1x1->setActionGroup( group );
    mUi->action5x5->setCheckable( true );
    mUi->action5x5->setChecked( (int)EditorSettings::Get().GetGridSize() == 5 );
    mUi->action5x5->setActionGroup( group );
    connect( group, SIGNAL( triggered( QAction* ) ), this, SLOT( GridSizeChanged( QAction* ) ) );

    // Setup window and toolbar menu actions
    mUi->menuWindows->addAction( mUi->dockTiles->toggleViewAction() );
    mUi->menuWindows->addAction( mUi->dockScene->toggleViewAction() );
    mUi->menuWindows->addAction( mUi->dockLog->toggleViewAction() );
    mUi->menuToolbars->addAction( mUi->fileToolbar->toggleViewAction() );
    mUi->menuToolbars->addAction( mUi->viewToolbar->toggleViewAction() );

    // Setup initial docks
    tabifyDockWidget( mUi->dockScene, mUi->dockTiles );

    // Setup Status bar
    mCameraLabel = new QLabel( this );
    statusBar()->addPermanentWidget( mCameraLabel );

    // Setup asset list widget
    new QListWidgetItem(QIcon("tile.png"), tr("Tile"), mUi->listWidget);

    // Setup output log
    SetupOutputLog();

    // Setup recent file menu
    mRecentFiles = new RecentFileList( this, mUi->menuRecent_Maps, PROCYON_RECENT_FILE_MAX );
    connect( mRecentFiles, SIGNAL( RecentFileSelected( QString ) ), this, SLOT( OpenDocument( QString ) ) );

    // Load settings init
    mSettings->Load();

    // Create initial document
    NewDocument();
}

Editor::~Editor()
{
    delete mUi;
}

void Editor::SetupOutputLog()
{
    LogOutputView* outputLog = new LogOutputView( mUi->dockWidgetContents_3 );
    outputLog->setObjectName(QStringLiteral("outputLogger"));
    outputLog->setReadOnly(true);
    outputLog->setMaximumBlockCount(200);

    QHBoxLayout *horizontalLayout = new QHBoxLayout( mUi->dockWidgetContents_3 );
    horizontalLayout->addWidget( outputLog );
}

void Editor::GridSizeChanged( QAction* action )
{
	if ( action == mUi->action1x1 )
        EditorSettings::Get().SetGridSize( 1.0f );
	else if ( action == mUi->action5x5 )
        EditorSettings::Get().SetGridSize( 5.0f );
}

void Editor::ResetViewport()
{
	mCanvas->ResetCamera();
}

void Editor::SetActiveDocument( int idx )
{
    if ( idx == -1 ) // no active tab, unsupported
    {
        return;
    }

    MapDocument *doc = mDocuments[ idx ];

    if ( mActiveDocument == doc )
        return;

    mActiveDocument =  doc;
    mTabBar->setCurrentIndex( idx );
    emit ActiveDocumentChanged( doc );
}

void Editor::TabMoved( int from, int to )
{
    MapDocument *tmp = mDocuments[ from ];
    mDocuments[ from ] = mDocuments[ to ];
    mDocuments[ to ]  = tmp;
}

void Editor::NewDocument()
{
    AddDocument( new MapDocument() );
    ShowStatusMessage( "New document created" );
}

void Editor::OpenDocument( QString filePath /* = QString() */ )
{
    // Prompt with FileDialog if no path is already provided
    if ( filePath.isEmpty() )
    {
        filePath = QFileDialog::getOpenFileName(this,
            tr("Open Map"), "", tr("Xml Files (*.xml)"));
    }

    // If we already have this document open
    int tabIdx = -1;
    if ( ( tabIdx = GetDocumentIndex( filePath ) ) != -1 )
    {
        // Just switch tabs
        SetActiveDocument( tabIdx );
    }
    else
    {
        // Create and load
        MapDocument *doc = new MapDocument();
        if ( !doc->Load( filePath ) )
        {
            ShowStatusMessage( "Unable to open " + filePath );
            delete doc;
            return;
        }

        // Add the document to the tab list
        AddDocument( doc );

        // Notify
        ShowStatusMessage( "Opened " + filePath );
    }


    mRecentFiles->AddRecentFile( filePath );
}

void Editor::AddDocument( MapDocument *doc, bool makeActive /* = true */ )
{

    mDocuments.push_back( doc );

    int tabIdx = mTabBar->addTab( doc->GetTabString() );
    connect(doc, &MapDocument::FilePathChanged, [doc, this](const QFileInfo &newFilePath) {
        int idx = GetDocumentIndex( doc );
        if ( idx != -1 )
        {
            mTabBar->setTabText( idx, newFilePath.fileName() );
            mTabBar->setTabToolTip( idx, newFilePath.filePath() );
        }
        else
        {
            PROCYON_WARN( "Editor", "MapDocument::FilePathChanged but unable to find index for map document." );            
        }
    } );

    connect(doc, &MapDocument::ModificationStateChanged, [doc, this]( bool modified ) {
        int idx = GetDocumentIndex( doc );
        if ( idx != -1 )
        {
            mTabBar->setTabText( idx, doc->GetTabString() );
        }
        else
        {
            PROCYON_WARN( "Editor", "MapDocument::ModificationStateChanged but unable to find index for map document." );   
        }
    } );

    if ( makeActive )
    {
        SetActiveDocument( tabIdx );

        bool replace = mDocuments.size() == 2 && mDocuments[ 0 ]->IsFresh();
        if ( replace )
        {
            CloseDocument( mDocuments[ 0 ] );
        }
    }
}

bool Editor::SaveDocument( MapDocument *doc /* = NULL */ )
{
    if ( !doc )
    {
        doc = mActiveDocument;
    }

    if ( doc->HasSavePath() )
    {
        if ( doc->Save( doc->GetFilePath() ) )
        {
            ShowStatusMessage( "Saved " + doc->GetFilePath() );
            return true;
        }
    }

    return SaveDocumentAs();
}

bool Editor::SaveDocumentAs( MapDocument *doc /* = NULL */ )
{
    if ( !doc )
    {
        doc = mActiveDocument;
    }

    // workaround: the gl repainting causes extreme slowdown in the file dialog.
    mCanvas->PauseRendering();

    //    open save dialog
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("Save Map"), doc->GetFilePath(), tr("Map Files (*.xml)"));

    bool saved = false;
    if ( !filePath.isEmpty() )
    {
        doc->SetFilePath( filePath );
        if ( ( saved = doc->Save( doc->GetFilePath() ) ) )
        {
            mRecentFiles->AddRecentFile( doc->GetFilePath() );
            ShowStatusMessage( "Saved " + doc->GetFilePath() );
        }
    }

    mCanvas->StartRendering();
    return saved;
}

bool Editor::CloseDocument( MapDocument *doc /* = NULL */, bool forceDiscard /* = false */ )
{
    if ( !doc )
    {
        doc = mActiveDocument;
    }

    int idx = GetDocumentIndex( doc );

    if ( !forceDiscard && doc->IsModified() )
    {
        SetActiveDocument( idx );

        QString docName = (doc->HasSavePath()) ? doc->FileName() : "untitled";

        QMessageBox msgBox( this );
        msgBox.setText("Document '" + docName + "' has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
            // Save clicked
            if ( !SaveDocument() )
            {
                return false; // save cancel, abort close
            }
            break;
        case QMessageBox::Cancel:
            return false ;
        case QMessageBox::Discard:
            break; // continue;
        }
    }

    mTabBar->removeTab( idx );
    mDocuments.removeAt( idx );
    ShowStatusMessage( "Closed " + ( ( doc->HasSavePath() ) ? doc->GetFilePath() : "untitled" ) );

    if ( mDocuments.size() == 0 )
    {
        NewDocument();
    }
    else if ( doc == mActiveDocument )
    {
        SetActiveDocument( mTabBar->currentIndex() );
    }

    delete doc;
    return true;
}

void Editor::CanvasCameraChanged( const Camera2D* cam )
{
	glm::vec2 camPos = cam->GetPosition();

	QString camText = QString("<%1, %2>")
		.arg( (double)camPos.x, 0, 'g', 3, '0' )
		.arg( (double)camPos.y, 0, 'g', 3, '0' );
	mCameraLabel->setText( camText );
    mCameraLabel->setFrameStyle( QFrame::NoFrame );
}

int Editor::GetDocumentIndex( MapDocument* doc ) const
{
    return mDocuments.indexOf( doc );
}

int Editor::GetDocumentIndex( const QString& filePath ) const
{
    for ( int i = 0; i < mDocuments.size(); ++i )
    {
        MapDocument *doc = mDocuments[ i ];
        if ( doc->HasSavePath() && doc->GetFilePath() == filePath )
            return i;
    }
    return -1;
}

void Editor::closeEvent( QCloseEvent *event )
{
    QList<MapDocument*> copy( mDocuments );
    QListIterator< MapDocument* > iter( copy );
    iter.toBack();
    while ( iter.hasPrevious() )
    {
        if ( !CloseDocument( iter.previous() ) )
        {
            event->ignore();
            return;
        }
    }

    mSettings->Save();
}

void Editor::ShowStatusMessage( const QString& msg, bool permanent /* = false */ )
{
    statusBar()->showMessage( msg, (permanent) ? 0 : PROCYON_MSG_TIMEOUT);
    PROCYON_INFO( "Editor", "%s", ProcyonQtUtil::QStringToUtf8( msg ).c_str() );
}