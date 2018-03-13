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
#include "SandboxAssets.h"

#define WORLD_HEIGHT 10
#define WORLD_WIDTH 10

class SandboxMap : public Map
{
public:
    static TileId sTiles[ WORLD_HEIGHT * WORLD_WIDTH ];
	static TileSet sTileSet;

    virtual TileId GetTile( int x, int y ) const
    {
        return sTiles[ (WORLD_HEIGHT - 1 - y) * WORLD_WIDTH + x  ];
    }

	virtual const TileSet* GetTileSet() const { return &sTileSet; }
    virtual glm::ivec2 GetSize() const { return glm::ivec2( WORLD_WIDTH, WORLD_HEIGHT ); }
};

TileId SandboxMap::sTiles[]
    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

TileSet SandboxMap::sTileSet;

FontFace*       SandboxAssets::sMainFont        = NULL;
Map*            SandboxAssets::sMap             = NULL;
IImage*         SandboxAssets::sWindowIcon      = NULL;
Texture*		SandboxAssets::sPlayerTexture   = NULL;
Texture*		SandboxAssets::sTileTexture     = NULL;
Texture*		SandboxAssets::sTestTexture     = NULL;
SoundBuffer*    SandboxAssets::sJumpSound       = NULL;

void SandboxAssets::Load()
{
	sMainFont 		= new FontFace( "fonts/arial.ttf", 20);
	//sMainFont 	= new FontFace( "fonts/Economica-Regular.ttf", 20 );
    sWindowIcon     = new FileImage( "sprites/tile.png" );
    sPlayerTexture  = Texture::Allocate( "sprites/raccoon.jpg" );
    sTileTexture    = Texture::Allocate( "sprites/tile.png" );
    sTestTexture    = Texture::Allocate( "tinyTest.png" );
    sJumpSound      =  new SoundBuffer( "audio/jump4.wav" );

    TileDef def;
    def.filepath    = "sprites/tile.png";
    def.texture     = sTileTexture;
    def.collidable  = true;
    SandboxMap::sTileSet.AddTileDef( def );
    sMap = new SandboxMap();
}

void SandboxAssets::Destroy()
{
	delete sMainFont;
	delete sMap;
    delete sWindowIcon;
    delete sPlayerTexture;
    delete sTileTexture;
    delete sTestTexture;
    delete sJumpSound;
}
