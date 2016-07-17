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

class SandboxMap : public Map
{
    static TileId sTiles[WORLD_HEIGHT * WORLD_WIDTH];
	static TileSet sTileSet;

    virtual TileId GetTile( int x, int y ) const
    {
        return sTiles[ (WORLD_HEIGHT - 1 - y) * WORLD_WIDTH + x  ];
    }

	virtual const TileSet* GetTileSet() const { return &sTileSet; }
};

TileId SandboxMap::sTiles[]
    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 2, 2, 2, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 2, 2, 2, 2, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

TileSet SandboxMap::sTileSet;

FontFace*       SandboxAssets::sMainFont        = NULL;
Map*            SandboxAssets::sMap             = NULL;
IImage*         SandboxAssets::sWindowIcon      = NULL;
GLTexture*      SandboxAssets::sPlayerTexture   = NULL;
GLTexture*      SandboxAssets::sTileTexture     = NULL;
GLTexture*      SandboxAssets::sTestTexture     = NULL;
SoundBuffer*    SandboxAssets::sJumpSound       = NULL;

void SandboxAssets::Load()
{
	sMainFont 		= CreateFontFace( "fonts/arial.ttf" );
	//sMainFont 		= CreateFontFace( "fonts/Economica-Regular.ttf" );
	sMap 			= new SandboxMap();
    sWindowIcon     = new FileImage( "sprites/tile.png" );
    sPlayerTexture  = new GLTexture( GL_TEXTURE_2D, "sprites/raccoon.jpg" );
    sTileTexture    = new GLTexture( GL_TEXTURE_2D, "sprites/tile.png" );
    sTestTexture    = new GLTexture( GL_TEXTURE_2D, "tinyTest.png" );
    sJumpSound      = new SoundBuffer( "audio/jump.wav" );
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