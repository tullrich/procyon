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
#include "Shape.h"
#include "GLGeometry.h"
#include "RenderCore.h"

using namespace Procyon::GL;

namespace Procyon {

    Shape::Shape( GLGeometry* geo )
        : mGeometry( geo )
        , mColor( 0.0f, 0.0f, 0.0f, 1.0f )
    {
    }

    glm::vec4 Shape::GetColor() const
    {
        return mColor;
    }

    void Shape::SetColor( float r, float g, float b, float a )
    {
        SetColor( glm::vec4( r, g, b, a ) );
    }

    void Shape::SetColor( const glm::vec4& color )
    {
        mColor = color;
    }

    void Shape::SetAlpha( float alpha )
    {
        mColor.w = alpha;
    }

    float Shape::GetAlpha() const
    {
        return mColor.w;
    }

    const GLGeometry* Shape::GetGeometry() const
    {
        return mGeometry;
    }

    void Shape::Render( const GL::GLProgram* program ) const
    {
        mGeometry->Bind( program );
        mGeometry->Draw();
    }

    void Shape::PostRenderCommands(  Renderer* r, RenderCore* rc ) const
    {
        glm::mat3 tform = GetTransform();
        glm::vec2 quaddata[] =
        {
            glm::vec2( tform * glm::vec3( 0.0f, 1.0f, 1.0f ) ), // v0
            glm::vec2( tform * glm::vec3( 1.0f, 1.0f, 1.0f ) ), // v1
            glm::vec2( tform * glm::vec3( 0.0f, 0.0f, 1.0f ) ), // v2
            glm::vec2( tform * glm::vec3( 1.0f, 0.0f, 1.0f ) ), // v3
        };

        // v0, v1, v2
        // v2, v1, v3

        RenderCommand cmd;
        cmd.op               = RENDER_OP_PRIMITIVE;
        cmd.program          = NULL;
        cmd.primmode         = PRIMITIVE_QUAD;
        cmd.verts            = (PrimitiveVertex*) &quaddata[0];
        cmd.vertcount        = 4;
        cmd.flags            = RENDER_SCREEN_SPACE;
        cmd.color[0]         = mColor.x;
        cmd.color[1]         = mColor.y;
        cmd.color[2]         = mColor.z;
        cmd.color[3]         = mColor.w;
        rc->AddOrAppendCommand( cmd );
    }

} /* namespace Procyon */
