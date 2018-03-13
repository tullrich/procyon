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

namespace Procyon {

	Sprite::Sprite( const Texture* tex )
        : mTexture( tex )
        , mTextureRect( glm::vec2(), glm::vec2( 1.0f ) )
	{
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

    void Sprite::PostRenderCommands( Renderer* r, RenderCore* rc ) const
    {
        r->DrawTexture( mTexture, mPosition, mDimensions, mOrientation, mTextureRect );
    }

} /* namespace Procyon */
