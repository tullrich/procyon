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
#include "X11Platform.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace Procyon {

    using namespace Unix;

	static KeySym TranslateKeyToKeySym[ PROCYON_KEY_CODE_COUNT ] =
	{
		0x0, // KEY_UNKNOWN

		// Numeric keys
		XK_0, // KEY_0
		XK_1, // KEY_1
		XK_2, // KEY_2
		XK_3, // KEY_3
		XK_4, // KEY_4
		XK_5, // KEY_5
		XK_6, // KEY_6
		XK_7, // KEY_7
		XK_8, // KEY_8
		XK_9, // KEY_9

        // Alphabetical keys
        XK_a, // KEY_A
        XK_b, // KEY_B
        XK_c, // KEY_C
        XK_d, // KEY_D
        XK_e, // KEY_E
        XK_f, // KEY_F
        XK_g, // KEY_G
        XK_h, // KEY_H
        XK_i, // KEY_I
        XK_j, // KEY_J
        XK_k, // KEY_K
        XK_l, // KEY_L
        XK_m, // KEY_M
        XK_n, // KEY_N
        XK_o, // KEY_O
        XK_p, // KEY_P
        XK_q, // KEY_Q
        XK_r, // KEY_R
        XK_s, // KEY_S
        XK_t, // KEY_T
        XK_u, // KEY_U
        XK_v, // KEY_V
        XK_w, // KEY_W
        XK_x, // KEY_X
        XK_y, // KEY_Y
        XK_z,  // KEY_Z

        XK_space, // KEY_SPACE
		XK_apostrophe, // KEY_APOSTROPHE
		XK_comma, // KEY_COMMA
		XK_minus, // KEY_MINUS
		XK_period, // KEY_PERIOD
		XK_slash, // KEY_SLASH
		XK_semicolon, // KEY_SEMICOLON
		XK_equal, // KEY_EQUAL
		XK_bracketleft, // KEY_BRACKET_LEFT
		XK_backslash, // KEY_BACK_SLASH
		XK_bracketright, // KEY_BRACKET_RIGHT
		XK_grave, // KEY_GRAVE

		// Function keys
		XK_F1, // KEY_F1
		XK_F2, // KEY_F2
		XK_F3, // KEY_F3
		XK_F4, // KEY_F4
		XK_F5, // KEY_F5
		XK_F6, // KEY_F6
		XK_F7, // KEY_F7
		XK_F8, // KEY_F8
		XK_F9, // KEY_F9
		XK_F10, // KEY_F10

		// Modifier keys
		XK_Shift_L, // KEY_SHIFT_L
		XK_Shift_R, // KEY_SHIFT_R
		XK_Control_L, // KEY_CTRL_L
		XK_Control_R, // KEY_CTRL_R
		XK_Alt_L, // KEY_ALT_L
		XK_Alt_R, // KEY_ALT_R
		XK_Super_L, // KEY_SUPER_L
		XK_Super_R, // KEY_SUPER_R

		// Arrow key
		XK_Up, // KEY_UP
		XK_Down, // KEY_DOWN
		XK_Left, // KEY_LEFT
		XK_Right, // KEY_RIGHT

		// Misc keys
		XK_Caps_Lock, // KEY_CAPS_LOCK
		XK_BackSpace, // KEY_BACKSPACE
		XK_Tab, // KEY_TAB
		XK_Return, // KEY_RETURN
		XK_Escape, // KEY_ESCAPE
		XK_Pause, // KEY_PAUSE
		XK_Delete, // KEY_DELETE
		XK_Insert, // KEY_INSERT
		XK_Home, // KEY_HOME
		XK_End, // KEY_END
		XK_Page_Down, // KEY_PAGE_UP
		XK_Page_Up, // KEY_PAGE_DOWN

		// Numpad keys
		XK_KP_0, // KEY_NUMPAD_0
		XK_KP_1, // KEY_NUMPAD_1
		XK_KP_2, // KEY_NUMPAD_2
		XK_KP_3, // KEY_NUMPAD_3
		XK_KP_4, // KEY_NUMPAD_4
		XK_KP_5, // KEY_NUMPAD_5
		XK_KP_6, // KEY_NUMPAD_6
		XK_KP_7, // KEY_NUMPAD_7
		XK_KP_8, // KEY_NUMPAD_8
		XK_KP_9, // KEY_NUMPAD_9
		XK_KP_Add, // KEY_NUMPAD_ADD
		XK_KP_Subtract, // KEY_NUMPAD_SUBTRACT
		XK_KP_Multiply, // KEY_NUMPAD_MULTIPLY
		XK_KP_Divide, // KEY_NUMPAD_DIVIDE
		XK_KP_Decimal, // KEY_NUMPAD_DECIMAL
	};

	/* static */ void PlatformInput::PollKeyboardState( KeyboardState* out )
	{
        char keyState[32];
        XQueryKeymap( gDisplay, keyState );

        for ( int key = 1; key < PROCYON_KEY_CODE_COUNT; ++key )
        {
			const int keySym	= TranslateKeyToKeySym[ key ];
			if ( keySym != 0 )
			{
                const int iKeyCodeToFind = XKeysymToKeycode( gDisplay, keySym );
                if ( iKeyCodeToFind != 0 )
                {
                    char state = keyState[ iKeyCodeToFind / 8 ] & ( 1 << ( iKeyCodeToFind % 8 ) );
				    out->keys[ key ] = state != 0;
                }
            }
        }
	}

	static unsigned TranslateMouseBtnToMask[ PROCYON_MOUSE_BUTTON_COUNT ] =
	{
        0x0, // MOUSE_BTN_UNKNOWN
		Button1Mask, // MOUSE_BTN_LEFT
		Button3Mask, // MOUSE_BTN_RIGHT
		Button2Mask, // MOUSE_BTN_MIDDLE
		Button4Mask, // MOUSE_BTN_FORWARD
		Button5Mask, // MOUSE_BTN_BACK
		0x0, // MOUSE_SCROLL_UP
		0x0, // MOUSE_SCROLL_DOWN
		0x0, // MOUSE_SCROLL_LEFT
		0x0  // MOUSE_SCROLL_RIGHT
	};

	/* static */ void PlatformInput::PollMouseState( MouseState* out )
	{
        Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;

        unsigned int mask_return = 0;
        XQueryPointer( gDisplay, DefaultRootWindow( gDisplay )
            , &root_return, &child_return
            , &root_x_return, &root_y_return
            , &win_x_return, &win_x_return, &mask_return );

        for ( int btn = 1; btn < PROCYON_MOUSE_BUTTON_COUNT; ++btn )
        {
            const unsigned mask	= TranslateMouseBtnToMask[ btn ];
            out->btns[ btn ] = ( mask_return & mask ) != 0;
            if (out->btns[btn])
            {
                PROCYON_DEBUG("PlatformInput", "Mouse %s Down", ProcyonMouseButtonToStr((ProcyonMouseButton)btn));
            }
        }
	}

	/* static */ glm::ivec2 PlatformInput::GetMousePosition()
	{
        xcb_query_pointer_cookie_t qp_cookie = xcb_query_pointer( gConnection, gScreen->root );
        xcb_query_pointer_reply_t *qp_reply = xcb_query_pointer_reply( gConnection, qp_cookie, NULL );

        glm::ivec2 ret;
        if (qp_reply) {
            ret = glm::ivec2( qp_reply->win_x, qp_reply->win_y );
            free (qp_reply);
        }
        return ret;
	}

	/* static */ glm::ivec2 PlatformInput::GetMousePosition( const IWindow* relative )
	{
        xcb_query_pointer_cookie_t qp_cookie = xcb_query_pointer( gConnection, (xcb_window_t)relative->GetNativeHandle() );
        xcb_query_pointer_reply_t *qp_reply = xcb_query_pointer_reply( gConnection, qp_cookie, NULL );

        glm::ivec2 ret;
        if (qp_reply) {
            ret = glm::ivec2( qp_reply->win_x, qp_reply->win_y );
            free (qp_reply);
        }
        return ret;
	}

	/* static */ void PlatformInput::SetMousePosition( const glm::ivec2& position )
	{
        xcb_warp_pointer( gConnection
            , XCB_NONE
            , gScreen->root
            , 0
            , 0
            , 0
            , 0
            , position.x
            , position.y
        );
        xcb_flush( gConnection );
	}

	/* static */ void PlatformInput::SetMousePosition( const glm::ivec2& position, const IWindow* relative )
	{
        xcb_warp_pointer( gConnection
            , XCB_NONE
            , relative->GetNativeHandle()
            , 0
            , 0
            , 0
            , 0
            , position.x
            , position.y
        );
        xcb_flush( gConnection );
	}
} /* namespace Procyon */
