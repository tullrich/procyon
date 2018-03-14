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
#include "Transformable.h"

namespace Procyon {

	Transformable::Transformable()
		: mOrientation( 0.0f )
        , mDimensions( 1.0f, 1.0f )
	{
	}

    glm::vec2 Transformable::GetPosition() const
    {
        return mPosition;
    }

	void Transformable::SetPosition( float x, float y )
	{
		mPosition = glm::vec2( x, y );
	}

    void Transformable::SetPosition( const glm::vec2& pos )
    {
    	mPosition = pos;
    }

	void Transformable::Move( float deltax, float deltay )
	{
		mPosition += glm::vec2( deltax, deltay );
	}
	
	void Transformable::Move( const glm::vec2& delta )
	{
		mPosition += delta;
	}

	void Transformable::SetOrientation( float angle )
	{
		mOrientation = angle;
	}

	void Transformable::Rotate( float deltaAngle )
	{
		mOrientation += deltaAngle;
	}

    void Transformable::SetOrigin( float x, float y )
    {
        SetOrigin( glm::vec2( x, y ) );
    }

    void Transformable::SetOrigin( const glm::vec2& origin )
    {
        mOrigin = origin;
    }

	void Transformable::SetDimensions( float width, float height )
	{
        SetDimensions( glm::vec2( width, height ) );
	}

	void Transformable::SetDimensions( const glm::vec2& dim )
	{
		mDimensions = dim;
	}

    glm::vec2 Transformable::GetDimensions() const
    {
    	return mDimensions;
    }

    float Transformable::GetWidth() const
    {
    	return mDimensions.x;
    }

    float Transformable::GetHeight() const
    {
    	return mDimensions.y;
    }

    glm::mat3 Transformable::GetTransform() const
    {
        return glm::translate( glm::mat3(), mPosition ) * glm::rotate( glm::mat3(), mOrientation ) 
            * glm::scale( glm::mat3(), mDimensions )
            * glm::inverse( glm::translate( glm::mat3(), glm::vec2( mOrigin.x, -mOrigin.y ) ) );
        //return glm::translate( glm::rotate( glm::mat3(), mOrientation ), mPosition );
        //return glm::translate( glm::scale( glm::rotate( glm::translate( 
        //    glm::mat3(), mPosition ), mOrientation ), mDimensions ), mOrigin );
    }

} /* namespace Procyon */