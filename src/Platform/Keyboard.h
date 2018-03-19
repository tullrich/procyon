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
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "KeyCodes.h"

namespace Procyon {

	struct KeyboardState
	{
		bool keys[ PROCYON_KEY_CODE_COUNT ];
	};

	class Keyboard
	{
	public:
		static void Reset();
		static void Poll( bool hasFocus );
		static bool	IsKeyUp( ProcyonKeyCode key );
		static bool	IsKeyDown( ProcyonKeyCode key );
		static bool	OnKeyUp( ProcyonKeyCode key);
		static bool	OnKeyDown( ProcyonKeyCode key );

	protected:
		static KeyboardState sCurrentKeyState;
		static KeyboardState sPrevKeyState;
	};
} /* namespace Procyon */

#endif /* _KEYBOARD_H */
