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
#include "ProcyonCommon.h"
#include "MainLoop.h"
#include "Renderer.h"
#include "GLContext.h"
#include "Sprite.h"
#include "World.h"
#include "Camera.h"
#include "SpinningBox.h"
#include <Box2D/Box2D.h>

using namespace Procyon::GL;
using namespace Procyon;

static b2Body* CreateDynamicBody( b2World* world, glm::vec2 position, glm::vec2 dims )
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set( PixelsToMeters( position.x ),PixelsToMeters( position.y ) );
    b2Body* b = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox( PixelsToMeters( dims.x ) / 2.0f, PixelsToMeters( dims.y ) / 2.0f );

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.1f;
    fixtureDef.friction = 0.3f;

    b->CreateFixture(&fixtureDef);

    return b;
}

static b2Body* CreateStaticBody( b2World* world, glm::vec2 position, glm::vec2 dims )
{
    // init body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set( PixelsToMeters( position.x ),PixelsToMeters( position.y ) );
    b2Body* b = world->CreateBody(&groundBodyDef);

    // attach shape to body
    b2PolygonShape groundBox;
    groundBox.SetAsBox( PixelsToMeters( dims.x ) / 2.0f, PixelsToMeters( dims.y ) / 2.0f );
    b->CreateFixture(&groundBox, 0.0f);

    return b;
}

static inline glm::vec2 b2ToGlmVec2( const b2Vec2& vec )
{
    return glm::vec2( vec.x, vec.y );
}

class Box2DTester : public MainLoop
{
public:

    Box2DTester()
    	: MainLoop( "Sandbox", 500, 500 )
    	, mRenderer( NULL )
    	, mCamera( NULL )
    {
    }

    virtual void Initialize()
    {
        // init world
        b2Vec2 gravity(0.0f, -10.0f);
        mPhysWorld = new b2World(gravity);

    	// Create the renderer
    	mRenderer = new Renderer();

    	// Create the camera
   		mCamera = new Camera2D();
    	mCamera->SetPosition( 160.0f, 160.0f );
    	mCamera->OrthographicProj( -160.0f, 160.0f, -160.0, 160.0f );
    }

    virtual void Cleanup()
    {
        delete mCamera;
        delete mRenderer;
        delete mPhysWorld;
    }

    virtual void Process( FrameTime t )
    {
	    // step phys sim
	    int32 velocityIterations = 8;
	    int32 positionIterations = 3;
	    mPhysWorld->Step( TARGET_HZ, velocityIterations, positionIterations);

	    for( auto pair : mSpriteBodies )
	    {
	        glm::vec2 posMeters = b2ToGlmVec2( pair.second->GetPosition() );
	        pair.first->SetPosition( posMeters * PIXELS_PER_METER );
	        pair.first->SetOrientation( pair.second->GetAngle() );
	        pair.first->Process( t );
	    }
    }

    virtual void Render()
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

	        for( auto pair : mSpriteBodies )
	        {
	            mRenderer->Draw( pair.first );
	        }
	    }
	    mRenderer->EndRender();

    	mContext->SwapBuffers();
    }

    virtual void OnMouseDown( const InputEvent& ev )
    {
    	if( ev.mousebutton == MOUSE_BTN_RIGHT || ev.mousebutton == MOUSE_BTN_LEFT )
	    {
	        glm::vec2 worldpos = mCamera->ScreenToWorld( 
	            glm::vec2( ( ev.mousex - 0.5f ) / 0.5f, -( ev.mousey - 0.5f ) / 0.5f ) );

	        Sprite* s = new SpinningBox();
	        s->SetPosition( worldpos );
	        s->SetDimensions( 32.0f, 32.0f );
	        s->SetOrigin( 0.5f, 0.5f );


	        b2Body* b = NULL;
            if ( ev.mousebutton == MOUSE_BTN_RIGHT )
            {
                b = CreateDynamicBody( mPhysWorld, worldpos, glm::vec2( 32.0f, 32.0f ) );
            }
            else
            {
                b = CreateStaticBody( mPhysWorld, worldpos, glm::vec2( 32.0f, 32.0f ) );
            }
	        mSpriteBodies.push_back( std::pair<Sprite*, b2Body*>( s, b ) );
	    }
    }

    virtual void OnWindowChanged( const InputEvent& ev ) { }

    typedef std::vector<std::pair<Sprite*, b2Body*>> SpriteBodyList;
	SpriteBodyList	mSpriteBodies;
    b2World*        mPhysWorld;

    Renderer*       mRenderer;
    Camera2D* 		mCamera;
};

int main( int argc, char *argv[] )
{
	LOGOG_INITIALIZE();
	{
		logog::Cout err;
        logog::GetFilterDefault().Group( "Console" );

        Box2DTester b2t;
        b2t.Initialize();
        b2t.Run();
        b2t.Cleanup();
	}
	
    LOGOG_SHUTDOWN();

	return 0;
}