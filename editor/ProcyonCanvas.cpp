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
#include "Renderer.h"
#include "World.h"
#include "Console.h"
#include "Sprite.h"
#include "ProcyonQtUtil.h"
#include <QTimer>
#include <QKeyEvent>
#include "Window.h"
#include "Camera.h"
#include "Grid.h"
#include "EditorAssets.h"

using namespace Procyon;

ProcyonCanvas::ProcyonCanvas(QWidget* Parent)
	: QOpenGLWidget(Parent)
    , mCamera( NULL )
    , mRenderer( NULL )
    , mGhostTile( NULL )
{
    // Accepts both clicking and tabbing focus, enabling keyboard events.
    setFocusPolicy( Qt::StrongFocus );

    // Receive all mouse events
    setMouseTracking( true );

    // Enable msaa
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setSamples( 4 );

    setFormat( fmt );
}

ProcyonCanvas::~ProcyonCanvas()
{
    Console_Destroy();

    delete mRenderer;
}

void ProcyonCanvas::SetShowGrid( bool render )
{
    mGrid->SetEnabled( render );
}

void ProcyonCanvas::SetGridSize( float gridSize )
{
    mGrid->SetGridSize( (float)TILE_PIXEL_SIZE * gridSize );
}

// Reset the camera transform back to the origin.
void ProcyonCanvas::ResetCamera()
{
    mCamera->SetPosition( 0.0f, 0.0f );
    mCamera->SetZoom( 1.0f );
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
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(1000 / 60);

    // Setup the viewport camera
    mCamera     = new Camera2D();
    mCamera->SetPosition( 0.0f, 0.0f );
    mCamera->OrthographicProj( -(float)width()/2.0f, (float)width()/2.0f, -(float)height()/2.0f, (float)height()/2.0f );

    // Create the renderer
    mRenderer   = new Renderer();

    // Create the viewport grid
    mGrid       = new Grid( (float)TILE_PIXEL_SIZE );

    // Load the bundled assets.
    EditorAssets::Load();

    mGhostTile  = new Sprite( EditorAssets::sTileTexture );

    mWorld = new Procyon::World();

    // Notify to setup initial state
    emit CameraChanged( mCamera ); 

    // Start the DeltaTime tracker
    mElapsed.start();
}

void ProcyonCanvas::paintGL()
{
    // Setup dt
    float elapsedSecs = mElapsed.elapsed() / 1000.0f;
    FrameTime ft = {
        .dt = elapsedSecs - mLastDt.tsl,
        .tsl = elapsedSecs
    };
    mLastDt = ft;

    // Advance the Console
    Console_Process( ft );

    //TODO: Clear the screen
    int hex = 0x99CCFF;
    glClearColor( ((hex >> 16)&0xff)/255.0f, ((hex >> 8)&0xff)/255.0f, (hex&0xff)/255.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //TODO: Enable some required gl settings
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glEnable( GL_MULTISAMPLE );
    //glHint(GL_POINT_SMOOTH, GL_NICEST);
    //glHint( GL_LINE_SMOOTH, GL_NICEST );
    //glHint(GL_POLYGON_SMOOTH, GL_NICEST);

    //glEnable(GL_POINT_SMOOTH);
    //glEnable( GL_LINE_SMOOTH );
    //glEnable(GL_POLYGON_SMOOTH);

    // Render the scene
    mRenderer->BeginRender();
    {   
        mRenderer->ResetCameras( *mCamera );

        mWorld->Render( mRenderer );

        mGrid->Render( mRenderer );

        mRenderer->DrawWorldLine( glm::vec2(0.0f, 0.0f), glm::vec2(32.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
        mRenderer->DrawWorldLine( glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 32.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) );

        mRenderer->Draw( mGhostTile );

        Console_Render( mRenderer );
    }
    mRenderer->EndRender();
}

void ProcyonCanvas::keyPressEvent( QKeyEvent* event )
{
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
        const float kCameraSpeed = 0.5f;

        // Move the camera
        switch( ev.keysym )
        {
            case KEY_LEFT:  mCamera->Move(  -kCameraSpeed,  0.0f ); break;
            case KEY_RIGHT: mCamera->Move(  kCameraSpeed,  0.0f ); break;
            case KEY_UP:    mCamera->Move(  0.0f,  kCameraSpeed ); break;
            case KEY_DOWN:  mCamera->Move(  0.0f, -kCameraSpeed ); break;
            case KEY_TILDE: Console_Toggle(); break;
        }
    }
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
    return mCamera->ScreenToWorld( ndc );
}

// Called when the mouse is clicked over this widget.
void ProcyonCanvas::mousePressEvent( QMouseEvent* event )
{
    // Store mouse down position for mouse move drag.
    mMouseLast = WindowToScreen( event->pos() );
    mDragStart = event->pos();
}

void ProcyonCanvas::mouseReleaseEvent( QMouseEvent* event )
{
    glm::vec2 scr = WindowToWorld( event->pos() );
    PROCYON_DEBUG( "ProcyonCanvas", "Mouse Release Event <%f, %f>", scr.x, scr.y );
    mWorld->SetTileType( POINT_TO_TILE( scr.x, scr.y ), TILE_1 ); 
}

// Called when the mouse is moved over this widget *only* while a mouse button is pressed 
// (since mouse tracking is disabled for this).
void ProcyonCanvas::mouseMoveEvent( QMouseEvent* event )
{
    // If the mouse left is pressed, dragging is attempted
    if ( event->buttons() & Qt::LeftButton )
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
    // Compute delta from last known mouse pos
    glm::vec2 ndc = WindowToScreen( event->pos() );
    glm::vec2 delta = mCamera->ScreenDirToWorldDir( ndc - mMouseLast );

    // Store mouse pos for next time
    mMouseLast = ndc;

    // Move the camera
    mCamera->Move( -delta * glm::vec2( 0.5f ) );

    // Notify
    emit CameraChanged( mCamera );
}

void ProcyonCanvas::OnMouseMove( QMouseEvent* event )
{
    mGhostTile->SetPosition( WindowToWorld( event->pos()) );
    //PROCYON_DEBUG( "ProcyonCanvas", "MouseMove <%.2f, %.2f>", event->pos().x(), event->pos().y() );
}

#define MIN_ZOOM 0.2f
#define MAX_ZOOM 10.0f

// Called on mouse wheel.
void ProcyonCanvas::wheelEvent( QWheelEvent* event )
{
    // Rotation delta in 1/8 degrees. 
    // positive indicates forward from the user, negative is towards the user.
    QPoint angleDelta = event->angleDelta();
    float yScroll = (float)angleDelta.y() / 188.0f;
    yScroll /= 32.0f;

    // Apply zoom amount
    if ( mCamera->GetZoom() - yScroll <= MIN_ZOOM )
    {
        mCamera->SetZoom( MIN_ZOOM );
    }
    else if ( mCamera->GetZoom() - yScroll >= MAX_ZOOM )
    {
        mCamera->SetZoom( MAX_ZOOM );
    }
    else
    {
        mCamera->ZoomIn( yScroll );
    }

    PROCYON_DEBUG( "ProcyonCanvas", "yScroll %f WheelEvent %f", yScroll, mCamera->GetZoom() );
    event->accept();
}