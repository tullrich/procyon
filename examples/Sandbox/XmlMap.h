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
#ifndef XML_MAP_H
#define XML_MAP_H

#include "ProcyonCommon.h"
#include "Collision/World.h"

namespace Procyon {

	class XmlMap : public Map
	{
	public:
		XmlMap( const std::string& filePath );

		const std::string& GetFilePath() const { return mFilePath; }

		bool Load();

		virtual TileId GetTile( int x, int y ) const;
		virtual const TileSet* GetTileSet() const { return &mTileSet; }
        virtual glm::ivec2 GetSize() const { return mSize; }

	protected:
		std::string             mFilePath;
		TileSet 	            mTileSet;
        glm::ivec2              mSize;
        std::vector< TileId >   mTiles;
	};

} /* namespace Procyon */

#endif /* XML_MAP_H */
