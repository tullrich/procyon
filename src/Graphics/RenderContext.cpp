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
#include "RenderContext.h"
#include "ShapeRenderer.h"
#include "FontRenderer.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Renderable.h"

namespace Procyon {

	RenderContext::RenderContext()
		: mShapeRenderer( new ShapeRenderer() )
		, mFontRenderer( new FontRenderer() )
		, mSpriteRenderer( new SpriteRenderer() )
	{
	}

	RenderContext::~RenderContext()
	{
		delete mShapeRenderer;
		delete mFontRenderer;
		delete mSpriteRenderer;
	}

	void RenderContext::BeginRender()
	{
        // Sprite rendering
        mSpriteRenderer->BeginRender();
	}

	void RenderContext::Render( const Camera2D* camera, const Renderable* r )
	{
		r->Render( camera, this );
	}

	void RenderContext::RenderShape( const Camera2D* camera, const Shape* s )
	{
		mShapeRenderer->Render( camera, s );
	}

	void RenderContext::RenderText( const Camera2D* camera, const FontFace* f, const char* buf )
	{
	    mFontRenderer->RenderText( *camera, *f, -camera->GetWidth() / 2.0f, camera->GetHeight() / 2.0f
        	- 25.0f, buf );
	}

	void RenderContext::RenderSprite( const Camera2D* camera, const Sprite* s )
	{
		mSpriteRenderer->RenderSprite( camera, s );
	}

	void RenderContext::EndRender()
	{
        mSpriteRenderer->EndRender();
	}

} /* namespace Procyon */
