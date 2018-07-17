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
#include "MainLoop.h"
#include "Audio/AudioDevice.h"
#include "Platform/Window.h"
#include "Platform/Keyboard.h"
#include "Platform/Mouse.h"
#include "Graphics/Renderer.h"
#include "Graphics/GL/GLContext.h"
#include "Image.h"
#include "Console.h"

#include <thread>

namespace Procyon {

    static double Now()
    {
        typedef std::chrono::steady_clock Clock;
        typedef std::chrono::milliseconds Ms;
        return std::chrono::time_point_cast<Ms>( Clock::now() )
            .time_since_epoch().count() / 1000.0;
    }

	MainLoop::MainLoop( const std::string& windowTitle, unsigned width, unsigned height )
        : mAvgFPS( (double)TARGET_FPS )
		, mFrame( 0 )
	{
        mStartTime      = Now();
        mSimTime.tsl   = 0.0f;
		mSimTime.dt    = 0.0f;

		mWindow = IWindow::Allocate( windowTitle, width, height );

		mWindow->SetEventListener( this );

		// Create the audio device
		mAudioDev = new AudioDevice();

		// Create the renderer
		mRenderer = new Renderer( mWindow );

		// Initialize console
		Console_Init();
	}

    double MainLoop::SecsSinceLaunch() const
    {
        return Now() - mStartTime;
    }

	MainLoop::~MainLoop()
	{
		Console_Destroy();

		delete mRenderer;
		delete mAudioDev;
		delete mWindow;
    }

    void MainLoop::HandleInputEvent( const InputEvent& ev )
    {
        if ( ev.type == EVENT_KEY_DOWN &&
             ev.keysym == KEY_GRAVE )
        {
            Console_Toggle();
            return;
        }
		if ( ev.type == EVENT_KEY_DOWN &&
             ev.keysym == KEY_F10 )
        {
            mWindow->SetFullscreen( !mWindow->GetFullscreen() );
            return;
        }
        if ( ev.type == EVENT_TEXT && ev.unicode == '`' )
        {
            return; // consume text `
        }

        if ( Console_IsOpen() )
        {
            if ( Console_HandleEvent( ev ) )
            {
                return;
            }
        }

        // Do some stuff with ev?
        switch( ev.type )
        {
            case EVENT_KEY_DOWN:
                PROCYON_DEBUG( "RawInput", "Got KeyDown: %s, Ctrl: %s, Meta: %s"
                    , ProcyonKeyCodeToStr( ev.keysym )
                    , (ev.modifiers & MODIFIER_CTRL) ? "true" : "false"
                    , (ev.modifiers & MODIFIER_META) ? "true" : "false");

                OnKeyDown( ev );
                break;
            case EVENT_KEY_REPEAT:
                PROCYON_DEBUG( "RawInput", "Got KeyRepeat: %s, Ctrl: %s, Meta: %s"
                    , ProcyonKeyCodeToStr( ev.keysym )
                    , ( ev.modifiers & MODIFIER_CTRL ) ? "true" : "false"
                    , ( ev.modifiers & MODIFIER_META ) ? "true" : "false" );

                OnKeyRepeat( ev );
                break;
            case EVENT_KEY_UP:
                PROCYON_DEBUG( "RawInput", "Got KeyUp: %s, Ctrl: %s, Meta: %s"
                    , ProcyonKeyCodeToStr( ev.keysym )
                    , (ev.modifiers & MODIFIER_CTRL) ? "true" : "false"
                    , (ev.modifiers & MODIFIER_META) ? "true" : "false");
                OnKeyUp( ev );
                break;
            case EVENT_TEXT:
                PROCYON_DEBUG( "RawInput", "Got Text: Unicode: %c"
                    , ev.unicode );
                break;
            case EVENT_MOUSE_DOWN:
                PROCYON_DEBUG( "RawInput", "Got MouseDown X: %f, Y: %f", ev.mousex, ev.mousey );
                OnMouseDown( ev );
                break;
            case EVENT_MOUSE_UP:
                PROCYON_DEBUG( "RawInput", "Got MouseUp X: %f, Y: %f", ev.mousex, ev.mousey );
                OnMouseUp( ev );
                break;
            case EVENT_MOUSE_MOVE:
                PROCYON_DEBUG( "MoveInput", "Got MouseMove X: %f, Y: %f", ev.mousex, ev.mousey );
                OnMouseMoved( ev );
                break;
            case EVENT_WINDOW_CHANGED:
                PROCYON_DEBUG( "RawInput", "Got WindowChanged X: %i, Y: %i, Height: %i, Width: %i"
                    , ev.windowx, ev.windowy, ev.height, ev.width );
                OnWindowChanged( ev );
                break;
        }
    }

	void MainLoop::Run()
	{
		float prevFrameStart = 0.0f;
        while ( mWindow->IsOpen() )
        {
			float frameStart = (float)SecsSinceLaunch();
			float frameDelta = 0.0f;
			if ( mFrame != 0 )
			{
				frameDelta = frameStart - prevFrameStart;

				// Update average fps lerp
				mAvgFPS = mAvgFPS * 0.9 + ( 1.0 / frameDelta ) * 0.1;
			}
			prevFrameStart = frameStart;

            Frame( frameDelta );

			// Framerate limit
			float processDelta = (float)SecsSinceLaunch() - frameStart;
			if ( processDelta < TARGET_HZ )
			{
				int sleepmicros = (int)( ( TARGET_HZ - processDelta ) * 1.0e6 );
				PROCYON_DEBUG( "MainLoop", "Sleeping for %i", sleepmicros );
				std::this_thread::sleep_for( std::chrono::microseconds( sleepmicros ) );
			}

        }
	}

	void MainLoop::Frame( float dt )
	{
		if ( mFrame != 0 )
		{
			mSimTime.dt = glm::clamp( dt, 0.0f, MAX_DT );
			mSimTime.tsl += mSimTime.dt;
		}

		mWindow->PollEvents();
		Keyboard::Poll( mWindow->HasFocus() );
		Mouse::Poll( mWindow->HasFocus() );

		Console_Process( mSimTime );
		Process( mSimTime );

		mRenderer->BeginRender();
			Render();
			Console_Render( mRenderer );
		mRenderer->EndRender();

		mFrame++;
	}

} /* namespace Procyon */
