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

#ifndef _WIN32_GL_CONTEXT_H
#define _WIN32_GL_CONTEXT_H

#include "ProcyonCommon.h"
#include "Graphics/GL/GLContext.h"

namespace Procyon {
namespace Win32 {

	class Win32Window;

	class Win32GLContext : public GL::GLContextBase
	{
	public:
		Win32GLContext( Win32Window *window );
		virtual ~Win32GLContext();

		virtual void MakeCurrent();
		virtual void SwapBuffers();

	protected:
		Win32Window* 	mWindow;
	};

} /* namespace Win32 */
} /* namespace Procyon */

#endif /* _WIN32_GL_CONTEXT_H */
