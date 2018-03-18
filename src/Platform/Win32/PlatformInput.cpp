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

#include <Windows.h>

namespace Procyon {

	static int TranslateKeyToVKKey[ PROCYON_KEY_CODE_COUNT ] =
	{
		0x0, // KEY_UNKNOWN

		// Numeric keys
		0x30, // KEY_0
		0x31, // KEY_1
		0x32, // KEY_2
		0x33, // KEY_3
		0x34, // KEY_4
		0x35, // KEY_5
		0x36, // KEY_6
		0x37, // KEY_7
		0x38, // KEY_8
		0x39, // KEY_9

		// Alphabetical keys
		0x41, // KEY_A
		0x42, // KEY_B
		0x43, // KEY_C
		0x44, // KEY_D
		0x45, // KEY_E
		0x46, // KEY_F
		0x47, // KEY_G
		0x48, // KEY_H
		0x49, // KEY_I
		0x4A, // KEY_J
		0x4B, // KEY_K
		0x4C, // KEY_L
		0x4D, // KEY_M
		0x4E, // KEY_N
		0x4F, // KEY_O
		0x50, // KEY_P
		0x51, // KEY_Q
		0x52, // KEY_R
		0x53, // KEY_S
		0x54, // KEY_T
		0x55, // KEY_U
		0x56, // KEY_V
		0x57, // KEY_W
		0x58, // KEY_X
		0x59, // KEY_Y
		0x5A, // KEY_Z

		VK_SPACE, // KEY_SPACE
		VK_OEM_7, // KEY_APOSTROPHE
		VK_OEM_COMMA, // KEY_COMMA
		VK_OEM_MINUS, // KEY_MINUS
		VK_OEM_PERIOD, // KEY_PERIOD
		VK_OEM_2, // KEY_SLASH
		VK_OEM_1, // KEY_SEMICOLON
		VK_OEM_PLUS, // KEY_EQUAL
		VK_OEM_4, // KEY_BRACKET_LEFT
		VK_OEM_5, // KEY_BACK_SLASH
		VK_OEM_6, // KEY_BRACKET_RIGHT
		VK_OEM_3, // KEY_GRAVE

		// Function keys
		VK_F1, // KEY_F1
		VK_F2, // KEY_F2
		VK_F3, // KEY_F3
		VK_F4, // KEY_F4
		VK_F5, // KEY_F5
		VK_F6, // KEY_F6
		VK_F7, // KEY_F7
		VK_F8, // KEY_F8
		VK_F9, // KEY_F9
		VK_F10, // KEY_F10

		// Modifier keys
		VK_LSHIFT, // KEY_SHIFT_L
		VK_RSHIFT, // KEY_SHIFT_R
		VK_LCONTROL, // KEY_CTRL_L
		VK_RCONTROL, // KEY_CTRL_R
		VK_LMENU, // KEY_ALT_L
		VK_RMENU, // KEY_ALT_R
		VK_LWIN, // KEY_SUPER_L
		VK_RWIN, // KEY_SUPER_R

		// Arrow key
		VK_UP, // KEY_UP
		VK_DOWN, // KEY_DOWN
		VK_LEFT, // KEY_LEFT
		VK_RIGHT, // KEY_RIGHT

		// Misc keys
		VK_CAPITAL, // KEY_CAPS_LOCK
		VK_BACK, // KEY_BACKSPACE
		VK_TAB, // KEY_TAB
		VK_RETURN, // KEY_RETURN
		VK_ESCAPE, // KEY_ESCAPE
		VK_PAUSE, // KEY_PAUSE
		VK_DELETE, // KEY_DELETE
		VK_INSERT, // KEY_INSERT
		VK_HOME, // KEY_HOME
		VK_END, // KEY_END
		VK_PRIOR, // KEY_PAGE_UP
		VK_NEXT, // KEY_PAGE_DOWN

		// Numpad keys
		VK_NUMPAD0, // KEY_NUMPAD_0
		VK_NUMPAD1, // KEY_NUMPAD_1
		VK_NUMPAD2, // KEY_NUMPAD_2
		VK_NUMPAD3, // KEY_NUMPAD_3
		VK_NUMPAD4, // KEY_NUMPAD_4
		VK_NUMPAD5, // KEY_NUMPAD_5
		VK_NUMPAD6, // KEY_NUMPAD_6
		VK_NUMPAD7, // KEY_NUMPAD_7
		VK_NUMPAD8, // KEY_NUMPAD_8
		VK_NUMPAD9, // KEY_NUMPAD_9
		VK_ADD, // KEY_NUMPAD_ADD
		VK_SUBTRACT, // KEY_NUMPAD_SUBTRACT
		VK_MULTIPLY, // KEY_NUMPAD_MULTIPLY
		VK_DIVIDE, // KEY_NUMPAD_DIVIDE
		VK_DECIMAL, // KEY_NUMPAD_DECIMAL
	};

	/* static */ void PlatformInput::PollKeyboardState( KeyboardState* out )
	{
		for ( int key = 1; key < PROCYON_KEY_CODE_COUNT; ++key )
		{
			const int vkKey	= TranslateKeyToVKKey[ key ];
			if ( vkKey != 0 )
			{
				const SHORT	vkState	= GetAsyncKeyState( vkKey );
				out->keys[ key ] = ( vkState & 0x8000 ) != 0;
			}
		}
	}

	static int TranslateMouseBtnToVKKey[ PROCYON_MOUSE_BUTTON_COUNT ] =
	{
		0x0, // MOUSE_BTN_UNKNOWN
		VK_LBUTTON, // MOUSE_BTN_LEFT
		VK_RBUTTON, // MOUSE_BTN_RIGHT
		VK_MBUTTON, // MOUSE_BTN_MIDDLE
		VK_XBUTTON2, // MOUSE_BTN_FORWARD
		VK_XBUTTON1, // MOUSE_BTN_BACK
		0x0, // MOUSE_SCROLL_UP
		0x0, // MOUSE_SCROLL_DOWN
		0x0, // MOUSE_SCROLL_LEFT
		0x0  // MOUSE_SCROLL_RIGHT
	};

	/* static */ void PlatformInput::PollMouseState( MouseState* out )
	{
		for ( int btn = 1; btn < PROCYON_MOUSE_BUTTON_COUNT; ++btn )
		{
			const int vkKey	= TranslateMouseBtnToVKKey[ btn ];
			if ( vkKey > 0 )
			{
				const SHORT	vkState	= GetAsyncKeyState( vkKey );
				out->btns[ btn ] = ( vkState & 0x8000 ) != 0;
			}
		}
	}

	/* static */ glm::ivec2 PlatformInput::GetMousePosition()
	{
		POINT point;
		GetCursorPos( &point );
		PROCYON_DEBUG("Mouse", "Client pos <%i, %i>", point.x, point.y );
		return glm::ivec2( point.x, point.y );
	}

	/* static */ glm::ivec2 PlatformInput::GetMousePosition( const IWindow* relative )
	{
		POINT point;
		GetCursorPos( &point );
		ScreenToClient( (HWND)relative->GetNativeHandle(), &point );
		return glm::ivec2( point.x, point.y );
	}

	/* static */ void PlatformInput::SetMousePosition( const glm::ivec2& position )
	{
		SetCursorPos( position.x, position.y );
	}

	/* static */ void PlatformInput::SetMousePosition( const glm::ivec2& position, const IWindow* relative )
	{
		POINT point;
		point.x = position.x;
		point.y = position.y;
		ClientToScreen( (HWND)relative->GetNativeHandle(), &point );
		SetCursorPos( point.x, point.y );
	}
} /* namespace Procyon */
