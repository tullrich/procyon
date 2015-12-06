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
#ifndef _CAMERA_H
#define _CAMERA_H

#include "ProcyonCommon.h"
#include "Transformable.h"

namespace Procyon {

	class Camera2D : public Transformable
	{
	public:
		Camera2D();

		void 		OrthographicProj( float left, float right, float bottom, float top );

		glm::mat3 	GetProjection() const;
		glm::mat3 	GetView() const;
		glm::mat3 	GetViewProjection() const;

		float 		GetZoom() const;
		void 		SetZoom( float zoom );
		void 		ZoomIn( float amount );
		void 		ZoomOut( float amount );

		float 		GetHeight() const;
		float 		GetWidth() const;

		// Get a rect covered by this camera in world space.
		Rect 		GetScreenRect() const;

		// Translate point from screen to world coordinates.
		glm::vec2   ScreenToWorld( const glm::vec2& view ) const;

		// Translate direction from screen to world coordinates (preserves magnitude).
		glm::vec2   ScreenDirToWorldDir( const glm::vec2& dir ) const;

	protected:
		float		mZoom;
		float		mHeight;
		float		mWidth;
		glm::mat3 	mProjection;
	};

} /* namespace Procyon */

#endif /* _CAMERA_H */
