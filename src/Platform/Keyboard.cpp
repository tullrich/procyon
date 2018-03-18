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
#include "Keyboard.h"
#include "PlatformInput.h"

#include <cstring>

namespace Procyon {

	/* static */ KeyboardState Keyboard::sCurrentKeyState = { 0 };
	/* static */ KeyboardState Keyboard::sPrevKeyState = { 0 };

	/* static */ void Keyboard::Reset()
	{
		memset( &sCurrentKeyState, 0, sizeof( KeyboardState ) );
		memset( &sPrevKeyState, 0, sizeof( KeyboardState ) );
	}

	/* static */ void Keyboard::Poll()
	{
		sPrevKeyState = sCurrentKeyState;
		PlatformInput::PollKeyboardState( &sCurrentKeyState );
	}

	/* static */ bool Keyboard::IsKeyUp( ProcyonKeyCode key )
	{
		return !sCurrentKeyState.keys[ int( key ) ];
	}

	/* static */ bool Keyboard::IsKeyDown( ProcyonKeyCode key )
	{
		return sCurrentKeyState.keys[ int( key ) ];
	}

	/* static */ bool Keyboard::OnKeyUp( ProcyonKeyCode key)
	{
		return !sCurrentKeyState.keys[ int( key ) ] && sPrevKeyState.keys[ int( key ) ];
	}

	/* static */ bool Keyboard::OnKeyDown( ProcyonKeyCode key )
	{
		return sCurrentKeyState.keys[ int( key ) ] && !sPrevKeyState.keys[ int( key ) ];
	}
} /* namespace Procyon */
