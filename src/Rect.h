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

#ifndef _RECT_H
#define _RECT_H

namespace Procyon {

	template< typename VecT, typename T >
	struct TRect
	{
		VecT topleft; 		// top-left coords
		VecT dimensions; 	// width and height

		TRect()
			: dimensions( (T)1 )
		{
		}

		TRect( const VecT& tl, const VecT& dims )
			: topleft( tl )
			, dimensions( dims )
		{
		}

		TRect( T x, T y, T width, T height )
			: topleft( x, y )
			, dimensions( width, height )
		{
		}

		glm::mat3 GetTransform() const
		{
			return glm::scale( glm::translate( glm::mat3(), glm::vec2( topleft ) ), glm::vec2( dimensions ) );
		}

		VecT GetDimensions() const { return dimensions; }
		T GetWidth() const { return dimensions.x; }
		T GetHeight() const { return dimensions.y; }


		// Returns the top left corner of this Rect
		VecT GetTopLeft() const { return topleft; }

		// Returns the bottom left corner of this Rect
		VecT GetBottomLeft() const
		{
			return topleft + VecT( (T)0, -dimensions.y );
		}

		// Returns the top right corner of this Rect
		VecT GetTopRight() const
		{
			return topleft + VecT( dimensions.x, (T)0 );
		}

		VecT GetBottomRight() const
		{
			return topleft + VecT( dimensions.x, -dimensions.y );
		}

	};

	typedef TRect< glm::vec2, float > Rect;
	typedef TRect< glm::ivec2, uint32_t > IntRect;

} /* namespace Procyon */

#endif /* _RECT_H */
