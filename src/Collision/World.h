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
#ifndef _WORLD_H
#define _WORLD_H

#include "ProcyonCommon.h"

#define WORLD_HEIGHT 10
#define WORLD_WIDTH 10

#define PIXELS_PER_METER 32.0f
#define PixelsToMeters( pixels ) 						\
			(float)pixels / (float)PIXELS_PER_METER
#define MetersToPixels( meters )						\
			(float)meters * (float)PIXELS_PER_METER

#define TILE_PIXEL_SIZE 32
#define HALF_TILE_SIZE (((float)TILE_PIXEL_SIZE)/2.0f)
#define TILE_METER_SIZE 								\
			PixelsToMeters( TILE_PIXEL_SIZE )

#define POINT_TO_TILE(x,y) glm::ivec2(x / TILE_PIXEL_SIZE, y / TILE_PIXEL_SIZE)

namespace Procyon {

	class Renderable;
	class Renderer;
	class Camera2D;
	class World;
	class Contact;
	namespace GL { class GLTexture; }

	typedef int TileId;

	struct TileDef
	{
		std::string filepath;
		GL::GLTexture* texture;
		bool collidable;

		static TileDef Empty;
	};

	class TileSet
	{
	public:
		TileId AddTileDef( const TileDef& tile );
		bool RemoveTileDef( TileId id );
		const TileDef& GetTileDef( TileId id ) const;
		void Clear();
		int Size() const { return (int)mTileDefs.size() + 1; }
	protected:
		std::vector< TileDef > mTileDefs;
	};

	class Map
	{
	public:
		virtual TileId GetTile( int x, int y ) const = 0;
		virtual const TileSet* GetTileSet() const = 0;
	};

	class World
	{
	public:
							World( const TileSet* tileset = nullptr );

		void  				LoadMap( const Map* map );
		void 				Render( Renderer *r );

		void 				TileIntersection( const Aabb& bounds, std::vector<glm::ivec2>& out );
		bool 				IsInternalCollision( const glm::ivec2& gridCoords, const Contact& c );

		void 			 	SetTile( const glm::ivec2& t, TileId type );
		TileId	 			GetTile( const glm::ivec2& t ) const;
		const TileDef&	 	GetTileDef( const glm::ivec2& t ) const;
		const TileDef&		PointToTileDef( const glm::vec2& point ) const;

	protected:
		const TileSet* 	mTileSet;
		TileId	 		mTiles[ WORLD_WIDTH ][ WORLD_HEIGHT ];
	};

} /* namespace Procyon */

#endif /* _WORLD_H */