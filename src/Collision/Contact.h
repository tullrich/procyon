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
#ifndef _CONTACT_H
#define _CONTACT_H

#include "ProcyonCommon.h"

namespace Procyon {

	struct Contact
	{
		glm::vec2 	normal;
		float 		distance;

		Contact()
			: distance( 0.0f )
		{
		}

		Contact( const glm::vec2& norm, float dist )
			: normal( norm )
			, distance( dist )
		{
		}
	};
	
} /* namespace Procyon */

#endif /* _CONTACT_H */
