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
#include "Graphics/Renderer.h"
#include "Graphics/Sprite.h"
#include "Graphics/RenderCore.h"
#include "Platform/Keyboard.h"
#include "Platform/Mouse.h"
#include "XmlMap.h"
#include "SandboxAssets.h"

static glm::vec2 PixelToWorld(const glm::ivec2& pixel, const glm::ivec2& winSize, const Camera2D* cam)
{
	return glm::vec2( glm::inverse( cam->GetViewProjection() ) *
		glm::vec3( pixel.x / ( float )winSize.x * 2.0f - 1.0f, -pixel.y / ( float )winSize.y * 2.0f + 1.0f, 1.0f ) );
}

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

	// Create the tile map
	mWorld = new World();
	mWorld->LoadMap( ( mCustomMap ) ? mCustomMap : SandboxAssets::sMap );

	// Create the player
	mPlayer   = new Player( mWorld );

	// Poles
	for ( int i = 0; i < 2; i++ )
	{
		Sprite* lightpost = new Sprite( SandboxAssets::sLightPostTexture );
		lightpost->SetOrigin( 0.5f, 0.5f );
		lightpost->SetPosition( TILE_TO_WORLD( 6 + 6 * i, 1 ) );
		mStaticSprites.push_back( lightpost );
	}

	// Dumpsters
	for ( int i = 0; i < 2; i++ )
	{
		Sprite* dumpster = new Sprite( SandboxAssets::sDumpsterTexture );
		dumpster->SetOrigin( 0.5f, 0.5f );
		dumpster->SetPosition( TILE_TO_WORLD( 6 + 6 * i, 1 ));
		mStaticSprites.push_back( dumpster );
	}

	// Beams
	for ( int i = 0; i < 2; i++ )
	{
		Sprite* lightpost_beam = new Sprite( SandboxAssets::sLightPostBeamTexture );
		lightpost_beam->SetOrigin( 0.5f, 0.5f );
		lightpost_beam->SetPosition( TILE_TO_WORLD( 6 + 6 * i, 1 ) );
		mStaticSprites.push_back( lightpost_beam );
	}


    // Create the cameras
    mCamera = new Camera2D();
	mCamera->OrthographicProj( -SANDBOX_RESOLUTION_X / 2.0f, SANDBOX_RESOLUTION_X / 2.0f, -SANDBOX_RESOLUTION_Y / 2.0f, SANDBOX_RESOLUTION_Y / 2.0f );
	mCamera->SetPosition( mPlayer->GetPosition() );
	mScreenCamera = new Camera2D();
	mScreenCamera->OrthographicProj( -SANDBOX_WINDOW_WIDTH / 2.0f, SANDBOX_WINDOW_WIDTH / 2.0f, -SANDBOX_WINDOW_HEIGHT / 2.0f, SANDBOX_WINDOW_HEIGHT / 2.0f );

	// Fps Text
	mFpsText = new Text( SandboxAssets::sMainFont, 18 );
	mFpsText->SetPosition( 6.0f - SANDBOX_WINDOW_WIDTH / 2.0f, -SANDBOX_WINDOW_HEIGHT / 2.0f );
	mFpsText->SetColor( glm::vec4( 1.0f ) );

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
    delete mScreenCamera;
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

	if ( Mouse::OnButtonDown( MOUSE_BTN_LEFT ) )
	{
		mPlayer->Teleport( PixelToWorld( Mouse::GetPosition( mWindow ), mWindow->GetSize(), mCamera ) );
	}

	int lrInput = (int)Keyboard::IsKeyDown( KEY_D ) - (int)Keyboard::IsKeyDown( KEY_A );
    mPlayer->SetLeftRightInput( (float)lrInput );
	if ( Keyboard::IsKeyDown( KEY_SPACE ) )
	{
		mPlayer->Jump();
	}

    mPlayer->Process( t );

	mPolyLine.Process( t, mPlayer );

	// Move camera
	glm::vec2 target = mPlayer->GetPosition() + glm::vec2(0.0f, CAMERA_VERTICAL_OFFSET);
    mCamera->SetPosition( glm::floor( mCamera->GetPosition() * (1.0f - CAMERA_LERP_RATE) + target * CAMERA_LERP_RATE ) );

	// Update fps text
	mFpsText->SetText( BuildFPSString() );
}

void Sandbox::Render()
{
	mRenderer->ResetCameras( *mScreenCamera );
	mRenderer->PushCamera( *mCamera );

	// Draw background sprites
	for ( auto iter : mBackground )
	{
		mRenderer->Draw( iter );
	}

	// Draw Tiles
	mWorld->Render( mRenderer );

	// Draw sprites
	for ( auto iter : mStaticSprites )
	{
		mRenderer->Draw( iter );
	}

	mPlayer->Draw( mRenderer );

	mPolyLine.Draw( mRenderer );

	mRenderer->PopCamera();

	mRenderer->DrawRectShape(
		Mouse::GetPosition( mWindow ) * glm::ivec2( 1, -1 ),
		glm::vec2( 5.0f ),
		0.0f,
		glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );

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
	mScreenCamera->OrthographicProj( -ev.width / 2.0f, ev.width / 2.0f, -ev.height / 2.0f, ev.height / 2.0f );
	mFpsText->SetPosition( 6.0f - ev.width / 2.0f, -ev.height / 2.0f );
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

std::string Sandbox::BuildFPSString() const
{
    const RenderFrameStats& stats = mRenderer->GetRenderCore()->GetFrameStats();
	std::stringstream builder;
    builder << "fps " << (int)mAvgFPS << " batches " << stats.batches << " quads " << stats.totalquads;
    builder << " [min " << stats.batchmin << " max " << stats.batchmax << "]";
	return builder.str();
}
