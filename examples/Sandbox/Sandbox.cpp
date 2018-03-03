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
#include "Console.h"
#include "Player.h"
#include "CustomSprites.h"
#include "Text.h"
#include "XmlMap.h"
#include "ProcyonVaser.h"

#include "SandboxAssets.h"
#include "../../editor/Grid.h"

World*              world;
bool                LRInput[ 2 ];
glm::vec2 			sMouseLoc;
bool 				sMouseDown;
Text*               fps;
std::vector< glm::highp_vec2 > sPoints;
std::vector< glm::highp_vec2 > sPoints2;

VASEr::polyline_opt sOpts;
VASEr::tessellator_opt sTessOpts;

Sandbox::Sandbox()
    : MainLoop( "Sandbox", SANDBOX_WINDOW_WIDTH, SANDBOX_WINDOW_HEIGHT )
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

    fps = new Text( SandboxAssets::sMainFont, 11 );

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
    mCamera->OrthographicProj( 
          -SANDBOX_WINDOW_WIDTH / 2.0f, SANDBOX_WINDOW_WIDTH / 2.0f
        , -SANDBOX_WINDOW_HEIGHT / 2.0f, SANDBOX_WINDOW_HEIGHT / 2.0f );

    mGrid = new Grid( (float)TILE_PIXEL_SIZE );

    // Create the player
    mPlayer   = new Player( world );

	memset( &sOpts, 0, sizeof( VASEr::polyline_opt ) );
	memset( &sTessOpts, 0, sizeof( VASEr::tessellator_opt ) );

	sOpts.tess = &sTessOpts;
	sTessOpts.tessellate_only = true;

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
        mPlayer->SetLeftRightInput( (float) stickValue );
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
    mCamera->SetPosition( glm::round( mCamera->GetPosition() * (1.0f - kCameraLerpSpeed) + mPlayer->GetPosition() * kCameraLerpSpeed ) );

    Console_Process( t );

    const RenderFrameStats& stats = mRenderer->GetRenderCore()->GetFrameStats();
    std::stringstream builder;
    fps->SetPosition( glm::floor( 6.0f-mCamera->GetWidth() / 2.0f ), glm::floor( -mCamera->GetHeight() / 2.0f ) );
    //fps->SetPosition( 6.0f-mCamera->GetWidth() / 2.0f, 6.0f-mCamera->GetHeight() / 2.0f );
    builder << "fps " << (int)mAvgFPS << " batches " << stats.batches << " quads " << stats.totalquads;
    builder << " [min " << stats.batchmin << " max " << stats.batchmax << "]";
    fps->SetText( builder.str() );

	static const float screen_x = 500.0f;
	static const int maxSamples = 200;
	static const float x_scale = screen_x / (float)maxSamples;

	static float accum = 0.0f;
	static int tick = 0;
	static float sampleHz = 1.0f / 15.0f;
	accum += t.dt;
	while ( accum >= sampleHz )
	{
		accum -= sampleHz;
		float x = tick % maxSamples * x_scale - screen_x / 2.0f;
		float y = sin( tick * sampleHz ) * 100.0f;

		if ( sPoints2.size() != maxSamples )
		{
			sPoints2.push_back( glm::vec2( x, y ) );
		}
		else
		{
			sPoints2[ tick % maxSamples ] = glm::vec2( x, y );
		}
		tick++;
	}
}

void Sandbox::Render()
{
    mContext->MakeCurrent();

    int hex = 0x99CCFF;
    glClearColor( ((hex >> 16)&0xff)/255.0f, ((hex >> 8)&0xff)/255.0f, (hex&0xff)/255.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
	glDisable( GL_MULTISAMPLE );

    mRenderer->BeginRender();
    {
        mRenderer->ResetCameras( *mCamera );

        world->Render( mRenderer );

        mRenderer->Draw( mPlayer->GetRenderable() );

        mGrid->Render( mRenderer, world->GetSize() );

        mRenderer->Draw( fps );
		//SandboxAssets::sTestTexture->SetMinMagFilter( GL_LINEAR, GL_LINEAR );
		//mRenderer->DrawTexture( SandboxAssets::sTestTexture, glm::vec2(), glm::vec2( 10.0f ), 0.0f );
		//mRenderer->DrawTexture( SandboxAssets::sTestTexture, glm::vec2(), glm::vec2(00.0f, 200.0f), 0.0f );

		float lerp = (sin( mCamera->GetPosition().x / 20.0f )+1.0f) / 2.0f;
		//mRenderer->DrawAALine( glm::vec2(), glm::vec2(50.0f), 15.0f, lerp, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f ) );

		//mRenderer->DrawPolyLine( sPoints, glm::vec4( 0.0f, 1.0f, 0.0f, 0.6f ), 20.f
		//	, PolyLineJoinMode::BEVEL, PolyLineCapMode::BUTT, 250.0f );

		//mRenderer->DrawPolyLine( sPoints2, glm::vec4( 0.0f, 1.0f, 0.0f, 0.6f ), 5.f
		//	, PolyLineJoinMode::BEVEL, PolyLineCapMode::BUTT, 250.0f );


		//VASEr::polyline( const Vec2*, Color, double W, int length, const polyline_opt*);


		if( sPoints.size() >= 4 )
		{
			sOpts.cap = VASEr::PLC_round;
			sOpts.feather = true;
			sOpts.feathering = 5.0;
			VASEr::VASErin::vertex_array_holder holder;
			sTessOpts.holder = &holder;

			const double weight = 15.0;
			const VaseColor color = GlmToVaseColor( glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
			//VASEr::segment( GlmToVaseVec2( sPoints[ 0 ] ), GlmToVaseVec2( sPoints[ 1 ] ), color, weight, &sOpts);

			std::vector< VaseVec2 > stuff( sPoints.size() );
			std::vector< VaseColor > stuffColor( sPoints.size() );
			for (size_t i = 0; i < sPoints.size(); i++)
			{
				PROCYON_DEBUG( "PolyLine", "sPoints[ %i ] <%f, %f>", i, sPoints[ i ].x, sPoints[ i ].y );
				VaseVec2& d = stuff[ i ];
				d.x = sPoints[ i ].x;
				d.y = sPoints[ i ].y;

				VaseColor& c = stuffColor[ i ];
				c.r = ( sPoints[ i ].x + 250.0f ) / 250.0f;
				c.g = ( sPoints[ i ].y + 250.0f ) / 250.0f;
				c.b = 0.0;
				c.a = 0.6f;
			}
			//VASEr::polyline( stuff.data(), stuffColor.data(), weight, stuff.size(), &sOpts );

			VASEr::polybezier_opt bazOpts;
			bazOpts.poly = &sOpts;
			VASEr::polybezier( stuff.data(), color, weight, (int) stuff.size(), &bazOpts );

			if ( holder.glmode == GL_TRIANGLES )
			{
				PROCYON_DEBUG( "PolyLine", "vertex_array_holder.vert %i mode: Triangles", holder.vert.size() );

				assert( holder.vert.size()/2 == holder.color.size()/4 );
				std::vector< ColorVertex > native( holder.vert.size()/2 );
				for ( int i = 0; i < holder.vert.size()/2; i++ )
				{
					ColorVertex& vert = native[ i ];
					vert.position[ 0 ] = holder.vert[ i * 2 ];
					vert.position[ 1 ] = holder.vert[ i * 2 + 1 ];
					vert.color[ 0 ] = holder.color[ i * 4 ];
					vert.color[ 1 ] = holder.color[ i * 4 + 1 ];
					vert.color[ 2 ] = holder.color[ i * 4 + 2 ];
					vert.color[ 3 ] = holder.color[ i * 4 + 3 ];
				}

				RenderCommand cmd;
		        cmd.op               = RENDER_OP_POLYGON;
		        cmd.program          = NULL;
		        cmd.flags            = RENDER_SCREEN_SPACE;
		        cmd.colorprimmode    = PRIMITIVE_TRIANGLE;
		        cmd.colorverts       = (ColorVertex*) native.data();
		        cmd.colorvertcount   = (int) native.size();
		        mRenderer->GetRenderCore()->AddOrAppendCommand( cmd );
			}
			else
			{
				const char* mode = NULL;
				switch( holder.glmode )
				{
					case GL_TRIANGLES: mode = "Triangles"; break;
					case GL_TRIANGLE_STRIP: mode = "Triangle Strip"; break;
					default: mode = "Other"; break;
				}
				PROCYON_DEBUG( "PolyLine", "Not Triangles: vertex_array_holder.vert %i mode: %s", holder.vert.size(), mode );
			}
		}

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
	else if ( ev.mousebutton == MOUSE_BTN_LEFT )
	{
		sPoints.push_back( sMouseLoc );
		sMouseDown = true;
	}
}

void Sandbox::OnMouseUp( const InputEvent& ev )
{
	if ( ev.mousebutton == MOUSE_BTN_LEFT )
	{
		sMouseDown = false;
	}
}

void Sandbox::OnMouseMoved( const InputEvent& ev )
{
	sMouseLoc = glm::vec2( glm::inverse( mCamera->GetProjection() )
		* glm::vec3( ev.mousex * 2.0f - 1.0f, -ev.mousey * 2.0f + 1.0f, 1.0f ) );

	if ( sMouseDown && sPoints.size() > 0 )
	{
		sPoints[ sPoints.size() - 1 ] = sMouseLoc;
	}
}

void Sandbox::OnWindowChanged( const InputEvent& ev )
{
	PROCYON_DEBUG( "Sandbox", "OnWindowChanged <%i, %i>", ev.width, ev.height );
    glViewport( 0, 0, ev.width, ev.height );
    mCamera->OrthographicProj( -ev.width / 2.0f, ev.width / 2.0f
        , -ev.height / 2.0f, ev.height / 2.0f );
	PROCYON_DEBUG( "Sandbox", "mCamera <%f, %f>", mCamera->GetWidth(), mCamera->GetHeight() );
	glm::mat3 proj = mCamera->GetProjection();
	PROCYON_DEBUG( "Sandbox", "GetProjection \n\t[%f, %f, %f]\n\t[%f, %f, %f]\n\t[%f, %f, %f]"
		, proj[0][0], proj[0][1], proj[0][2]
		, proj[1][0], proj[1][1], proj[1][2]
		, proj[2][0], proj[2][1], proj[2][2] );

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
