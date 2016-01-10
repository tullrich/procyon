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
#ifndef _CUSTOM_SPRITES_H
#define _CUSTOM_SPRITES_H

/*
================
CustomSprites.h

A few test implementations of sprites with custom/complex materials.
================
*/
class SpecialSprite : public Sprite
{
public:
    SpecialSprite( const GLTexture* tex1, const GLTexture* tex2 )
        : Sprite( GetOrCreateSpecialSpriteProgram(), tex1 )
        , mLifespan( 0.0f )
    {
        SetSampler( "tex2", tex2 );
        SetTextureRect( Rect( glm::vec2(), glm::vec2( 0.5f ) ) );
    }

    GLProgram* GetOrCreateSpecialSpriteProgram()
    {
        static GLProgram* program = NULL;
        if ( !program )
        {
            program = new GLProgram( "shaders/shader.vert", "shaders/shader2.frag" );
        }
        return program;
    }

    virtual void Process( FrameTime t )
    {
        mLifespan += t.dt;
        float interp = ( (float)sin( mLifespan * 4.0f ) + 1.0f ) / 2.0f ;
        SetUniform( "triangleColor", interp );
    }

protected:
    float mLifespan;
};

#endif /* _CUSTOM_SPRITES_H */
