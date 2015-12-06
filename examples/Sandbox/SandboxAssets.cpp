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
    static int tiles[WORLD_HEIGHT * WORLD_WIDTH];

    virtual TileType GetTileType( int x, int y ) const
    {
        return (TileType)tiles[ (WORLD_HEIGHT - 1 - y) * WORLD_WIDTH + x  ];
    }
};

int SandboxMap::tiles[] 
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


FontFace*       SandboxAssets::sMainFont        = NULL;
Map*            SandboxAssets::sMap             = NULL;
IImage*         SandboxAssets::sWindowIcon      = NULL;
GLTexture*      SandboxAssets::sPlayerTexture   = NULL;
GLTexture*      SandboxAssets::sTileTexture     = NULL;
SoundBuffer*    SandboxAssets::sJumpSound       = NULL;

void SandboxAssets::Load()
{
	sMainFont 		= CreateFontFace( "Economica-Regular.ttf" );
	sMap 			= new SandboxMap();
    sWindowIcon     = new FileImage( "tile.png" );
    sPlayerTexture  = new GLTexture( GL_TEXTURE_2D, "raccoon.jpg" );
    sTileTexture    = new GLTexture( GL_TEXTURE_2D, "tile.png" );
    sJumpSound      = new SoundBuffer( "jump.wav" );
}

void SandboxAssets::Destroy()
{
	delete sMainFont;
	delete sMap;
    delete sWindowIcon;
    delete sPlayerTexture;
    delete sTileTexture;
    delete sJumpSound;
}