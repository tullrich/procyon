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
#ifndef _RENDERER_H
#define _RENDERER_H

#include "ProcyonCommon.h"
#include "Camera.h"

namespace Procyon {

	class RenderCore;
	class Renderable;
	class Camera2D;
	namespace GL { class GLTexture; }

	class Renderer
	{
	public:
	   						Renderer();
	   						~Renderer();

		const Camera2D& 	PushCamera();
		const Camera2D& 	PushCamera( const Camera2D& camera );
		void 				PopCamera();
		const Camera2D& 	GetCamera();
		void 				ResetCameras( const Camera2D& camera = Camera2D() );

		void 				BeginRender();
		void 				Draw( const Renderable* r );
        void                DrawWireframeRect( const Rect& rect, const glm::vec4& color );
		void 				DrawLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color );
		void				DrawWorldLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color );
		void 				DrawTexture( const GL::GLTexture* tex, const glm::vec2& pos, const glm::vec2& dim, float orient, struct Rect textureRect = Rect() );
        void                DrawFullscreenTexture( const GL::GLTexture* tex );
		void 				EndRender();


		const RenderCore* 	GetRenderCore() const;
		RenderCore* 		GetRenderCore();

   	protected:
		void 				Flush();

   		typedef std::stack<Camera2D> CameraStack;
   		CameraStack 		mCameras;
		RenderCore* 		mRenderCore;
	};

} /* namespace Procyon */

#endif /* _RENDERER_H */
