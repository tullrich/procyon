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
#include "GLTexture.h"
#include "Renderer.h"
#include "Sprite.h"
#include "RectShape.h"
#include "Contact.h"

#define FOREACH_TILE_INDEX( tiles )  				\
	for ( int x = 0; x < WORLD_WIDTH; x++ )			\
		for ( int y = 0; y < WORLD_HEIGHT; y++ )	\

using namespace Procyon::GL;

namespace Procyon {

	TileDef TileDef::Empty = { "Empty", nullptr, false };

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

	World::World( const TileSet* tileset /*= nullptr*/ )
		: mTileSet( tileset )
	{
		FOREACH_TILE_INDEX( mTiles )
		{
			mTiles[ x ][ y ] = 0;
		}
	}

	void World::LoadMap( const Map* map )
	{
		mTileSet = map->GetTileSet();;
		FOREACH_TILE_INDEX( mTiles )
		{
			mTiles[ x ][ y ] = map->GetTile( x, y );
		}
	}

	void World::Render( Renderer *r )
	{
		if ( !mTileSet )
			return;

		FOREACH_TILE_INDEX( mTiles )
		{
			const TileDef& tt = mTileSet->GetTileDef( mTiles[ x ][ y ] );
			if ( tt.texture )
			{
				glm::vec2 pos = glm::vec2( (float)x , (float)y ) * (float)TILE_PIXEL_SIZE + HALF_TILE_SIZE;
				glm::vec2 dim( (float)TILE_PIXEL_SIZE );
				r->DrawTexture( tt.texture, pos, dim, 0 );
			}
		}
	}

	void World::TileIntersection( const Aabb& bounds, std::vector<glm::ivec2>& out )
	{
		glm::vec2 min = bounds.GetMin();
		int minX = min.x / TILE_PIXEL_SIZE;
		int minY = min.y / TILE_PIXEL_SIZE;

		glm::vec2 max = bounds.GetMax();
		int maxX = max.x / TILE_PIXEL_SIZE;
		int maxY = max.y / TILE_PIXEL_SIZE;

		if ( maxX < 0 || minX >= WORLD_WIDTH ||
			 maxY < 0 || minY >= WORLD_HEIGHT )
			return;

		maxX = glm::clamp( maxX, 0, WORLD_WIDTH - 1 );
		minY = glm::clamp( minY, 0, WORLD_HEIGHT - 1 );
		maxX = glm::clamp( maxX, 0, WORLD_WIDTH - 1 );
		maxY = glm::clamp( maxY, 0, WORLD_HEIGHT - 1 );


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
		if ( c.normal.x > 0.0f && gridCoords.x + 1 < WORLD_WIDTH )
		{
			if ( mTiles[ gridCoords.x + 1 ][ gridCoords.y ] )
				return true;
		}
		else if ( c.normal.x < 0.0f && gridCoords.x - 1 >= 0 )
		{
			if ( mTiles[ gridCoords.x - 1 ][ gridCoords.y ] )
				return true;
		}

		if ( c.normal.y > 0.0f && gridCoords.y + 1 < WORLD_HEIGHT )
		{
			if ( mTiles[ gridCoords.x ][ gridCoords.y + 1] )
				return true;
		}
		else if( c.normal.y < 0.0f && gridCoords.y - 1 >= 0 )
		{
			if ( mTiles[ gridCoords.x ][ gridCoords.y - 1 ] )
				return true;
		}

		return false;
	}

	void World::SetTile( const glm::ivec2& t, TileId id )
	{
		if ( t.x < 0 || t.x >= WORLD_WIDTH ||
			 t.y < 0 || t.y >= WORLD_HEIGHT )
			return;
		mTiles[ t.x ][ t.y ] = id;
	}

	TileId World::GetTile( const glm::ivec2& t ) const
	{
		return mTiles[ t.x ][ t.y ];
	}

	const TileDef& World::GetTileDef( const glm::ivec2& t ) const
	{
		return mTileSet->GetTileDef( mTiles[ t.x ][ t.y ] );
	}

	// Point in pixels
	const TileDef& World::PointToTileDef( const glm::vec2& point ) const
	{
		int xidx = point.x / TILE_PIXEL_SIZE;
		int yidx = point.y / TILE_PIXEL_SIZE;

		if ( xidx < 0 || xidx >= WORLD_WIDTH ||
			 yidx < 0 || yidx >= WORLD_HEIGHT )
			return TileDef::Empty;

		return GetTileDef( glm::vec2( xidx, yidx ) );
	}

} /* namespace Procyon */
