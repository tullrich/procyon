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
#ifndef _PLATFORM_INPUT_H
#define _PLATFORM_INPUT_H

#include "ProcyonCommon.h"

namespace Procyon {

	struct KeyboardState;
	struct MouseState;
	class IWindow;

	class PlatformInput
	{
	public:
		static void PollKeyboardState( KeyboardState* out );
		static void PollMouseState( MouseState* out );
	    static glm::ivec2 GetMousePosition();
	    static glm::ivec2 GetMousePosition( const IWindow* relative );
		static void SetMousePosition( const glm::ivec2& position );
		static void SetMousePosition( const glm::ivec2& position, const IWindow* relative );
	};
} /* namespace Procyon */

#endif /* _PLATFORM_INPUT_H */
