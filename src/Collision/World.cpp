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
#include "World.h"
#include "Contact.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include "Graphics/Sprite.h"

#define TILE_INDEX( tx, ty ) ( ( tx ) * mSize.y + ( ty ) )

#define FOREACH_TILE_INDEX( tiles ) 			\
	for ( int x = 0; x < mSize.x; x++ )			\
		for ( int y = 0; y < mSize.y; y++ )		\

namespace Procyon {

	TileDef TileDef::Empty;

	TileId TileSet::AddTileDef( const TileDef& tile )
	{
		mTileDefs.emplace_back( tile );
		return TileId( mTileDefs.size() );
	}

	bool TileSet::RemoveTileDef( TileId id )
	{
		if ( id > 0 && id <= mTileDefs.size() )
		{
			mTileDefs.erase( mTileDefs.begin() + int( id - 1 ) );
			return true;
		}
		return false;
	}

	const TileDef& TileSet::GetTileDef( TileId id ) const
	{
		if ( id <= 0 )
		{
			return TileDef::Empty;
		}
		return mTileDefs[ id - 1 ];
	}

	void TileSet::Clear()
	{
		mTileDefs.clear();
	}

	void World::NewWorld( const glm::ivec2& size, const TileSet* tileset )
	{
		mTileSet = tileset;
		mSize = size;
		mTiles.clear();
		mTiles.resize( size.x * size.y, 0 ); // clear to empty
	}

	void World::LoadMap( const Map* map )
	{
		mTileSet = map->GetTileSet();
		mSize = map->GetSize();
		mTiles.clear();
		mTiles.resize( mSize.x * mSize.y, 0 ); // clear to empty
		FOREACH_TILE_INDEX( mTiles )
		{
			mTiles[ TILE_INDEX( x, y) ] = map->GetTile( x, y );
		}
	}

	void World::Render( Renderer *r )
	{
		if ( !mTileSet )
			return;

		FOREACH_TILE_INDEX( mTiles )
		{
			const TileDef& tt = mTileSet->GetTileDef( mTiles[ TILE_INDEX( x, y ) ] );
			if ( tt.texture )
			{
				glm::vec2 pos = glm::vec2( (float)x , (float)y ) * (float)TILE_PIXEL_SIZE + HALF_TILE_SIZE;
				glm::vec2 dim( (float)TILE_PIXEL_SIZE );
				r->DrawRectShape(pos, dim, 0.0f, glm::vec4(92/255.0f, 172/255.0f, 144/255.0f, 1.f));
				//r->DrawTexture( tt.texture, pos, dim, 0 );
			}
		}
	}

	void World::TileIntersection( const Aabb& bounds, std::vector< glm::ivec2 >& out )
	{
		glm::vec2 min = bounds.GetMin();
		int minX = int( min.x ) / TILE_PIXEL_SIZE;
		int minY = int( min.y ) / TILE_PIXEL_SIZE;

		glm::vec2 max = bounds.GetMax();
		int maxX = int( max.x ) / TILE_PIXEL_SIZE;
		int maxY = int( max.y ) / TILE_PIXEL_SIZE;

		if ( maxX < 0 || minX >= mSize.x ||
			 maxY < 0 || minY >= mSize.y )
			return;

		maxX = glm::clamp( maxX, 0, mSize.x - 1 );
		minY = glm::clamp( minY, 0, mSize.y - 1 );
		maxX = glm::clamp( maxX, 0, mSize.x - 1 );
		maxY = glm::clamp( maxY, 0, mSize.y - 1 );

		PROCYON_DEBUG( "World", "IsInternalCollision <%i, %i> max <%i, %i>", minX, minY, maxX, maxY );

		for ( int x = minX; x <= maxX; x++ )
		{
			for ( int y = minY; y <= maxY; y++ )
			{
				if( GetTileDef( glm::vec2( x, y ) ).collidable )
				{
					out.push_back( glm::ivec2( x, y ) );
				}
			}
		}
	}

	bool World::IsInternalCollision( const glm::ivec2& gridCoords, const Contact& c )
	{
		if ( c.normal.x > 0.0f && gridCoords.x + 1 < mSize.x )
		{
			if ( mTiles[ TILE_INDEX( gridCoords.x + 1, gridCoords.y ) ] )
				return true;
		}
		else if ( c.normal.x < 0.0f && gridCoords.x - 1 >= 0 )
		{
			if ( mTiles[ TILE_INDEX( gridCoords.x - 1, gridCoords.y ) ] )
				return true;
		}

		if ( c.normal.y > 0.0f && gridCoords.y + 1 < mSize.y )
		{
			if ( mTiles[ TILE_INDEX( gridCoords.x, gridCoords.y + 1 ) ] )
				return true;
		}
		else if( c.normal.y < 0.0f && gridCoords.y - 1 >= 0 )
		{
			if ( mTiles[ TILE_INDEX( gridCoords.x, gridCoords.y - 1 ) ] )
				return true;
		}

		return false;
	}

	void World::SetTile( const glm::ivec2& t, TileId id )
	{
		if ( t.x < 0 || t.x >= mSize.x ||
			 t.y < 0 || t.y >= mSize.y )
			return;
		mTiles[ TILE_INDEX( t.x, t.y ) ] = id;
	}

	TileId World::GetTile( const glm::ivec2& t ) const
	{
		return mTiles[ TILE_INDEX( t.x, t.y ) ];
	}

	const TileDef& World::GetTileDef( const glm::ivec2& t ) const
	{
		return mTileSet->GetTileDef( mTiles[ TILE_INDEX( t.x, t.y ) ] );
	}

	// Point in pixels
	const TileDef& World::PointToTileDef( const glm::vec2& point ) const
	{
		int xidx = int( point.x ) / TILE_PIXEL_SIZE;
		int yidx = int( point.y ) / TILE_PIXEL_SIZE;

		if ( xidx < 0 || xidx >= mSize.x ||
			 yidx < 0 || yidx >= mSize.y )
			return TileDef::Empty;

		return GetTileDef( glm::vec2( xidx, yidx ) );
	}

	bool World::InBounds( const glm::ivec2& t )
	{
		return t.x >= 0 && t.x < mSize.x && t.y >= 0 && t.y < mSize.y;
	}

} /* namespace Procyon */
