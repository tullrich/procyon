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
#ifndef _TRANSFORMABLE_H
#define _TRANSFORMABLE_H

#include "ProcyonCommon.h"

namespace Procyon {

	class Transformable
	{
	public:
							Transformable();

		glm::vec2			GetPosition() const;
	    void 				SetPosition( float x, float y );
	    void				SetPosition( const glm::vec2& pos );

	    void				Move( float deltax, float deltay );
	    void				Move( const glm::vec2& delta );

	    void				SetOrientation( float angle );
	    void				Rotate( float deltaAngle );

	    glm::vec2			GetScale() const;
    	float 				GetScaleX() const;
    	float 				GetScaleY() const;
	    void				SetScale( float width, float height );
	    void				SetScale( const glm::vec2& dim );

	    void 				SetOrigin( float x, float y );
	    void 				SetOrigin( const glm::vec2& origin );

	    virtual glm::mat3 	GetTransform() const;

	protected:
	    float       			mOrientation;
	    glm::vec2   			mPosition;
	    glm::vec2   			mOrigin;
	    glm::vec2   			mScale;
	};

} /* namespace Procyon */

#endif /* _TRANSFORMABLE_H */
