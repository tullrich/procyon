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
#include "Sandbox.h"

#include "GLContext.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLProgram.h"
#include "GLBuffer.h"
#include "GLGeometry.h"
#include "GLMaterial.h"
#include "Renderer.h"
#include "RenderCore.h"
#include "Sprite.h"
#include "LineShape.h"
#include "RectShape.h"
#include "RenderContext.h"
#include "Console.h"
#include "Player.h"
#include "CustomSprites.h"
#include "Text.h"
#include "XmlMap.h"

#include "SandboxAssets.h"
#include "../../editor/Grid.h"

World*              world;
bool                LRInput[2];
Text*               fps;

Sandbox::Sandbox()
    : MainLoop( "Sandbox", 500, 500 )
    , mJoyStick( NULL )
    , mPlayer( NULL )
    , mRenderer( NULL )
    , mCamera( NULL )
    , mGrid( NULL )
    , mCustomMap( NULL )
{
}

void Sandbox::Initialize( int argc, char *argv[] )
{
    // Create the audio device
    mAudioDev = new AudioDevice();

    // Create the renderer
    mRenderer = new Renderer();

    SandboxAssets::Load();

    if ( argc >= 2 )
    {
        mCustomMap = LoadMap( argv[ 1 ] );
    }

    mWindow->SetIcon( *SandboxAssets::sWindowIcon );

    fps = new Text( SandboxAssets::sMainFont, 16 );

    world = new World();

    if ( mCustomMap )
    {
        world->LoadMap( mCustomMap );
    }
    else
    {
        world->LoadMap( SandboxAssets::sMap );
    }

    Console_Init();

    // Create the camera
    mCamera = new Camera2D();
    mCamera->SetPosition( 160.0f, 160.0f );
    mCamera->OrthographicProj( -160.0f, 160.0f, -160.0, 160.0f );

    mGrid = new Grid( (float)TILE_PIXEL_SIZE );

    // Create the player
    mPlayer   = new Player( world );

    // Create the optional joystick device (hardcoded for now)
    try
    {
        mJoyStick   = Joystick_Open( "/dev/input/event16" );
    }
    catch( ... ) { }
}

void Sandbox::Cleanup()
{
    delete mJoyStick;
    delete mPlayer;
    delete mCamera;

    delete mCustomMap;
    SandboxAssets::Destroy();
    Console_Destroy();

    delete mRenderer;
    delete mAudioDev;
}

void Sandbox::Process( FrameTime t )
{
    // Pump joystick
    if ( mJoyStick )
    {
        JoystickInputEvent je;
        while ( mJoyStick->Poll( je ) )
        {
            // use je
            if ( je.type == JOY_EVENT_KEY_DOWN )
            {
                if ( je.key == KEY_JOY_SOUTH )
                    mPlayer->Jump();
                PROCYON_DEBUG( "Input", "Joy key event %s", ProcyonKeyCodeToStr( je.key ) );
            }
            else
            {
                PROCYON_DEBUG( "Input", "Joy axis event" );
            }
        }

        double stickValue = mJoyStick->GetAxisValue( AXIS_LEFT_STICK_X );
        mPlayer->SetLeftRightInput( stickValue );
    }

    if ( LRInput[0] )
    {
        mPlayer->SetLeftRightInput( -1.0f );
    }
    else if ( LRInput[1] )
    {
        mPlayer->SetLeftRightInput( 1.0f );
    }
    mPlayer->Process( t );

    const float kCameraLerpSpeed = 1.0f;
    mCamera->SetPosition( mCamera->GetPosition() * (1.0f - kCameraLerpSpeed) + mPlayer->GetPosition() * kCameraLerpSpeed );

    Console_Process( t );

    const RenderFrameStats& stats = mRenderer->GetRenderCore()->GetFrameStats();
    std::stringstream builder;
    fps->SetPosition( int(6.0f-mCamera->GetWidth() / 2.0f), int(-mCamera->GetHeight() / 2.0f) );
    //fps->SetPosition( 6.0f-mCamera->GetWidth() / 2.0f, 6.0f-mCamera->GetHeight() / 2.0f );
    builder << "fps " << (int)mAvgFPS << " batches " << stats.batches << " quads " << stats.totalquads;
    builder << " [min " << stats.batchmin << " max " << stats.batchmax << "]";
    fps->SetText( builder.str() );
}

void Sandbox::Render()
{
    mContext->MakeCurrent();

    int hex = 0x99CCFF;
    glClearColor( ((hex >> 16)&0xff)/255.0f, ((hex >> 8)&0xff)/255.0f, (hex&0xff)/255.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );

    mRenderer->BeginRender();
    {
        mRenderer->ResetCameras( *mCamera );

        world->Render( mRenderer );

        //mGrid->Render( mRenderer );

        mRenderer->Draw( mPlayer->GetRenderable() );

        mRenderer->Draw( fps );

        Console_Render( mRenderer );
    }
    mRenderer->EndRender();

    mContext->SwapBuffers();
}

void Sandbox::OnKeyDown( const InputEvent& ev )
{
    if ( ev.keysym == KEY_UP )
    {
        mCamera->Move( 0.0f, 32.0f );
    }
    else if ( ev.keysym == KEY_LEFT )
    {
        mCamera->Move( -32.0f, 0.0f );
    }
    else if ( ev.keysym == KEY_DOWN )
    {
        mCamera->Move( 0.0f, -32.0f );
    }
    else if ( ev.keysym == KEY_RIGHT )
    {
        mCamera->Move( 32.0f, 0.0f );
    }
    else if ( ev.keysym == KEY_SPACE )
    {
        mPlayer->Jump();
    }
    else if ( ev.keysym == KEY_D )
    {
        LRInput[1] = true;
    }
    else if ( ev.keysym == KEY_A )
    {
        LRInput[0] = true;
    }
}

void Sandbox::OnKeyUp( const InputEvent& ev )
{
    if ( ev.keysym == KEY_D )
    {
        LRInput[1] = false;
    }
    else if ( ev.keysym == KEY_A )
    {
        LRInput[0] = false;
    }
}

void Sandbox::OnMouseDown( const InputEvent& ev )
{
    if ( ev.mousebutton == MOUSE_SCROLL_UP )
    {
        mCamera->ZoomOut( 0.1f );
    }
    else if( ev.mousebutton == MOUSE_SCROLL_DOWN )
    {
        mCamera->ZoomIn( 0.1f );
    }
}

void Sandbox::OnMouseUp( const InputEvent& ev ) { }

void Sandbox::OnMouseMoved( const InputEvent& ev ) { }

void Sandbox::OnWindowChanged( const InputEvent& ev )
{
    glViewport( 0, 0, ev.width, ev.height );
    mCamera->OrthographicProj( -ev.width / 2.0f, ev.width / 2.0f
        , -ev.height / 2.0f, ev.height / 2.0f );
    //mCamera->OrthographicProj( -0.5f, 0.5f, -0.5f, 0.5f );
}

Map* Sandbox::LoadMap( std::string filePath )
{
    XmlMap *map = new XmlMap( filePath );
    if ( !map->Load() )
    {
        delete map;
        return NULL;
    }
    return map;
}
