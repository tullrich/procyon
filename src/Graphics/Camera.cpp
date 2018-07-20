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
#include "Camera.h"

namespace Procyon {

	Camera2D::Camera2D()
		: mZoom( 1.0f )
		, mHeight( 1.0f )
		, mWidth( 1.0f )
	{
	}

	void Camera2D::OrthographicProj( float left, float right, float bottom, float top )
	{
	    float h    = right - left;
	    float v    = top - bottom;

	    mHeight = abs( v );
	    mWidth 	= abs( h );

	    mProjection =
	     	glm::mat3( 2.0f / h  				, 0.0f      				, 0.0f
                    ,  0.0f      				, 2.0f / v  				, 0.0f
                    ,  -( right + left ) / h    , -( top + bottom ) / v     , 1.0f    );
	}

	glm::mat3 Camera2D::GetProjection() const
	{
		return mProjection;
	}

	glm::mat3 Camera2D::GetView() const
	{
		glm::mat3 view = glm::scale( GetTransform(), glm::vec2( 1.0f / mZoom ) );
		return glm::inverse( view );
	}

	glm::mat3 Camera2D::GetViewProjection() const
	{
		return mProjection * GetView();
	}

	float Camera2D::GetZoom() const
	{
		return mZoom;
	}

	void Camera2D::SetZoom( float zoom )
	{
		mZoom = zoom;
	}

	void Camera2D::ZoomIn( float amount )
	{
		mZoom -= amount;
	}

	void Camera2D::ZoomOut( float amount )
	{
		mZoom += amount;
	}

	glm::vec2 Camera2D::ScreenToWorld( const glm::vec2& view ) const
	{
		return glm::vec2( glm::inverse( GetViewProjection() ) * glm::vec3( view, 1.0f ) );
	}

	glm::vec2 Camera2D::ScreenDirToWorldDir( const glm::vec2& dir ) const
	{
		return glm::vec2( glm::inverse( GetViewProjection() ) * glm::vec3( dir, 0.0f ) );
	}

	float Camera2D::GetHeight() const
	{
		return mHeight;
	}

	float Camera2D::GetWidth() const
	{
		return mWidth;
	}

	Rect Camera2D::GetScreenRect() const
	{
		// Grab inverse view projection
		glm::mat3 invVp = glm::inverse( GetViewProjection() );

		// Compute rect dimensions
		glm::vec2 screenTopleft = glm::vec2( invVp * glm::vec3( -1.0f, 1.0f, 1.0f ) );
		glm::vec2 screenDims = glm::vec2( invVp * glm::vec3( 2.0f, 2.0f, 0.0f ) );

		return Rect( screenTopleft, screenDims );
	}

/*	void Camera3D::PerspectiveProj( float near, float far, float fov, float aspect )
	{
	    //float h    = right - left;
	    //float v    = top - bottom;
	    float d    = far - near;
   		float S = 1 / ( tan( fov * 0.5f * M_PI / 180 ) );

	    mProjection =
			glm::mat4(  S       , 0.0f  , 0.0f 				    , 0.0f
		              , 0.0f    , S 	, 0.0f 					, 0.0f
		              , 0.0f  	, 0.0f  , -( far + near) / d 	, -1.0f
		              , 0.0f    , 0.0f  , -2.0f * far * near / d, 0.0f   );

// return glm::mat4(   2.0f * near / h       , 0.0f                  , 0.0f, 0.0f
//                 , 0.0f                  , 2.0f * near / v       , 0.0f, 0.0f
//                 , ( right + left ) / h  , ( top + bottom ) / v  , -( far + near) / d, -1.0f
//                 , 0.0f                  , 0.0f                  , -2.0f * far * near / d, 0.0f
//             );
	}*/

} /* namespace Procyon */
