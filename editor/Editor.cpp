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
#include <QtVariantPropertyManager>
#include <QtTreePropertyBrowser>
#include <QMetaProperty>
#include <QOpenGLContext>
#include <QGraphicsView>
#include <QAbstractScrollArea>
#include <QScrollBar>

#include "Camera.h"

#include "Editor.h"
#include "ui_editor.h"
#include "ProcyonQtUtil.h"
#include "ProcyonCanvas.h"
#include "LogOutputTarget.h"
#include "MapDocument.h"
#include "EditorSettings.h"
#include "RecentFileList.h"
#include "PrefsDialog.h"

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

	QIcon icon;
	icon.addFile( QStringLiteral(":/resources/appico.ico") );
    setWindowIcon( icon );

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
    QAbstractScrollArea* scrollArea = new QAbstractScrollArea( mUi->centralwidget );
    QVBoxLayout* scrollAreaLayout = new QVBoxLayout( scrollArea->viewport() );
    scrollAreaLayout->setContentsMargins( 0, 0, 0, 0 );

    // set black background
    QPalette pal = palette();
    pal.setColor( QPalette::Background, Qt::red );
    scrollArea->viewport()->setAutoFillBackground( true );
    scrollArea->viewport()->setPalette( pal );

    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarPolicy::ScrollBarAlwaysOn );
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarPolicy::ScrollBarAlwaysOn );
    mUi->centralwidget->layout()->addWidget( scrollArea );
    scrollArea->verticalScrollBar()->setPageStep( 10 );
    scrollArea->horizontalScrollBar()->setPageStep( 10 );
    scrollArea->verticalScrollBar()->setRange( 0, 100 );
    scrollArea->horizontalScrollBar()->setRange( 0, 100 );

    mCanvas = new ProcyonCanvas( scrollArea->viewport() );
    connect( scrollArea->horizontalScrollBar(), &QScrollBar::valueChanged, [ this ]( int value ) {
        PROCYON_DEBUG("ProcyonCanvas", "scrolllled");
        //mCanvas->move( value, 0 );
        mActiveDocument->GetRootSceneObject()->setProperty( "Position", QPoint( value, 0 ) );
    } );
    scrollAreaLayout->addWidget( mCanvas );

    connect( this, SIGNAL( ActiveDocumentChanged(MapDocument*) ), mCanvas, SLOT( OnActiveDocumentChanged(MapDocument*) ) );
    connect( mCanvas, SIGNAL( CameraChanged( const Procyon::Camera2D* ) )
           , this, SLOT( CanvasCameraChanged( const Procyon::Camera2D* ) ) );

    // QGraphicsView
    /*QGraphicsScene* scene = new QGraphicsScene( 0, 0, 1000.0f, 1000.0f );
    QGraphicsView* view = new QGraphicsView( scene, mUi->centralwidget );
    view->setViewport( new QGLWidget() );
    scene->addText( "Hello, world!" );
    mUi->centralwidget->layout()->addWidget( view );*/

    // Init simple menu actions
    connect( mUi->actionNew, SIGNAL( triggered() ), this, SLOT( NewDocument() ) );
    connect( mUi->actionOpen, SIGNAL( triggered() ), this, SLOT( OpenDocument() ) );
    connect( mUi->actionSave, SIGNAL( triggered() ), this, SLOT( SaveDocument() ) );
    connect( mUi->actionSave_As, SIGNAL( triggered() ), this, SLOT( SaveDocumentAs() ) );
    connect( mUi->actionClose, SIGNAL( triggered() ), this, SLOT( CloseDocument() ) );
    connect( mUi->actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
    connect( mUi->actionReset, SIGNAL( triggered() ), this, SLOT( ResetViewport() ) );
    connect( mUi->actionPreferences, SIGNAL( triggered() ), this, SLOT( OpenPreferences() ) );

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
    mUi->menuWindows->addAction( mUi->dockObjects->toggleViewAction() );
    mUi->menuToolbars->addAction( mUi->fileToolbar->toggleViewAction() );
    mUi->menuToolbars->addAction( mUi->viewToolbar->toggleViewAction() );

    // Setup initial docks
    tabifyDockWidget( mUi->dockScene, mUi->dockTiles );

	// Get the Tile sets list widget
	mTileSetsView = mUi->listWidget;

    // Setup Status bar
    mCameraLabel = new QLabel( this );
    statusBar()->addPermanentWidget( mCameraLabel );

	// Setup object list
	mSceneTreeItemMenu = new QMenu( this );
	mSceneCreate = new QAction( "Add Object", mSceneTreeItemMenu );
	mSceneDelete = new QAction( "Delete", mSceneTreeItemMenu );
	mSceneTreeItemMenu->addAction( mSceneDelete );
	mSceneTreeItemMenu->addAction( mSceneCreate );
	mUi->sceneTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect( mUi->sceneTree, SIGNAL( customContextMenuRequested( const QPoint& ) )
		, this, SLOT( OnSceneTreeContextMenuRequested( const QPoint & ) ), Qt::QueuedConnection );
	connect( mUi->sceneTree, SIGNAL( activated( const QModelIndex& ) )
		, this, SLOT( OnObjectActivated( const QModelIndex& ) ) );


	// Setup property editor
    mPropertyManager = new QtVariantPropertyManager( this );
    QtVariantEditorFactory *factory = new QtVariantEditorFactory( this );
    mUi->propertyList->setFactoryForManager( mPropertyManager, factory);
    connect(mPropertyManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(OnPropertyValueChanged(QtProperty *, const QVariant &)));

    // Setup recent file menu
    mRecentFiles = new RecentFileList( this, mUi->menuRecent_Maps, PROCYON_RECENT_FILE_MAX );
    connect( mRecentFiles, SIGNAL( RecentFileSelected( QString ) ), this, SLOT( OpenDocument( QString ) ) );

	// Load settings init
	mSettings->Load();

    // Setup output log
    SetupOutputLog();

    // Create initial document
    NewDocument();
}

Editor::~Editor()
{
    delete mUi;
}

void Editor::SetupOutputLog()
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout( mUi->dockWidgetContents_3 );
    horizontalLayout->addWidget( new LogOutputView( mUi->dockWidgetContents_3 ) );
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

	QItemSelectionModel *m = mUi->sceneTree->selectionModel();
	mUi->sceneTree->setModel( new SceneObjectListModel( this, mActiveDocument->GetRootSceneObject() ) );
	delete m;

	mTileSetsView->clear();
	for ( int i = 0; i < doc->GetTileDefCount(); i++ )
	{
		mTileSetsView->addItem( new QListWidgetItem(
			QIcon( QString( doc->GetTileDef( i ).filepath.c_str() ) ), QString( doc->GetTileDef( i ).filepath.c_str() ) ) );
	}

    emit ActiveDocumentChanged( doc );
}

void Editor::TabMoved( int from, int to )
{
    MapDocument *tmp = mDocuments[ from ];
    mDocuments[ from ] = mDocuments[ to ];
    mDocuments[ to ]  = tmp;
}


void Editor::OpenPreferences()
{
	PrefsDialog* prefsDialog = new PrefsDialog( this );
	prefsDialog->exec();
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
		mCanvas->PauseRendering();
        filePath = QFileDialog::getOpenFileName(this,
            tr("Open Map"), "", tr("Xml Files (*.xml)"));
		mCanvas->StartRendering();
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
    mCanvas->StartRendering();

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

void Editor::OnSceneTreeContextMenuRequested( const QPoint& point )
{
	QModelIndex index = mUi->sceneTree->indexAt( point );
	if ( index.isValid() )
	{
		mSceneDelete->setEnabled( true );
	}
	else
	{
		mSceneDelete->setEnabled( false );
	}
	QAction* result = mSceneTreeItemMenu->exec( mUi->sceneTree->mapToGlobal( point ) );

	if ( result == mSceneDelete )
	{
		if ( index.isValid() )
		{
			SceneObject* object = static_cast< SceneObject* >( index.internalPointer() );
			if ( object->GetParent() )
			{

				PROCYON_DEBUG( "Editor", "Deleting row [%i, %i - %i ]"
					, index.row(), index.column(), index.internalPointer() );
				object->GetParent()->RemoveChild( object );
				PROCYON_DEBUG( "Editor", "Post 1" );
				//delete object;
				//object->deleteLater();
			}
		}
	}
	else if ( result == mSceneCreate )
	{
		SceneObject* parent;
		if ( index.isValid() )
		{
			parent = static_cast< SceneObject* >( index.internalPointer() );
		}
		else
		{
			parent = mActiveDocument->GetRootSceneObject();
		}
		parent->AddChild( new SceneObject( "New Object", parent ) );
	}
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

void Editor::OnObjectActivated( const QModelIndex &index )
{
	if ( index.isValid() )
	{
		PROCYON_DEBUG( "Editor", "OnObjectActivated");
		SceneObject* object = static_cast< SceneObject* >( index.internalPointer() );
		SetPropertySheetObject( object );

	}
}

void Editor::AddClassProperties(const QMetaObject *metaObject)
{
    if ( !metaObject || metaObject == &QObject::staticMetaObject )
        return;

    AddClassProperties( metaObject->superClass() );

	QtProperty *classProperty = mClassToProperty.value( metaObject );
    if (!classProperty) {
        QString className = QLatin1String( metaObject->className() );
        classProperty = mPropertyManager->addProperty( QtVariantPropertyManager::groupTypeId(), className );
        mClassToProperty[ metaObject ] = classProperty;
        //m_propertyToClass[classProperty] = metaObject;

        for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++) {
            QMetaProperty metaProperty = metaObject->property(idx);
            int type = metaProperty.userType();
            QtVariantProperty *subProperty = 0;
            if (!metaProperty.isReadable()) {
                //subProperty = m_readOnlyManager->addProperty(QVariant::String, QLatin1String(metaProperty.name()));
                //subProperty->setValue(QLatin1String("< Non Readable >"));
            } else if (metaProperty.isEnumType()) {
                /*if (metaProperty.isFlagType()) {
                    subProperty = m_manager->addProperty(QtVariantPropertyManager::flagTypeId(), QLatin1String(metaProperty.name()));
                    QMetaEnum metaEnum = metaProperty.enumerator();
                    QMap<int, bool> valueMap;
                    QStringList flagNames;
                    for (int i = 0; i < metaEnum.keyCount(); i++) {
                        int value = metaEnum.value(i);
                        if (!valueMap.contains(value) && isPowerOf2(value)) {
                            valueMap[value] = true;
                            flagNames.append(QLatin1String(metaEnum.key(i)));
                        }
                    subProperty->setAttribute(QLatin1String("flagNames"), flagNames);
                    subProperty->setValue(flagToInt(metaEnum, metaProperty.read(m_object).toInt()));
                    }
                } else {
                    subProperty = m_manager->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String(metaProperty.name()));
                    QMetaEnum metaEnum = metaProperty.enumerator();
                    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
                    QStringList enumNames;
                    for (int i = 0; i < metaEnum.keyCount(); i++) {
                        int value = metaEnum.value(i);
                        if (!valueMap.contains(value)) {
                            valueMap[value] = true;
                            enumNames.append(QLatin1String(metaEnum.key(i)));
                        }
                    }
                    subProperty->setAttribute(QLatin1String("enumNames"), enumNames);
                    subProperty->setValue(enumToInt(metaEnum, metaProperty.read(m_object).toInt()));
                }*/
            } else if (mPropertyManager->isPropertyTypeSupported(type)) {
                if (!metaProperty.isWritable())
				{
                    //subProperty = m_readOnlyManager->addProperty(type, QLatin1String(metaProperty.name()) + QLatin1String(" (Non Writable)"));
                }
				if (!metaProperty.isDesignable())
				{
                    //subProperty = m_readOnlyManager->addProperty(type, QLatin1String(metaProperty.name()) + QLatin1String(" (Non Designable)"));
				}
				else
				{
                    subProperty = mPropertyManager->addProperty(type, QLatin1String(metaProperty.name()));
				}
                subProperty->setValue(metaProperty.read( mPropertyObject ));
            } else {
                subProperty = mPropertyManager->addProperty(QVariant::String, QLatin1String(metaProperty.name()));
                subProperty->setValue(QLatin1String("< Unknown Type >"));
                subProperty->setEnabled(false);
            }
            classProperty->addSubProperty(subProperty);
            //m_propertyToIndex[subProperty] = idx;
            //m_classToIndexToProperty[metaObject][idx] = subProperty;
        }
    } else {
        //updateClassProperties(metaObject, false);
    }

    mTopLevelProperties.append(classProperty);
    mUi->propertyList->addProperty(classProperty);
}

void Editor::OnPropertyValueChanged(QtProperty* property, const QVariant& value )
{
	PROCYON_DEBUG( "Editor", "OnPropertyValueChanged" );

    const QMetaObject *metaObject = mPropertyObject->metaObject();
    int idx = metaObject->indexOfProperty( property->propertyName().toUtf8().data() );

	if ( idx == -1 )
	{
        return;
	}

    QMetaProperty metaProperty = metaObject->property( idx );
    if (metaProperty.isEnumType())
	{
        if (metaProperty.isFlagType())
		{
            //metaProperty.write( mPropertyObject, intToFlag(metaProperty.enumerator(), value.toInt()));
		}
        else
		{
            //metaProperty.write( mPropertyObject, intToEnum(metaProperty.enumerator(), value.toInt()));
		}
    }
	else
	{
        metaProperty.write( mPropertyObject, value);
    }
}

void Editor::SetPropertySheetObject( QObject *object )
{
    if ( mPropertyObject == object)
	{
        return;
	}

    if ( mPropertyObject ) {
    	QListIterator< QtProperty* > it( mTopLevelProperties );
        while (it.hasNext()) {
            mUi->propertyList->removeProperty(it.next());
        }
        mTopLevelProperties.clear();
    }

    mPropertyObject = object;

    if ( !mPropertyObject )
	{
        return;
	}

    AddClassProperties( mPropertyObject->metaObject() );
}
