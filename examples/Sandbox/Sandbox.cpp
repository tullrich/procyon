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

#include "Console.h"
#include "Audio/AudioDevice.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderCore.h"
#include "Platform/Keyboard.h"
#include "Platform/Mouse.h"
#include "XmlMap.h"
#include "SandboxAssets.h"

Sandbox::Sandbox()
    : MainLoop( "Sandbox", SANDBOX_WINDOW_WIDTH, SANDBOX_WINDOW_HEIGHT )
    , mJoyStick( NULL )
    , mPlayer( NULL )
    , mCamera( NULL )
    , mCustomMap( NULL )
	, mFpsText( NULL )
	, mWorld( NULL )
{
}

void Sandbox::Initialize( int argc, char *argv[] )
{
    SandboxAssets::Load();

    if ( argc >= 2 )
    {
        mCustomMap = LoadMap( argv[ 1 ] );
    }

    mWindow->SetIcon( *SandboxAssets::sWindowIcon );
	mRenderer->SetClearColor( glm::vec4( 42.0f/225.0f, 47.0f/255.0f, 67.0f/255.0f, 1.0f ) );

	mFpsText = new Text( SandboxAssets::sMainFont, 18 );
	mFpsText->SetColor( glm::vec3( 1.0f ) );

	mWorld = new World();

    if ( mCustomMap )
    {
		mWorld->LoadMap( mCustomMap );
    }
    else
    {
		mWorld->LoadMap( SandboxAssets::sMap );
    }

	// Create the player
	mPlayer   = new Player( mWorld );

    // Create the camera
    mCamera = new Camera2D();
    mCamera->OrthographicProj(
          -SANDBOX_WINDOW_WIDTH / 2.0f, SANDBOX_WINDOW_WIDTH / 2.0f
        , -SANDBOX_WINDOW_HEIGHT / 2.0f, SANDBOX_WINDOW_HEIGHT / 2.0f );
	mCamera->SetPosition( mPlayer->GetPosition() );

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
            if ( je.type == JOY_EVENT_BTN_DOWN )
            {
                if ( je.btn == JOY_BTN_SOUTH )
				{
					mPlayer->Jump();
				}
                PROCYON_DEBUG( "Input", "Joy button event %s", ProcyonJoyButtonToStr( je.btn ) );
            }
            else
            {
                PROCYON_DEBUG( "Input", "Joy axis event" );
            }
        }

        double stickValue = mJoyStick->GetAxisValue( AXIS_LEFT_STICK_X );
        mPlayer->SetLeftRightInput( (float) stickValue );
    }

	int lrInput = (int)Keyboard::IsKeyDown( KEY_D ) - (int)Keyboard::IsKeyDown( KEY_A );
    mPlayer->SetLeftRightInput( (float)lrInput );
	if ( Mouse::OnButtonDown( MOUSE_BTN_FORWARD ) )
	{
		mPlayer->Jump();
		Mouse::SetPosition( glm::ivec2( mPlayer->GetPosition() ), mWindow );
	}

    mPlayer->Process( t );

	mPolyLine.Process( t, mPlayer );

    const float kCameraLerpSpeed = 0.25f;

	glm::vec2 target = mPlayer->GetPosition() + glm::vec2(0.0f, CAMERA_VERTICAL_OFFSET);
    mCamera->SetPosition( mCamera->GetPosition() * (1.0f - kCameraLerpSpeed) + target * kCameraLerpSpeed );

    const RenderFrameStats& stats = mRenderer->GetRenderCore()->GetFrameStats();
    std::stringstream builder;
	mFpsText->SetPosition( glm::floor( 6.0f-mCamera->GetWidth() / 2.0f ), glm::floor( -mCamera->GetHeight() / 2.0f ) );
    builder << "fps " << (int)mAvgFPS << " batches " << stats.batches << " quads " << stats.totalquads;
    builder << " [min " << stats.batchmin << " max " << stats.batchmax << "]";
	mFpsText->SetText( builder.str() );
}

void Sandbox::Render()
{
	mRenderer->ResetCameras(*mCamera);

	mWorld->Render( mRenderer );

	mPlayer->Draw( mRenderer );

	mRenderer->DrawRectShape( glm::vec2( Mouse::GetPosition( mWindow ) ),
		glm::vec2( 5.0f ),
		0.0f,
		glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );

	mPolyLine.Draw( mRenderer );

	mRenderer->Draw( mFpsText );
}

void Sandbox::OnMouseMoved( const InputEvent& ev )
{
	glm::vec2 screenPos = glm::vec2( glm::inverse( mCamera->GetProjection() )
		* glm::vec3( ev.mousex * 2.0f - 1.0f, -ev.mousey * 2.0f + 1.0f, 1.0f ) );

	if ( Mouse::IsButtonDown( MOUSE_BTN_LEFT ) )
	{
		mPolyLine.SetEndpoint( screenPos );
	}
}

void Sandbox::OnWindowChanged( const InputEvent& ev )
{
	PROCYON_DEBUG( "Sandbox", "OnWindowChanged <%i, %i>", ev.width, ev.height );
	mCamera->OrthographicProj( -ev.width / 2.0f, ev.width / 2.0f, -ev.height / 2.0f, ev.height / 2.0f );
}

Map* Sandbox::LoadMap( std::string filePath )
{
    XmlMap *map = new XmlMap( filePath );
    if ( !map->Load() )
    {
        delete map;
        return nullptr;
    }
    return map;
}
