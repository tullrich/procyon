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
#include "FontRenderer.h"
#include "GLProgram.h"
#include "GLMaterial.h"
#include "FontFace.h"
#include "Camera.h"

using namespace Procyon::GL;

namespace Procyon {

	FontRenderer::FontRenderer()
	{
        mProgram = new GLProgram( "shaders/font.vert", "shaders/font.frag" );
        mMaterial = new GLMaterial( mProgram );
	}

    FontRenderer::~FontRenderer()
    {
        delete mMaterial;
        delete mProgram;
    }

	void FontRenderer::RenderText( const Camera2D& camera, const FontFace& font
	        , float x, float y, const std::string& string)
    {
/*        mMaterial->SetSampler( "tex", font.GetTexture() );

        glm::mat3 p = camera.GetProjection();

        for( auto c : string )
        {
            if ( c >= 32 && c < 128 )
            {
                PROCYON_DEBUG( "Font", "Char %c <%f, %f>", c, x, y );

                float offsetx = 0.0f, offsety = 0.0f;
                stbtt_aligned_quad q = font.GetGlyphTransform( c - 32, &offsetx, &offsety );

                glm::mat3 trans = glm::translate( glm::mat3(), glm::vec2( q.s0, q.t0 ) );
                mMaterial->SetUniform( "uv_mvp"
                    , glm::scale( trans, glm::vec2( q.s1 - q.s0, q.t1 - q.t0 ) ) );

                glm::mat3 var;
                var = glm::translate( glm::mat3(), glm::vec2( x + q.x0, y - q.y0) )
                * glm::scale( glm::mat3() , glm::vec2(  (q.x1 - q.x0), (q.y1 - q.y0) ) );

                mMaterial->SetUniform( "mvp", p * var );

                x += offsetx;
                y -= offsety;

                mMaterial->Bind();
                mQuad.Bind( mMaterial->GetProgram() );

                glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );
            }
        }*/
    }

} /* Procyon */
