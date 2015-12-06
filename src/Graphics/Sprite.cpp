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
#include "Sprite.h"
#include "GLProgram.h"
#include "GLTexture.h"
#include "RenderContext.h"
#include "RenderCore.h"

using namespace Procyon::GL;

namespace Procyon {

    GLProgram* Sprite::sDefaultProgram = NULL;

    Sprite::Sprite( const GLProgram* prog, const GLTexture* mainTex )
        : GLMaterial( prog )
        , mTexture( mainTex )
        , mTextureRect( glm::vec2(), glm::vec2( 1.0f ) )
    {
        SetSampler( "tex", mTexture );
    }

	Sprite::Sprite( const GLTexture* tex )
		: GLMaterial( GetOrCreateDefaultProgram() )
        , mTexture( tex )
        , mTextureRect( glm::vec2(), glm::vec2( 1.0f ) )
	{
        SetSampler( "tex", mTexture );
	}

    GLProgram* Sprite::GetOrCreateDefaultProgram()
    {
        if ( !sDefaultProgram )
        {
            sDefaultProgram = new GLProgram( "shader.vert", "shader.frag" );
        }
        return sDefaultProgram;
    }

    void Sprite::SetTextureRect( const Rect& texRect )
    {
        mTextureRect = texRect;
    }

    const Rect& Sprite::GetTextureRect() const
    {
        return mTextureRect;
    }

    glm::mat3 Sprite::GetUVTransform() const
    {
        return mTextureRect.GetTransform();
    }

    void Sprite::Render( const Camera2D* camera, RenderContext* rc ) const
    {
        rc->RenderSprite( camera, this );
    }

    void Sprite::PostRenderCommands( RenderCore* rc ) const
    {
        BatchedQuad quaddata;
        quaddata.position[0] = mPosition.x;
        quaddata.position[1] = mPosition.y;
        quaddata.size[0]     = mDimensions.x;
        quaddata.size[1]     = mDimensions.y;
        quaddata.rotation    = mOrientation;
        quaddata.uvoffset[0] = mTextureRect.topleft.x;
        quaddata.uvoffset[1] = mTextureRect.topleft.y;
        quaddata.uvsize[0]   = mTextureRect.dimensions.x;
        quaddata.uvsize[1]   = mTextureRect.dimensions.y;
        quaddata.tint[0]     = 0.0f;
        quaddata.tint[1]     = 0.0f;
        quaddata.tint[2]     = 0.0f;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.program          = NULL;
        cmd.texture          = mTexture;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        rc->AddOrAppendCommand( cmd );
    }

} /* namespace Procyon */
