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

#include "Camera.h"

#include "editor.h"
#include "ui_editor.h"
#include "ProcyonCanvas.h"
#include "LogOutputTarget.h"

#include <thread>
#include <chrono>

using namespace Procyon;

EditorForm::EditorForm( QWidget *parent )
    : QMainWindow( parent )
    , mUi( new Ui::Editor )
    , mCanvas( NULL )
    , mCameraLabel( NULL )
{
    mUi->setupUi( this );

    // Setup the primary canvas viewport
	mCanvas = new ProcyonCanvas( mUi->centralwidget );
	QSizePolicy sizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	sizePolicy.setVerticalStretch( 1 );
	mCanvas->setSizePolicy( sizePolicy );
	mUi->centralVSplitter->addWidget( mCanvas );

    connect( mCanvas, SIGNAL( CameraChanged( const Procyon::Camera2D* ) )
    	   , this, SLOT( CanvasCameraChanged( const Procyon::Camera2D* ) ) );

    // Setup the Grid size QAction radio mode functionality
    mUi->actionDisableGrid->setCheckable( true );
    mUi->action1x1->setCheckable( true );
    mUi->action5x5->setCheckable( true );
    mUi->action1x1->setChecked( true );

    QActionGroup* group = new QActionGroup( this );
    group->setExclusive( true );
    mUi->actionDisableGrid->setActionGroup( group );
    mUi->action1x1->setActionGroup( group );
    mUi->action5x5->setActionGroup( group );
    connect( group, SIGNAL( triggered( QAction* ) ), this, SLOT( GridSizeChanged( QAction* ) ) );

    // Setup Reset btn
    connect( mUi->actionReset, SIGNAL( triggered() ), this, SLOT( ResetViewport() ) );

    // Setup Status bar
    mCameraLabel = new QLabel( this );
    statusBar()->addPermanentWidget( mCameraLabel );

    // Setup output log
    SetupOutputLog();
}

EditorForm::~EditorForm()
{
    delete mUi;
}

void EditorForm::SetupOutputLog()
{
    LogOutputView* outputLog = new LogOutputView( mUi->centralVSplitter );
    outputLog->setObjectName(QStringLiteral("outputLogger"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(outputLog->sizePolicy().hasHeightForWidth());
    outputLog->setSizePolicy(sizePolicy1);
    outputLog->setMinimumSize(QSize(0, 0));
    outputLog->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    outputLog->setReadOnly(true);
    outputLog->setMaximumBlockCount(200);
    mUi->centralVSplitter->addWidget( outputLog );
}

void EditorForm::GridSizeChanged( QAction* action )
{
	if ( action == mUi->actionDisableGrid )
	{
		mCanvas->SetShowGrid( false );
	}
	else
	{
		mCanvas->SetShowGrid( true );

		if ( action == mUi->action1x1 )
			mCanvas->SetGridSize( 1.0f );
		else if ( action == mUi->action5x5 )
			mCanvas->SetGridSize( 5.0f );
	}
}

void EditorForm::ResetViewport()
{
	mCanvas->ResetCamera();
}

void EditorForm::CanvasCameraChanged( const Camera2D* cam )
{
	glm::vec2 camPos = cam->GetPosition();

	QString camText = QString("<%1, %2>")
		.arg( (double)camPos.x, 0, 'g', 3, '0' )
		.arg( (double)camPos.y, 0, 'g', 3, '0' );
	mCameraLabel->setText( camText );
    mCameraLabel->setFrameStyle( QFrame::NoFrame );
}
