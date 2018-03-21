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
#include <GL/glew.h>
#include <QApplication>
#include "ProcyonCanvas.h"
#include "Graphics/Renderer.h"
#include "Collision/World.h"
#include "Console.h"
#include "Graphics/Sprite.h"
#include "ProcyonQtUtil.h"
#include <QTimer>
#include <QKeyEvent>
#include "Platform/Window.h"
#include "Graphics/Camera.h"
#include "Grid.h"
#include "EditorAssets.h"
#include "Editor.h"
#include "MapDocument.h"
#include "EditorSettings.h"

#define MIN_ZOOM 0.2f
#define MAX_ZOOM 10.0f

using namespace Procyon;

ProcyonCanvas::ProcyonCanvas( QWidget* Parent, Editor* editor )
	: QGLWidget(Parent)
	, mEditor( editor )
    , mRenderer( NULL )
    , mGhostTile( NULL )
    , mActiveDocument( NULL )
    , mTimer( NULL )
    , mCamera( NULL )
    , mStroke( 0 )
{
    // Accepts both clicking and tabbing focus, enabling keyboard events.
    setFocusPolicy( Qt::StrongFocus );

    // Receive all mouse events
    setMouseTracking( true );

    // Enable msaa
    QGLFormat fmt = QGLFormat::defaultFormat();
    //fmt.setSamples( 4 );
	fmt.setSwapInterval( 0 );
	//fmt.setSwapBehavior( QGLFormat::DoubleBuffer );

    setFormat( fmt );

	glInit();
}

ProcyonCanvas::~ProcyonCanvas()
{
    delete mGhostTile;
    delete mGrid;
    delete mCamera;
    delete mRenderer;

    Console_Destroy();
}

void ProcyonCanvas::SetGridSize( float gridSize )
{
    mGrid->SetGridSize( (float)TILE_PIXEL_SIZE * gridSize );
}

float ProcyonCanvas::GetGridSize() const
{
   return  mGrid->GetGridSize();
}

// Reset the camera transform back to the origin.
void ProcyonCanvas::ResetCamera()
{
    mCamera->SetPosition( glm::vec2( (float)TILE_PIXEL_SIZE * 5.0f ) );
    mCamera->SetZoom( 1.0f );

    emit CameraChanged( mCamera );
}

void ProcyonCanvas::PauseRendering()
{
    disconnect(mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    mTimer->stop();
}

void ProcyonCanvas::StartRendering()
{
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    mTimer->start( 1000 / 60 );
}

glm::vec2 ProcyonCanvas::GetCameraPosition() const
{
    return mCamera->GetPosition();
}

Procyon::Rect ProcyonCanvas::GetCameraViewport() const
{
    return mCamera->GetScreenRect();
}

void ProcyonCanvas::OnActiveDocumentChanged( MapDocument *doc )
{
    if ( mActiveDocument )
    {
        disconnect( mActiveDocument, SIGNAL( DocumentPreparingToSave() )
           , this, SLOT( SaveCameraState() ) );

        // Store current cam state before the switch
        mActiveDocument->SaveCameraState( mCamera );
    }

    // Switch
    mActiveDocument = doc;

    if ( mActiveDocument )
    {
        connect( mActiveDocument, SIGNAL( DocumentPreparingToSave() )
            , this, SLOT( SaveCameraState() ) );

        mMapBounds = Rect( 0, mActiveDocument->GetSize().y * TILE_PIXEL_SIZE,
            mActiveDocument->GetSize().x * TILE_PIXEL_SIZE, mActiveDocument->GetSize().y * TILE_PIXEL_SIZE );

        // Restore previous state
        if ( mCamera )
        {
            mActiveDocument->RestoreCameraState( mCamera );
            emit CameraChanged( mCamera );
        }
    }
}

void ProcyonCanvas::ToggleShowGrid( bool render )
{
    mGrid->SetEnabled( render );
}

bool ProcyonCanvas::GetShowGrid() const
{
    return mGrid->GetEnabled();
}

void ProcyonCanvas::SaveCameraState()
{
    // Store current cam state before the save
    mActiveDocument->SaveCameraState( mCamera );
}

void ProcyonCanvas::initializeGL()
{
    // Init glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if ( err != GLEW_OK || !glewIsSupported("GL_VERSION_3_3") )
    {
        //TODO: exit with grace
        exit(1);
    }

    // Initialize the console
    Console_Init();

    // Setup the repaint timer
    mTimer      = new QTimer(this);

    // Create the renderer
    mRenderer   = new Renderer( NULL );
	mRenderer->SetClearColor( glm::vec4(121.0f/255.0f, 121.0f/255.0f, 121.0f/255.0f, 1.0f ) );

    // Setup the viewport camera
    mCamera     = new Procyon::Camera2D();
    mCamera->OrthographicProj( -(float)1280.0f/2.0f, (float)1280.0f/2.0f, -(float)1280.0f/2.0f, (float)1280.0f/2.0f );
    ResetCamera();

    // Create the viewport grid
    mGrid       = new Grid( (float)TILE_PIXEL_SIZE );
    mGrid->SetEnabled( EditorSettings::Get().GetGridVisible() );
    mGrid->SetGridSize( (float)TILE_PIXEL_SIZE * EditorSettings::Get().GetGridSize() );

    // Load the bundled assets.
    EditorAssets::Load();

    mGhostTile  = new Sprite( EditorAssets::sTileTexture );

    // Setup signals
    connect( &EditorSettings::Get(), SIGNAL( GridVisibilityChanged( bool ) ), this, SLOT( ToggleShowGrid( bool ) ) );
    connect( &EditorSettings::Get(), SIGNAL( GridSizeChanged( float ) ), this, SLOT( SetGridSize( float ) ) );

    // start time-since-launch
    mElapsed.start();

    // Start the DeltaTime tracker
    StartRendering();
}

void ProcyonCanvas::paintGL()
{
    // Setup dt
    float elapsedSecs = mElapsed.elapsed() / 1000.0f;
    Procyon::FrameTime ft = {
        elapsedSecs - mLastDt.tsl,
        elapsedSecs
    };
    mLastDt = ft;

    // Advance the Console
    Console_Process( ft );

    // Enable some required gl settings
	glViewport( 0, 0, mCamera->GetWidth(), mCamera->GetHeight() );

    // Render the scene
    mRenderer->BeginRender();
    {
        if ( mActiveDocument )
        {
            mRenderer->ResetCameras( *mCamera );

			int hex2 = 0x2f2f43;//0xcccccc;
			mRenderer->DrawRectShape( glm::vec2( mActiveDocument->GetSize() * TILE_PIXEL_SIZE ) * 0.5f, mActiveDocument->GetSize() * TILE_PIXEL_SIZE, 0.0f
				, glm::vec4( ((hex2 >> 16)&0xff)/255.0f, ((hex2 >> 8)&0xff)/255.0f, (hex2&0xff)/255.0f, 1.0f ) );

            mActiveDocument->Render( mRenderer );

			//mActiveDocument->GetRootSceneObject()->Render( mRenderer );

            mGrid->Render( mRenderer, mActiveDocument->GetSize() );

            //mRenderer->DrawWorldLine( glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
            //mRenderer->DrawWorldLine( glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 32.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) );
            //mRenderer->DrawWorldLine( glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
            //mRenderer->DrawWorldLine( glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 32.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) );

            mRenderer->Draw( mGhostTile );
        }

        Console_Render( mRenderer );
    }
    mRenderer->EndRender();
}

void ProcyonCanvas::resizeGL( int w, int h )
{
    if ( !mActiveDocument )
        return;

	float add = 0.0f;
    mCamera->OrthographicProj(
		  -(float)w/2.0f + add, (float)w/2.0f + add
		, -(float)h/2.0f + add, (float)h/2.0f + add);
    emit CameraChanged( mCamera );
}

void ProcyonCanvas::keyPressEvent( QKeyEvent* event )
{
    if ( !mActiveDocument )
        return;

    // Translate from Qt to Procyon event
    InputEvent ev = ProcyonQtUtil::TranslateKeyEvent( event );
    PROCYON_DEBUG( "ProcyonCanvas", "Keydown Event '%li' '%c'", ev.unicode, ev.unicode );

    // If the Console is open, let it handle all key events
    if ( Console_IsOpen() )
    {
        Console_HandleEvent( ev );
    }
    else
    {
        const float kCameraSpeed = 10.0f;

        // Move the camera
        switch( ev.keysym )
        {
            case KEY_LEFT:  MoveCamera( -kCameraSpeed,  0.0f ); break;
            case KEY_RIGHT: MoveCamera( kCameraSpeed,  0.0f ); break;
            case KEY_UP:    MoveCamera( 0.0f,  kCameraSpeed ); break;
            case KEY_DOWN:  MoveCamera( 0.0f, -kCameraSpeed ); break;
            case KEY_GRAVE: Console_Toggle(); break;
        }
    }
}

void ProcyonCanvas::Zoom( float amount )
{
	PROCYON_INFO( "Editor", "<%f, %f> -> <%f, %f>"
		, mCamera->GetWidth(),  mCamera->GetHeight()
		, mCamera->GetWidth() / 0.77f, mCamera->GetHeight() / 0.77f );

   // Apply zoom amount
    if ( mCamera->GetZoom() - amount <= MIN_ZOOM )
    {
        mCamera->SetZoom( MIN_ZOOM );
    }
    else if ( mCamera->GetZoom() - amount >= MAX_ZOOM )
    {
        mCamera->SetZoom( MAX_ZOOM );
    }
    else
    {
        mCamera->ZoomIn( amount );
    }

    emit CameraChanged( mCamera );
}

void ProcyonCanvas::SetCamera( float xPos, float yPos )
{
    glm::vec2 newPos;
    newPos.x = glm::clamp( xPos, 0.0f, ( float )( mActiveDocument->GetSize().x * TILE_PIXEL_SIZE ) );
    newPos.y = glm::clamp( yPos, 0.0f, ( float )( mActiveDocument->GetSize().y * TILE_PIXEL_SIZE ) );

    mCamera->SetPosition( newPos );

    // Notify
    emit CameraChanged( mCamera );
}

void ProcyonCanvas::MoveCamera( float xMove, float yMove )
{
    SetCamera( xMove + mCamera->GetPosition().x, yMove + mCamera->GetPosition().y );
}

// Convert a point from window to screen coordinates. ( screen is normalized [-1, 1] )
glm::vec2 ProcyonCanvas::WindowToScreen( const QPointF& point )
{
    glm::vec2 winpos = ProcyonQtUtil::QPointFToVec2( point ) / glm::vec2( width(), -height() );
    return winpos * glm::vec2( 2.0f ) - glm::vec2( 1.0f, -1.0f );
}

// Convert a point from window to world coordinates.
glm::vec2 ProcyonCanvas::WindowToWorld( const QPointF& point )
{
    glm::vec2 ndc = WindowToScreen( point );

    if ( mActiveDocument )
    {
        return mCamera->ScreenToWorld( ndc );
    }
    return ndc;
}

// Called when the mouse is clicked over this widget.
void ProcyonCanvas::mousePressEvent( QMouseEvent* event )
{
    if ( !mActiveDocument )
        return;

    // Store mouse down position for mouse move drag.
    mMouseLast = WindowToScreen( event->pos() );
    mDragStart = event->pos();
    mStroke++;

    glm::vec2 scr = WindowToWorld( event->pos() );
    if ( event->buttons() & Qt::RightButton )
    {
        PROCYON_DEBUG( "ProcyonCanvas", "OnMousePress" );
        mActiveDocument->SetTile( POINT_TO_TILE( scr.x, scr.y ), mEditor->GetSelectedTileId(), mStroke );
    }
    else if ( event->buttons() & Qt::MiddleButton ) // Erase
    {
        PROCYON_DEBUG( "ProcyonCanvas", "OnMousePress" );
        mActiveDocument->SetTile( POINT_TO_TILE( scr.x, scr.y ), 0, mStroke );
    }
}

void ProcyonCanvas::mouseReleaseEvent( QMouseEvent* event )
{
    /*glm::vec2 scr = WindowToWorld( event->pos() );
    PROCYON_DEBUG( "ProcyonCanvas", "Mouse Release Event <%f, %f>", scr.x, scr.y );*/
}

// Called when the mouse is moved over this widget *only* while a mouse button is pressed
// (since mouse tracking is disabled for this).
void ProcyonCanvas::mouseMoveEvent( QMouseEvent* event )
{
    // If the mouse left is pressed, dragging is attempted
    if ( event->buttons() )
    {
        // Prevent misdrags on click by providing a bit of leeway.
        float distFromStart = ( event->pos() - mDragStart ).manhattanLength();
        if ( distFromStart >= QApplication::startDragDistance() )
        {
            OnMouseDrag( event );
        }
    }
    else
    {
        // Otherwise we fire mouse moves.
        OnMouseMove( event );
    }
}

void ProcyonCanvas::OnMouseDrag( QMouseEvent* event )
{
    if ( !mActiveDocument )
        return;

    glm::vec2 scr = WindowToWorld( event->pos() );
    if ( event->buttons() & Qt::LeftButton )
    {
        // Compute delta from last known mouse pos
        glm::vec2 ndc = WindowToScreen( event->pos() );
        glm::vec2 delta = mCamera->ScreenDirToWorldDir( ndc - mMouseLast );
        MoveCamera( -delta.x, -delta.y );

        // Store mouse pos for next time
        mMouseLast = ndc;

    }
    else if ( event->buttons() & Qt::RightButton )
    {
        PROCYON_DEBUG( "ProcyonCanvas", "OnMouseDrag" );
        mActiveDocument->SetTile( POINT_TO_TILE( scr.x, scr.y ), mEditor->GetSelectedTileId(), mStroke );
    }
    else if ( event->buttons() & Qt::MiddleButton ) // Move the camera
    {
        PROCYON_DEBUG( "ProcyonCanvas", "OnMouseDrag" );
        mActiveDocument->SetTile( POINT_TO_TILE( scr.x, scr.y ), 0, mStroke );
    }

	updateGL();
}

void ProcyonCanvas::OnMouseMove( QMouseEvent* event )
{
    mGhostTile->SetPosition( WindowToWorld( event->pos()) );
    //PROCYON_DEBUG( "ProcyonCanvas", "MouseMove <%.2f, %.2f>", event->pos().x(), event->pos().y() );
}

// Called on mouse wheel.
void ProcyonCanvas::wheelEvent( QWheelEvent* event )
{
    // Rotation delta in 1/8 degrees.
    // positive indicates forward from the user, negative is towards the user.
    QPoint angleDelta = event->angleDelta();
    float xScroll = (float)angleDelta.x() / 32.0f;
    float yScroll = (float)angleDelta.y() / 32.0f;

    if ( mActiveDocument )
    {
    	MoveCamera( xScroll, yScroll );
    }

    event->accept();
}
