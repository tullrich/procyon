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

#include "Win32GLContext.h"
#include "Win32Window.h"

#include <GLFW/glfw3.h>

namespace Procyon {
namespace Win32 {

	Win32GLContext::Win32GLContext( Win32Window *window )
		: mWindow( window )
	{
		// this first make current call creates the glfw context and must be
		// made before the glewinit() call.
		MakeCurrent();
		SharedInit();
	}

	Win32GLContext::~Win32GLContext()
	{
	}

	void Win32GLContext::MakeCurrent()
	{
		glfwMakeContextCurrent( mWindow->GetInternal() );
	}

	void Win32GLContext::SwapBuffers()
	{
		glfwSwapBuffers( mWindow->GetInternal() );
	}

} /* namespace Win32 */
} /* namespace Procyon */
