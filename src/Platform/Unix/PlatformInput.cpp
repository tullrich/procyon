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
#include "Platform/PlatformInput.h"
#include "Platform/Keyboard.h"
#include "Platform/Mouse.h"
#include "Platform/Window.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

namespace Procyon {

	static int TranslateKeyToKeySym[ PROCYON_KEY_CODE_COUNT ] =
	{
		0
	};

	/* static */ void PlatformInput::PollKeyboardState( KeyboardState* out )
	{
		/*for ( int key = 1; key < PROCYON_KEY_CODE_COUNT; ++key )
		{
			const int keySym	= TranslateKeyToKeySym[ key ];
			if ( keySym != 0 )
			{
				const SHORT	vkState	= GetAsyncKeyState( vkKey );
				out->keys[ key ] = ( vkState & 0x8000 ) != 0;
			}
		}*/
	}

	static int TranslateMouseBtnToKeySym[ PROCYON_MOUSE_BUTTON_COUNT ] =
	{
		0
	};

	/* static */ void PlatformInput::PollMouseState( MouseState* out )
	{
		/*for ( int btn = 1; btn < PROCYON_MOUSE_BUTTON_COUNT; ++btn )
		{
			const int keySym	= TranslateKeyToKeySym[ btn ];
			if ( keySym > 0 )
			{
				const SHORT	vkState	= GetAsyncKeyState( vkKey );
				out->btns[ btn ] = ( vkState & 0x8000 ) != 0;
			}
		}*/
	}

	/* static */ glm::ivec2 PlatformInput::GetMousePosition()
	{
		return glm::ivec2();
	}

	/* static */ glm::ivec2 PlatformInput::GetMousePosition( const IWindow* relative )
	{
		return glm::ivec2();
	}

	/* static */ void PlatformInput::SetMousePosition( const glm::ivec2& position )
	{
	}

	/* static */ void PlatformInput::SetMousePosition( const glm::ivec2& position, const IWindow* relative )
	{
	}
} /* namespace Procyon */
