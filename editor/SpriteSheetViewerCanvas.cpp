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

#include "SpriteSheetViewerCanvas.h"
#include "Graphics/Renderer.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Texture.h"
#include "ProcyonQtUtil.h"
#include "Platform/Window.h"
#include "Graphics/Camera.h"

#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

#define MIN_ZOOM 0.2f
#define MAX_ZOOM 10.0f

using namespace Procyon;

SpriteSheetViewerCanvas::SpriteSheetViewerCanvas( QWidget* Parent, Procyon::AnimatedSprite* sprite, QString texturePath  )
	: QGLWidget( Parent )
    , mRenderer( NULL )
    , mTimer( NULL )
    , mCamera( NULL )
    , mSprite( sprite )
    , mTexturePath( texturePath )
{
    // Accepts both clicking and tabbing focus, enabling keyboard events.
    setFocusPolicy( Qt::StrongFocus );

    // Receive all mouse events
    setMouseTracking( true );

    // Disable vsync if supported
    QGLFormat fmt = QGLFormat::defaultFormat();
    if ( fmt.swapInterval() != -1 )
    {
        fmt.setSwapInterval( 0 );
    }
    //fmt.setSamples( 4 ); // Enable msaa
    setFormat( fmt );

    // Init gl
	glInit();
}

SpriteSheetViewerCanvas::~SpriteSheetViewerCanvas()
{
    delete mCamera;
    delete mRenderer;
}

void SpriteSheetViewerCanvas::initializeGL()
{
    // Setup the repaint timer
    mTimer      = new QTimer(this);

    // Create the renderer
    mRenderer   = new Renderer( NULL );
	mRenderer->SetClearColor( glm::vec4(49.0f/255.0f, 77.0f/255.0f, 121.0f/255.0f, 1.0f ) );

    // Setup the viewport camera
    mCamera     = new Camera2D();
    mCamera->OrthographicProj( -(float)1280.0f/2.0f, (float)1280.0f/2.0f, -(float)1280.0f/2.0f, (float)1280.0f/2.0f );

    // start time-since-launch
    mElapsed.start();

    // Start the DeltaTime tracker
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    mTimer->start( 1000 / 60 );

    // Load the sprite texture
    Texture* texture = Texture::Allocate( mTexturePath.toUtf8().constData() );
    if ( texture )
    {
        mSprite->SetTexture( texture );
        mSprite->SetScale( glm::vec2( 4.0f ) );
    }
}

void SpriteSheetViewerCanvas::paintGL()
{
    // Setup dt
    float elapsedSecs = mElapsed.elapsed() / 1000.0f;
    FrameTime ft = {
        elapsedSecs - mLastDt.tsl,
        elapsedSecs
    };
    mLastDt = ft;

    mSprite->Process( ft );

    // Enable some required gl settings
	glViewport( 0, 0, mCamera->GetWidth(), mCamera->GetHeight() );

    // Render the scene
    mRenderer->BeginRender();
    {
        mRenderer->ResetCameras( *mCamera );

        mRenderer->Draw( mSprite );
    }
    mRenderer->EndRender();
}

void SpriteSheetViewerCanvas::resizeGL( int w, int h )
{
	float add = 0.0f;
    mCamera->OrthographicProj(
		  -(float)w/2.0f + add, (float)w/2.0f + add
		, -(float)h/2.0f + add, (float)h/2.0f + add);
}
