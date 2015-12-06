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
#include "ProcyonQtUtil.h"
#include <Qt>
#include <QEvent>
#include <QKeyEvent>
#include <QPointF>

using namespace Procyon;

ProcyonKeyCode ProcyonQtUtil::TranslateKeyCode( int keycode )
{
	switch ( keycode )
	{
		// Numerics
		case Qt::Key_0: return KEY_0;
		case Qt::Key_1: return KEY_1;
		case Qt::Key_2: return KEY_2;
		case Qt::Key_3: return KEY_3;
		case Qt::Key_4: return KEY_4;
		case Qt::Key_5: return KEY_5;
		case Qt::Key_6: return KEY_6;
		case Qt::Key_7: return KEY_7;
		case Qt::Key_8: return KEY_8;
		case Qt::Key_9: return KEY_9;

		// Latin chars
		case Qt::Key_A: return KEY_A;
		case Qt::Key_B: return KEY_B;
		case Qt::Key_C: return KEY_C;
		case Qt::Key_D: return KEY_D;
		case Qt::Key_E: return KEY_E;
		case Qt::Key_F: return KEY_F;
		case Qt::Key_G: return KEY_G;
		case Qt::Key_H: return KEY_H;
		case Qt::Key_I: return KEY_I;
		case Qt::Key_J: return KEY_J;
		case Qt::Key_K: return KEY_K;
		case Qt::Key_L: return KEY_L;
		case Qt::Key_M: return KEY_M;
		case Qt::Key_N: return KEY_N;
		case Qt::Key_O: return KEY_O;
		case Qt::Key_P: return KEY_P;
		case Qt::Key_Q: return KEY_Q;
		case Qt::Key_R: return KEY_R;
		case Qt::Key_S: return KEY_S;
		case Qt::Key_T: return KEY_T;
		case Qt::Key_U: return KEY_U;
		case Qt::Key_V: return KEY_V;
		case Qt::Key_W: return KEY_W;
		case Qt::Key_X: return KEY_X;
		case Qt::Key_Y: return KEY_Y;
		case Qt::Key_Z: return KEY_Z;

		// Misc Latin keys
		case Qt::Key_Space: 		return KEY_SPACE;
		case Qt::Key_Exclam: 		return KEY_EXCLAM;
		case Qt::Key_QuoteDbl: 		return KEY_QUOTE_DBL;
		case Qt::Key_NumberSign: 	return KEY_NUM_SIGN;
		case Qt::Key_Dollar: 		return KEY_DOLLAR;
		case Qt::Key_Percent: 		return KEY_PERCENT;
		case Qt::Key_Ampersand: 	return KEY_AMPERSAND;
		case Qt::Key_Apostrophe: 	return KEY_APOSTROPHE;
		case Qt::Key_ParenLeft: 	return KEY_PAREN_LEFT;
		case Qt::Key_ParenRight: 	return KEY_PAREN_RIGHT;
		case Qt::Key_Asterisk: 		return KEY_ASTERISK;
		case Qt::Key_Plus: 			return KEY_PLUS;
		case Qt::Key_Comma: 		return KEY_COMMA;
		case Qt::Key_Minus: 		return KEY_MINUS;
		case Qt::Key_Period: 		return KEY_PERIOD;
		case Qt::Key_Slash: 		return KEY_SLASH;
		case Qt::Key_Colon: 		return KEY_COLON;
		case Qt::Key_Semicolon: 	return KEY_SEMICOLON;
		case Qt::Key_Less: 			return KEY_LESS;
		case Qt::Key_Equal: 		return KEY_EQUAL;
		case Qt::Key_Greater: 		return KEY_GREATER;
		case Qt::Key_Question: 		return KEY_QUESTION;
		case Qt::Key_At: 			return KEY_AT;
		case Qt::Key_BracketLeft: 	return KEY_BRACKET_LEFT;
		case Qt::Key_Backslash: 	return KEY_BACK_SLASH;
		case Qt::Key_BracketRight: 	return KEY_BRACKET_RIGHT;
		case Qt::Key_AsciiCircum: 	return KEY_CIRCUM;
		case Qt::Key_Underscore: 	return KEY_UNDERSCORE;
		case Qt::Key_Agrave: 		return KEY_GRAVE;
		case Qt::Key_BraceLeft: 	return KEY_BRACE_LEFT;
		case Qt::Key_Bar: 			return KEY_BAR;
		case Qt::Key_BraceRight: 	return KEY_BRACE_RIGHT;
		case Qt::Key_AsciiTilde: 	return KEY_TILDE;

		// Function keys
		case Qt::Key_F1: 			return KEY_F1;
		case Qt::Key_F2: 			return KEY_F2;
		case Qt::Key_F3: 			return KEY_F3;
		case Qt::Key_F4: 			return KEY_F4;
		case Qt::Key_F5: 			return KEY_F5;
		case Qt::Key_F6: 			return KEY_F6;
		case Qt::Key_F7: 			return KEY_F7;
		case Qt::Key_F8: 			return KEY_F8;
		case Qt::Key_F9: 			return KEY_F9;
		case Qt::Key_F10: 			return KEY_F10;

		// Modifier keys
		case Qt::Key_Shift: 		return KEY_SHIFT_L; // No KEY_SHIFT_R
		case Qt::Key_Control: 		return KEY_CTRL_L; // No KEY_CTRL_R
		case Qt::Key_Meta: 			return KEY_META_L; // No KEY_META_R
		case Qt::Key_Alt: 			return KEY_ALT_L; // No KEY_ALT_R
		case Qt::Key_Super_L:		return KEY_SUPER_L;
		case Qt::Key_Super_R: 		return KEY_SUPER_R;
		case Qt::Key_CapsLock: 		return KEY_CAPS_LOCK;

		// Arrow key
		case Qt::Key_Up: 			return KEY_UP;
		case Qt::Key_Down: 			return KEY_DOWN;
		case Qt::Key_Left: 			return KEY_LEFT;
		case Qt::Key_Right: 		return KEY_RIGHT;

		// Misc keys
		case Qt::Key_Backspace: 	return KEY_BACKSPACE;
		case Qt::Key_Tab: 			return KEY_TAB;
		case Qt::Key_Return: 		return KEY_RETURN;
		case Qt::Key_Pause: 		return KEY_PAUSE;
		case Qt::Key_Escape: 		return KEY_ESCAPE;
		case Qt::Key_Delete: 		return KEY_DELETE;

		case Qt::Key_unknown: return KEY_UNKNOWN;
		default: return KEY_UNKNOWN;
	}
}


Procyon::InputEvent ProcyonQtUtil::TranslateKeyEvent( const QKeyEvent* qtevent )
{
	assert( qtevent->type() == QEvent::Type::KeyRelease
		|| 	qtevent->type() == QEvent::Type::KeyPress );

	InputEvent ev( EVENT_KEY_DOWN );
    ev.keycode = qtevent->nativeScanCode();
    ev.keysym = ProcyonQtUtil::TranslateKeyCode( qtevent->key() );
    ev.unicode = qtevent->text().at( 0 ).unicode();
    ev.modifiers = 0;// NYI

    return ev;
}

glm::vec2 ProcyonQtUtil::QPointFToVec2( const QPointF& point )
{
	return glm::vec2( point.x(), point.y() );
}