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
#ifndef _PROCYON_QT_UTIL
#define _PROCYON_QT_UTIL

#include "Window.h"

class QKeyEvent;
class QPointF;

class ProcyonQtUtil
{
public:

	// Translate Qt keycode to ProcyonKeyCode (returns KEY_UNKNOWN if unsupported).
	static Procyon::ProcyonKeyCode 	TranslateKeyCode( int keycode );

	// Translate a QEvent::KeyPress or QEvent::KeyRelease to InputEvent.
	static Procyon::InputEvent 		TranslateKeyEvent( const QKeyEvent* qtevent );

	// Translate a QPointF to glm::vec2
	static glm::vec2				QPointFToVec2( const QPointF& point );
};

#endif /* _PROCYON_QT_UTIL */