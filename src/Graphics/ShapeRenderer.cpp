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
#include "ShapeRenderer.h"
#include "GLProgram.h"
#include "GLMaterial.h"
#include "Camera.h"
#include "Shape.h"

using namespace Procyon::GL;

namespace Procyon {

    ShapeRenderer::ShapeRenderer()
        : mProgram( NULL )
        , mMaterial( NULL )
    {
        mProgram = new GLProgram( "shape.vert", "shape.frag" );
        mMaterial = new GLMaterial( mProgram );
    }

    ShapeRenderer::~ShapeRenderer()
    {
        delete mMaterial;
        delete mProgram;
    }

    void ShapeRenderer::Render( const Camera2D* camera, const Shape* s )
    {
        mMaterial->SetUniform( "mvp", camera->GetViewProjection() * s->GetTransform() );
        mMaterial->SetUniform( "color", s->GetColor() );
        mMaterial->Bind();

        s->Render( mProgram );
    }

} /* namespace Procyon */
