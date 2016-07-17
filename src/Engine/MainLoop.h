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
#ifndef _MAIN_LOOP_H
#define _MAIN_LOOP_H

#include "ProcyonCommon.h"
#include "Window.h"

#define TARGET_FPS 60
#define TARGET_HZ 1.0 / (double)TARGET_FPS

namespace Procyon {

	namespace GL {
		class IGLContext;
	}

	class MainLoop : public IInputEventListener
	{
	public:
	    				MainLoop( const std::string& windowTitle, unsigned width, unsigned height );
	    				~MainLoop();

		virtual void 	Initialize() { };
	    void 			Run();
	    virtual void    Cleanup() { };

	protected:
        virtual void 	HandleInputEvent( const InputEvent& ev );
    	double 			SecsSinceLaunch() const;
	    void    		Frame();

	    virtual void    Process( FrameTime t ) { };
	    virtual void    Render() { };

	    // input
	    virtual void    OnKeyDown( const InputEvent& ev ) { };
	    virtual void    OnKeyUp( const InputEvent& ev ) { };
	    virtual void    OnMouseDown( const InputEvent& ev ) { };
	    virtual void    OnMouseUp( const InputEvent& ev ) { };
	    virtual void    OnMouseMoved( const InputEvent& ev ) { };
	    virtual void    OnWindowChanged( const InputEvent& ev ) { };

	    IWindow*      	mWindow;
	    GL::IGLContext* mContext;

	    FrameTime       mPrevTime;
	    double			mStartTime;
	    double 			mAvgFPS;
	};

} /* namespace Procyon */

#endif /* _MAIN_LOOP_H */
