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
#include "RenderCore.h"

namespace Procyon {

    Shape::Shape( )
        : mColor( 0.0f, 0.0f, 0.0f, 1.0f )
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

	void Shape::PostRenderCommands(  Renderer* r, RenderCore* rc ) const
	{
        BatchedQuad quaddata;
        quaddata.position[0] = mPosition.x;
        quaddata.position[1] = mPosition.y;
        quaddata.size[0]     = mScale.x;
        quaddata.size[1]     = mScale.y;
        quaddata.rotation    = mOrientation;
        quaddata.uvoffset[0] = 0.0f;
        quaddata.uvoffset[1] = 0.0f;
        quaddata.uvsize[0]   = 0.0f;
        quaddata.uvsize[1]   = 0.0f;
        quaddata.color[0]    = mColor.r;
        quaddata.color[1]    = mColor.g;
        quaddata.color[2]    = mColor.b;
		quaddata.color[3]    = mColor.w;
		quaddata.origin[0]	 = mOrigin.x;
		quaddata.origin[1]	 = mOrigin.y;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.flags            = RENDER_SCREEN_SPACE;
        cmd.texture          = NULL;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        rc->AddOrAppendCommand( cmd );
	}

} /* namespace Procyon */
