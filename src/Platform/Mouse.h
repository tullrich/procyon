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
#ifndef _MOUSE_H
#define _MOUSE_H

#include "ProcyonCommon.h"
#include "KeyCodes.h"

namespace Procyon {

	class IWindow;

	struct MouseState
	{
		bool btns[ PROCYON_MOUSE_BUTTON_COUNT ];
	};

	class Mouse
	{
	public:
		static void Reset();
		static void Poll( bool hasFocus );
		static bool	IsButtonUp( ProcyonMouseButton key );
		static bool	IsButtonDown( ProcyonMouseButton key );
		static bool	OnButtonUp( ProcyonMouseButton key);
		static bool	OnButtonDown( ProcyonMouseButton key );

	    static glm::ivec2 GetPosition();
	    static glm::ivec2 GetPosition( const IWindow* relative );
	    static void SetPosition( const glm::ivec2& position );
	    static void SetPosition( const glm::ivec2& position, const IWindow* relative );

	protected:
		static MouseState sCurrentMouseState;
		static MouseState sPrevMouseState;
	};
} /* namespace Procyon */

#endif /* _MOUSE_H */
