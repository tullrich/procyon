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
#ifndef _RENDER_CONTEXT_H
#define _RENDER_CONTEXT_H

#include "ProcyonCommon.h"

namespace Procyon {

	class ShapeRenderer;
	class FontRenderer;
	class SpriteRenderer;
	class Shape;
	class Sprite;
	class FontFace;
	class Camera2D;
	class Renderable;

	class RenderContext
	{
	public:
				RenderContext();
				~RenderContext();

		void 	BeginRender();
		void 	Render( const Camera2D* camera, const Renderable* r );
		void 	RenderShape( const Camera2D* camera, const Shape* s );
		void 	RenderText( const Camera2D* camera, const FontFace* f, const char* buf );
		void 	RenderSprite( const Camera2D* camera, const Sprite* s );
		void 	EndRender();

	protected:
		ShapeRenderer* 	mShapeRenderer;
		FontRenderer* 	mFontRenderer;
		SpriteRenderer* mSpriteRenderer;
	};
	
} /* namespace Procyon */

#endif /* _RENDER_CONTEXT_H */
