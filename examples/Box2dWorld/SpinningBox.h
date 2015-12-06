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
#ifndef _SPINNING_BOX_H
#define _SPINNING_BOX_H

#include "Sprite.h"
#include "GLTexture.h"

using namespace Procyon::GL;
using namespace Procyon;

class SpinningBox : public Sprite
{
public:
    SpinningBox()
        : Sprite( GetOrCreateBoxTexture() )
        , mLifespan( 0.0f )
    {
    }

    GLTexture* GetOrCreateBoxTexture()
    {
        if ( !sTexture )
        {
            sTexture = new GLTexture( GL_TEXTURE_2D, "spinningbox.png" );
        }
        return sTexture;
    }

    virtual void Process( FrameTime t )
    {
        mLifespan += t.dt;

        glm::vec2 texDims = sTexture->GetDimensions();

        const float texWidth = 256.0f;
        const float texHeight = 128.0f;
        const float spriteSide = 50.0f;
        const float frameTime = 0.05f;
        const int numColumns = 5;

        int atlasIdx    = (int)( mLifespan / frameTime ) % 10;
        int columnIdx   = atlasIdx % numColumns;
        int rowIdx      = atlasIdx / numColumns;

        SetTextureRect( Rect( columnIdx * 50.0f / texDims.x, rowIdx * 50.0f / texDims.y
            , spriteSide / texDims.x, spriteSide / texDims.y ) );
    }
protected:

    static GLTexture*   sTexture;
    float               mLifespan;
};

GLTexture* SpinningBox::sTexture = NULL;

#endif /* _SPINNING_BOX_H */
