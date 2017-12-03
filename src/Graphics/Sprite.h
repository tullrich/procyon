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
#ifndef _SPRITE_H
#define _SPRITE_H

#include "ProcyonCommon.h"
#include "Transformable.h"
#include "Renderable.h"
#include "GraphicsPlatform.h"

namespace Procyon {

	class Sprite : public Transformable, public Material, public Renderable
	{
	public:
							Sprite( const Program* prog, const Texture* mainTex );
	    					Sprite( const Texture* tex );

	    virtual void 		Process( FrameTime t ) { };

   		void 				SetTextureRect( const Rect& texRect );
    	const Rect&  		GetTextureRect() const;
    	glm::mat3 			GetUVTransform() const;

    	virtual void 		PostRenderCommands( Renderer* r, RenderCore* rc ) const;
	protected:
		Program* 		    GetOrCreateDefaultProgram();

		static Program* 	sDefaultProgram;
	    const Texture* 	    mTexture;
	    Rect 				mTextureRect;
	};

} /* namespace Procyon */

#endif /* _SPRITE_H */
