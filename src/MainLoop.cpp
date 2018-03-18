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
        mPrevTime.tsl   = 0.0;
		mPrevTime.dt    = 0.0;

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
        while ( mWindow->IsOpen() )
        {
            double before = SecsSinceLaunch();

            Frame();

            double framerate =  1.0 / ( SecsSinceLaunch() - before );
			mAvgFPS = mAvgFPS * 0.9 + framerate * 0.1;
        }
	}

	void MainLoop::Frame()
	{
        FrameTime t;
        t.tsl   = (float)SecsSinceLaunch();
		if ( mFrame != 0)
		{
			t.dt    = t.tsl - mPrevTime.tsl;
		}
		else
		{
			t.dt = 0.0f;
		}
        mPrevTime = t;

		mWindow->PollEvents();
		Keyboard::Poll();
		Mouse::Poll();

		Console_Process( t );
		Process( t );

		mRenderer->BeginRender();
			Render();
			Console_Render( mRenderer );
		mRenderer->EndRender();
		mFrame++;

		float processRenderTime = (float)SecsSinceLaunch() - t.tsl;

		// framerate limit
		if ( processRenderTime < TARGET_HZ )
		{
			int sleepmicros = (int)( ( TARGET_HZ - processRenderTime ) * 1.0e6 );
			PROCYON_DEBUG( "MainLoop", "Sleeping for %i", sleepmicros );
			std::this_thread::sleep_for( std::chrono::microseconds( sleepmicros ) );
		}
	}

} /* namespace Procyon */
