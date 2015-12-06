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
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Camera.h"
#include "GLMaterial.h"

using namespace Procyon::GL;

namespace Procyon {

/*    void SpriteRenderer::RemoveSprite( Sprite* s )
    {
        auto lambda = [s]( const Sprite* val ) { return val == s; };
        mSprites.erase( std::remove_if( mSprites.begin(), mSprites.end(), lambda )
            , mSprites.end() );
    }*/

    void SpriteRenderer::BeginRender()
    {
        mRenderInstances.clear();
    }

    void SpriteRenderer::RenderSprite( const Camera2D* camera, const Sprite* s )
    {
        mRenderInstances.emplace_back( camera->GetViewProjection(), const_cast<Sprite*>( s ) );
    }

	void SpriteRenderer::EndRender()
    {
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glEnable( GL_BLEND );

        for ( auto instance : mRenderInstances )
        {
            Sprite* s = instance.mSprite;

            s->SetUniform( "uv_mvp", s->GetUVTransform() );
            s->SetUniform( "mvp", instance.mViewProj * s->GetTransform() );
            s->Bind();

            mQuad.Bind( s->GetProgram() );

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        }

        mQuad.Unbind();
    }

} /* namespace Procyon */
