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
#ifndef _KEY_CODES_H
#define _KEY_CODES_H

#ifndef KEY_ENUM_COUNT_NAME
#	define KEY_ENUM_COUNT_NAME( type ) typ##_COUNT
#endif

#ifndef KEY_ENUM_BEGIN
#	define KEY_ENUM_BEGIN(typ) enum typ {
#endif

#ifndef KEY_ENUM
#	define KEY_ENUM(nam) nam
#endif

#ifndef KEY_ENUM_END
#	define KEY_ENUM_END(typ) , KEY_ENUM_COUNT_NAME(type) };
#endif

#ifndef PROCYON_KEY_CODE_COUNT
#	define PROCYON_KEY_CODE_COUNT \
		KEY_ENUM_COUNT_NAME( Procyon::ProcyonKeyCode )
#endif

namespace Procyon {

	KEY_ENUM_BEGIN( ProcyonKeyCode )
		KEY_ENUM( KEY_UNKNOWN ),

		// Numeric keys
		KEY_ENUM( KEY_0 ),
		KEY_ENUM( KEY_1 ),
		KEY_ENUM( KEY_2 ),
		KEY_ENUM( KEY_3 ),
		KEY_ENUM( KEY_4 ),
		KEY_ENUM( KEY_5 ),
		KEY_ENUM( KEY_6 ),
		KEY_ENUM( KEY_7 ),
		KEY_ENUM( KEY_8 ),
		KEY_ENUM( KEY_9 ),

		// Alphabetical keys
		KEY_ENUM( KEY_A ),
		KEY_ENUM( KEY_B ),
		KEY_ENUM( KEY_C ),
		KEY_ENUM( KEY_D ),
		KEY_ENUM( KEY_E ),
		KEY_ENUM( KEY_F ),
		KEY_ENUM( KEY_G ),
		KEY_ENUM( KEY_H ),
		KEY_ENUM( KEY_I ),
		KEY_ENUM( KEY_J ),
		KEY_ENUM( KEY_K ),
		KEY_ENUM( KEY_L ),
		KEY_ENUM( KEY_M ),
		KEY_ENUM( KEY_N ),
		KEY_ENUM( KEY_O ),
		KEY_ENUM( KEY_P ),
		KEY_ENUM( KEY_Q ),
		KEY_ENUM( KEY_R ),
		KEY_ENUM( KEY_S ),
		KEY_ENUM( KEY_T ),
		KEY_ENUM( KEY_U ),
		KEY_ENUM( KEY_V ),
		KEY_ENUM( KEY_W ),
		KEY_ENUM( KEY_X ),
		KEY_ENUM( KEY_Y ),
		KEY_ENUM( KEY_Z ),

		// Misc Latin keys
		KEY_ENUM( KEY_SPACE ),
		KEY_ENUM( KEY_EXCLAM ),
		KEY_ENUM( KEY_QUOTE_DBL ),
		KEY_ENUM( KEY_NUM_SIGN ),
		KEY_ENUM( KEY_DOLLAR ),
		KEY_ENUM( KEY_PERCENT ),
		KEY_ENUM( KEY_AMPERSAND ),
		KEY_ENUM( KEY_APOSTROPHE ),
		KEY_ENUM( KEY_PAREN_LEFT ),
		KEY_ENUM( KEY_PAREN_RIGHT ),
		KEY_ENUM( KEY_ASTERISK ),
		KEY_ENUM( KEY_PLUS ),
		KEY_ENUM( KEY_COMMA ),
		KEY_ENUM( KEY_MINUS ),
		KEY_ENUM( KEY_PERIOD ),
		KEY_ENUM( KEY_SLASH ),
		KEY_ENUM( KEY_COLON ),
		KEY_ENUM( KEY_SEMICOLON ),
		KEY_ENUM( KEY_LESS ),
		KEY_ENUM( KEY_EQUAL ),
		KEY_ENUM( KEY_GREATER ),
		KEY_ENUM( KEY_QUESTION ),
		KEY_ENUM( KEY_AT ),
		KEY_ENUM( KEY_BRACKET_LEFT ),
		KEY_ENUM( KEY_BACK_SLASH ),
		KEY_ENUM( KEY_BRACKET_RIGHT ),
		KEY_ENUM( KEY_CIRCUM ),
		KEY_ENUM( KEY_UNDERSCORE ),
		KEY_ENUM( KEY_GRAVE ),
		KEY_ENUM( KEY_BRACE_LEFT ),
		KEY_ENUM( KEY_BAR ),
		KEY_ENUM( KEY_BRACE_RIGHT ),
		KEY_ENUM( KEY_TILDE ),

		// Function keys
		KEY_ENUM( KEY_F1 ),
		KEY_ENUM( KEY_F2 ),
		KEY_ENUM( KEY_F3 ),
		KEY_ENUM( KEY_F4 ),
		KEY_ENUM( KEY_F5 ),
		KEY_ENUM( KEY_F6 ),
		KEY_ENUM( KEY_F7 ),
		KEY_ENUM( KEY_F8 ),
		KEY_ENUM( KEY_F9 ),
		KEY_ENUM( KEY_F10 ),

		// Modifier keys
		KEY_ENUM( KEY_SHIFT_L ),
		KEY_ENUM( KEY_SHIFT_R ),
		KEY_ENUM( KEY_CTRL_L ),
		KEY_ENUM( KEY_CTRL_R ),
		KEY_ENUM( KEY_META_L ),
		KEY_ENUM( KEY_META_R ),
		KEY_ENUM( KEY_ALT_L ),
		KEY_ENUM( KEY_ALT_R ),
		KEY_ENUM( KEY_SUPER_L ),
		KEY_ENUM( KEY_SUPER_R ),
		KEY_ENUM( KEY_CAPS_LOCK ),

		// Arrow key
		KEY_ENUM( KEY_UP ),
		KEY_ENUM( KEY_DOWN ),
		KEY_ENUM( KEY_LEFT ),
		KEY_ENUM( KEY_RIGHT ),

		// Misc keys
		KEY_ENUM( KEY_BACKSPACE ),
		KEY_ENUM( KEY_TAB ),
		KEY_ENUM( KEY_RETURN ),
		KEY_ENUM( KEY_PAUSE ),
		KEY_ENUM( KEY_ESCAPE ),
		KEY_ENUM( KEY_DELETE ),

		// Keypad keys

		// Joystick keys
		KEY_ENUM( KEY_JOY_NORTH ),
		KEY_ENUM( KEY_JOY_EAST ),
		KEY_ENUM( KEY_JOY_WEST ),
		KEY_ENUM( KEY_JOY_SOUTH ),
		KEY_ENUM( KEY_JOY_THUMBL ),
		KEY_ENUM( KEY_JOY_THUMBR ),
		KEY_ENUM( KEY_JOY_TOPL ),
		KEY_ENUM( KEY_JOY_TOPR ),
		KEY_ENUM( KEY_JOY_SELECT ),
		KEY_ENUM( KEY_JOY_START ),
		KEY_ENUM( KEY_JOY_MODE ),
		KEY_ENUM( KEY_JOY_DPAD_UP ),
		KEY_ENUM( KEY_JOY_DPAD_RIGHT ),
		KEY_ENUM( KEY_JOY_DPAD_LEFT ),
		KEY_ENUM( KEY_JOY_DPAD_DOWN )

	KEY_ENUM_END( ProcyonKeyCode )

} /* namespace Procyon */

#endif /* _KEY_CODES_H */