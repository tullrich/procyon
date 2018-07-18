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

#ifndef _X11_GL_CONTEXT_H
#define _X11_GL_CONTEXT_H

#include "ProcyonCommon.h"
#include "Graphics/GL/GLContext.h"
#include "X11Window.h"

#include <GL/glx.h>
#include <GL/gl.h>

namespace Procyon {
namespace Unix {

	class X11GLContext : public GL::GLContextBase
	{
	public:
		X11GLContext( X11Window *window );
		virtual ~X11GLContext();

		virtual void MakeCurrent();
		virtual void SwapBuffers();

	protected:
		bool 		CreateX11Context();

		X11Window* 		mWindow;

		GLXFBConfig 	mFBConfig;
		GLXContext 		mContext;
		GLXWindow		mGLXWindow;
	};

} /* namespace Unix */
} /* namespace Procyon */

#endif /* _X11_GL_CONTEXT_H */
