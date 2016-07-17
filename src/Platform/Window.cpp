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

#include "Window.h"

#define KEY_CODE_STR_TABLE( typ ) typ##_name_table

#undef KEY_ENUM_BEGIN
#define KEY_ENUM_BEGIN(typ) const char* KEY_CODE_STR_TABLE( typ )[] = {

#undef KEY_ENUM
#define KEY_ENUM(nam) #nam

#undef KEY_ENUM_END
#define KEY_ENUM_END(typ) };

#undef _KEY_CODES_H
#include "KeyCodes.h"

namespace Procyon {

	WindowBase::WindowBase()
		: mListener( nullptr )
	{
	}

	void WindowBase::SetEventListener( IInputEventListener* listener )
	{
		mListener = listener;
	}

	IInputEventListener* WindowBase::GetEventListener()
	{
		return mListener;
	}

	const char* ProcyonKeyCodeToStr( ProcyonKeyCode kc )
	{
		if( (unsigned)kc >= PROCYON_KEY_CODE_COUNT )
			kc = KEY_UNKNOWN;

		return KEY_CODE_STR_TABLE( ProcyonKeyCode )[ (unsigned)kc ];
	}

} /* namespace Procyon */
