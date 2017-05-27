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
#include "Grid.h"
#include "Renderer.h"

using namespace Procyon;

Grid::Grid( float gridSize )
	: mGridSize( gridSize )
	, mEnabled( true )
	, mMajorFrequency( 2 )
{
}

void Grid::SetGridSize( float gridSize )
{
	mGridSize = gridSize;
}

void Grid::SetMajorLineFrequency(int frequency)
{
	mMajorFrequency = frequency;
}

void Grid::Render( Renderer* r, const glm::ivec2& size )
{
	if ( !mEnabled )
		return;

	const float gridAdvance = mGridSize * ((r->GetCamera().GetZoom() < 0.75f) ? mMajorFrequency : 1.0f);

    Procyon::Rect viewport = r->GetCamera().GetScreenRect();
    Procyon::Rect map( 0.0f, mGridSize * (float)size.y, mGridSize * ( float )size.x, mGridSize * ( float )size.y );

    Procyon::Rect rect( glm::max( viewport.topleft.x, map.topleft.x )
        , glm::min( viewport.topleft.y, map.topleft.y )
        , glm::min( viewport.dimensions.x, map.dimensions.x )
        , glm::min( viewport.dimensions.y, map.dimensions.y ) );

	// Draw horizontal lines
	float yCursor = rect.GetBottomLeft().y - fmod( rect.GetBottomLeft().y, gridAdvance );
	while ( yCursor <= rect.GetTopLeft().y )
	{
		float alpha = (((int)(yCursor / mGridSize)) % mMajorFrequency) ? 0.15f : 0.3f;
		r->DrawWorldLine( glm::vec2( int(rect.GetTopLeft().x), int(yCursor) )
			, glm::vec2(int( rect.GetTopLeft().x + rect.GetWidth()), int(yCursor) )
			, glm::vec4( glm::vec3( 0.0f ), 0.3f )  );
		//r->DrawAALine( glm::vec2( rect.GetTopLeft().x, yCursor ) + glm::vec2( 0.5f, 0.5f )
		//	, glm::vec2( rect.GetTopLeft().x + rect.GetWidth(), yCursor ) + glm::vec2( 0.5f, 0.5f )
		//	, 1.2f, 0.7f
		//	, glm::vec4( glm::vec3( 0.0f ), 0.3f )  );
		yCursor += gridAdvance;
	}

	// Draw vertical lines
	float xCursor = rect.GetTopLeft().x - fmod( rect.GetTopLeft().x, gridAdvance );
	while ( xCursor <= rect.GetTopRight().x )
	{
		float alpha = (((int)(xCursor / mGridSize)) % mMajorFrequency) ? 0.15f : 0.3f;
		r->DrawWorldLine( glm::vec2( xCursor, rect.GetTopLeft().y )
			, glm::vec2( xCursor, rect.GetTopLeft().y - rect.GetHeight() )
			, glm::vec4( glm::vec3( 0.0f ), 0.3f  )  );

		//r->DrawAALine( glm::vec2( xCursor, rect.GetTopLeft().y ) + glm::vec2( 0.5f, 0.5f )
		//	, glm::vec2( xCursor, rect.GetTopLeft().y - rect.GetHeight() ) + glm::vec2( 0.5f, 0.5f )
		//	, 1.2f, 0.7f
		//	, glm::vec4( glm::vec3( 0.0f ), 0.3f )  );
		xCursor += gridAdvance;
	}
}
