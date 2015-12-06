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
#include "GLMaterial.h"
#include "Renderable.h"

namespace Procyon {

	namespace GL 
	{
		class GLProgram;
		class GLTexture;
	}

	class Sprite : public Transformable, public GL::GLMaterial, public Renderable
	{
	public:
							Sprite( const GL::GLProgram* prog, const GL::GLTexture* mainTex );
	    					Sprite( const GL::GLTexture* tex );

	    virtual void 		Process( FrameTime t ) { };

   		void 				SetTextureRect( const Rect& texRect );
    	const Rect&  		GetTextureRect() const;
    	glm::mat3 			GetUVTransform() const;

    	virtual void 		Render( const Camera2D* camera, RenderContext* rc ) const;
    	virtual void 		PostRenderCommands( RenderCore* rc ) const;
	protected:
		GL::GLProgram* 		GetOrCreateDefaultProgram();

		static GL::GLProgram* 	sDefaultProgram;
	    const GL::GLTexture* 	mTexture;
	    Rect 					mTextureRect;				
	};

} /* namespace Procyon */

#endif /* _SPRITE_H */