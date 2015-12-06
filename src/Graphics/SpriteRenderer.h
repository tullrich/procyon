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
#ifndef _SPRITE_RENDERER_H
#define _SPRITE_RENDERER_H

#include "ProcyonCommon.h"
#include "Quad.h"

namespace Procyon {

	class Camera2D;
	class Sprite;

	class SpriteRenderer
	{
		class SpriteRenderInstance
		{
		public:
			glm::mat3 	mViewProj;
			Sprite* 	mSprite;

			SpriteRenderInstance()
				: mViewProj( glm::mat3() )
				, mSprite( NULL )
			{
			}

			SpriteRenderInstance( const glm::mat3& vp, Sprite* sprite )
				: mViewProj( vp )
				, mSprite( sprite )
			{
			}
		};

	public:
		void BeginRender();
    	void RenderSprite( const Camera2D* camera, const Sprite* s );
		void EndRender();

	protected:
	    Quad                    				mQuad;
	    std::vector<SpriteRenderInstance>    	mRenderInstances;
	};

} /* namespace Procyon */

#endif /* _SPRITE_RENDERER_H */