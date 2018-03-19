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
#include "Mouse.h"
#include "PlatformInput.h"

#include <cstring>

namespace Procyon {

	/* static */ MouseState Mouse::sCurrentMouseState = { 0 };
	/* static */ MouseState Mouse::sPrevMouseState = { 0 };


	/* static */ void Mouse::Reset()
	{
		memset( &sCurrentMouseState, 0, sizeof( MouseState ) );
		memset( &sPrevMouseState, 0, sizeof( MouseState ) );
	}

	/* static */ void Mouse::Poll( bool hasFocus )
	{
		sPrevMouseState = sCurrentMouseState;
		PlatformInput::PollMouseState( &sCurrentMouseState );

		if ( !hasFocus )
		{
			memset( &sCurrentMouseState, 0, sizeof( MouseState ) );
		}
	}

	/* static */ bool Mouse::IsButtonUp( ProcyonMouseButton key )
	{
		return !sCurrentMouseState.btns[ int( key ) ];
	}

	/* static */ bool Mouse::IsButtonDown( ProcyonMouseButton key )
	{
		return sCurrentMouseState.btns[ int( key ) ];
	}

	/* static */ bool Mouse::OnButtonUp( ProcyonMouseButton key )
	{
		return !sCurrentMouseState.btns[ int( key ) ] && sPrevMouseState.btns[ int( key ) ];
	}

	/* static */ bool Mouse::OnButtonDown( ProcyonMouseButton key )
	{
		return sCurrentMouseState.btns[ int( key ) ] && !sPrevMouseState.btns[ int( key ) ];
	}

	/* static */ glm::ivec2 Mouse::GetPosition()
	{
		return PlatformInput::GetMousePosition();
	}

	/* static */ glm::ivec2 Mouse::GetPosition( const IWindow* relative )
	{
		return PlatformInput::GetMousePosition( relative );
	}

	/* static */ void Mouse::SetPosition( const glm::ivec2& position )
	{
		return PlatformInput::SetMousePosition( position );
	}

	/* static */ void Mouse::SetPosition( const glm::ivec2& position, const IWindow* relative )
	{
		return PlatformInput::SetMousePosition( position, relative );
	}

} /* namespace Procyon */
