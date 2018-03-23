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

#include "AnimatedSprite.h"
#include "Graphics/Texture.h"

using namespace Procyon;

AnimatedSprite::AnimatedSprite( Texture* texture )
    : Sprite( texture )
    , mLifespan( 0.0f )
	, mPlaying( false )
{
}

void AnimatedSprite::Play()
{
	mPlaying = true;
}

void AnimatedSprite::Stop()
{
	mPlaying = false;
	mLifespan = 0.0f;
}

void AnimatedSprite::Process( FrameTime t )
{
	glm::vec2 texDims = mTexture->GetDimensions();

	if ( mPlaying )
	{
	    mLifespan += t.dt;
	}

    const float frameTime = 0.1f;
    const int numColumns = 2;

    int atlasIdx    = (int)( mLifespan / frameTime ) % 4;
    int columnIdx   = atlasIdx % numColumns;
    int rowIdx      = atlasIdx / numColumns;
	SetTextureRect( Rect( columnIdx * 32.0f / texDims.x, rowIdx * 32.0f / texDims.y
		, 32.0f / texDims.x, 32.0f / texDims.y ) );
}
