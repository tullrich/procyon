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

#ifndef _PROCYON_COMMON_H
#define _PROCYON_COMMON_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <set>
#include <map>
#include <type_traits>
#include <chrono>
#include <iomanip>
#include <stack>

#include "Logging.h"

#define BIT( bit ) (0x01<<bit)

namespace Procyon {

	struct FrameTime
	{
	    float   dt;     // delta time
	    float   tsl;    // time since launch
	};

	struct Rect
	{
		glm::vec2 topleft; 		// top-left coords
		glm::vec2 dimensions; 	// width and height

		Rect()
			: dimensions( 1.0f )
		{
		}

		Rect( const glm::vec2& tl, const glm::vec2& dims )
			: topleft( tl )
			, dimensions( dims )
		{
		}

		Rect( float x, float y, float width, float height )
			: topleft( x, y )
			, dimensions( width, height )
		{
		}

		glm::mat3 GetTransform() const
		{
			return glm::scale(
				glm::translate( glm::mat3(), topleft ), glm::vec2( dimensions.x, dimensions.y) );
		}

		glm::vec2 	GetDimensions() const { return dimensions; }
		float 		GetWidth() const { return dimensions.x; }
		float 		GetHeight() const { return dimensions.y; }


		// Returns the top left corner of this Rect
		glm::vec2 	GetTopLeft() const { return topleft; }

		// Returns the bottom left corner of this Rect
		glm::vec2 	GetBottomLeft() const
		{
			return topleft + glm::vec2( 0.0f, -dimensions.y );
		}

		// Returns the top right corner of this Rect
		glm::vec2 	GetTopRight() const
		{
			return topleft + glm::vec2( dimensions.x, 0.0f );
		}

		glm::vec2 	GetBottomRight() const
		{
			return topleft + glm::vec2( dimensions.x, -dimensions.y );
		}

	};
} /* namespace Procyon */

#include "Aabb.h"

#endif /* _PROCYON_COMMON_H */
