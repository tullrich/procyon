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
#include "Texture.h"
#include "Renderer.h"
#include "RenderCore.h"

namespace Procyon {

	Sprite::Sprite()
		: mTexture( NULL )
		, mTextureRect( glm::ivec2( 0 ), glm::ivec2( 1 ) )
	{
	}

	Sprite::Sprite( const Texture* tex )
        : mTexture( tex )
		, mTextureRect( glm::ivec2( 0 ), glm::ivec2( 1 ) )
	{
		if ( mTexture )
		{
			SetTextureRect( IntRect( glm::ivec2( 0 ), mTexture->GetDimensions() ) );
		}
	}

    void Sprite::SetTextureRect( const IntRect& texRect )
    {
        mTextureRect = texRect;
    }

    const IntRect& Sprite::GetTextureRect() const
    {
        return mTextureRect;
    }

    glm::mat3 Sprite::GetUVTransform() const
    {
        return mTextureRect.GetTransform();
    }

	void Sprite::SetTexture( const Texture* tex )
	{
		mTexture = tex;
	}

    void Sprite::PostRenderCommands( Renderer* r, RenderCore* rc ) const
    {
        BatchedQuad quaddata;
        quaddata.position[0] = mPosition.x;
        quaddata.position[1] = mPosition.y;
        quaddata.size[0]     = mScale.x;
        quaddata.size[1]     = mScale.y;
        quaddata.rotation    = mOrientation;
        quaddata.uvoffset[0] = (float)mTextureRect.GetTopLeft().x;
        quaddata.uvoffset[1] = (float)mTextureRect.GetTopLeft().y;
        quaddata.uvsize[0]   = (float)mTextureRect.GetWidth();
        quaddata.uvsize[1]   = (float)mTextureRect.GetHeight();
        quaddata.tint[0]     = 0.0f;
        quaddata.tint[1]     = 0.0f;
        quaddata.tint[2]     = 0.0f;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.flags            = 0;
        cmd.texture          = mTexture;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        rc->AddOrAppendCommand( cmd );
    }

} /* namespace Procyon */
