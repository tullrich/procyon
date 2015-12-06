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
#ifndef _AABB_H
#define _AABB_H

namespace Procyon {
	
	class Aabb
	{
	public:
		Aabb( const Aabb& first, const Aabb& second )
			: mCenter( first.mCenter )
			, mHalfExtent( first.mHalfExtent )
		{
			Contain( second );
		}

		Aabb( const glm::vec2& center, const glm::vec2& halfExtent )
			: mCenter( center )
			, mHalfExtent( halfExtent )
		{
		}

		Aabb( float centerX, float centerY, float halfExtentX, float halfExtentY )
			: mCenter( centerX, centerY )
			, mHalfExtent( halfExtentX, halfExtentY )
		{
		}

		glm::vec2 GetMax() const
		{
			return mCenter + mHalfExtent;
		}

		glm::vec2 GetMin() const
		{
			return mCenter - mHalfExtent;
		}

		void Contain( const glm::vec2& point )
		{
			glm::vec2 max = glm::max( GetMax(), point );
			glm::vec2 min = glm::min( GetMin(), point );
			SetMinMax( min, max );
		}

		void Contain( const Aabb& box )
		{
			glm::vec2 max = glm::max( GetMax(), box.GetMax() );
			glm::vec2 min = glm::min( GetMin(), box.GetMin() );
			SetMinMax( min, max );
		}

		void SetMinMax( const glm::vec2& min, const glm::vec2& max )
		{
			mHalfExtent = ( max - min ) / 2.0f;
			mCenter = min + mHalfExtent;
		}

		void SetCenter( const glm::vec2& center )
		{
			mCenter = center;
		}

		void SetHalfExtent( const glm::vec2& halfExtent )
		{
			mHalfExtent = halfExtent;
		}

		static Aabb FromMinMax( const glm::vec2& min, const glm::vec2& max )
		{
			glm::vec2 halfExtent = ( max - min ) / 2.0f;
			return Aabb( min + halfExtent, halfExtent );
		}

		glm::vec2 mCenter;
		glm::vec2 mHalfExtent;
	};
	
} /* namespace Procyon */

#endif /* _AABB_H */
