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
	class Texture;
	class Camera2D;
	class IWindow;

	enum class PolyLineJoinMode
	{
		MITER,
		BEVEL,
		ROUND,
	};

	enum class PolyLineCapMode
	{
		BUTT,
		ROUND,
		SQUARE,
	};

	class Renderer
	{
	public:
	   						Renderer( IWindow* window );
	   						~Renderer();

		const Camera2D& 	PushCamera();
		const Camera2D& 	PushCamera( const Camera2D& camera );
		void 				PopCamera();
		const Camera2D& 	GetCamera();
		void 				ResetCameras( const Camera2D& camera = Camera2D() );

		void 				SetClearColor( const glm::vec4 color );
		const glm::vec4&	GetClearColor() const;

		void 				BeginRender();
		void 				Draw( const Renderable* r );
        void                DrawWireframeRect( const Rect& rect, const glm::vec4& color, bool screenSpace );
		void 				DrawLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color );
		void 				DrawAALine( const glm::vec2& start, const glm::vec2& end, float width, float feather, const glm::vec4& color );
		void 				DrawAnchor( const glm::vec2* points, const glm::vec4& color, float width, PolyLineJoinMode joinMode, float miterLimit );
		void 				DrawPolyLine( const std::vector< glm::vec2 >& points, const glm::vec4& color
								, float width, PolyLineJoinMode joinMode, PolyLineCapMode capMode, float miterLimit = 1.0f );
		void				DrawWorldLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color );
		void 				DrawTexture( const Texture* tex, const glm::vec2& pos, const glm::vec2& dim, float orient, Rect textureRect = Rect() );
        void                DrawFullscreenTexture( const Texture* tex );
		void 				DrawRectShape( const glm::vec2& pos, const glm::vec2& dim, float orient, const glm::vec4& color );
		void 				EndRender();


		const RenderCore* 	GetRenderCore() const;
		RenderCore* 		GetRenderCore();

   	protected:
		void 				Flush();

		std::stack< Camera2D > 	mCameras;
		IWindow* 				mWindow;
		glm::vec4				mClearColor;
		RenderCore* 			mRenderCore;
	};

	extern bool sDebugLines;
} /* namespace Procyon */

#endif /* _RENDERER_H */
