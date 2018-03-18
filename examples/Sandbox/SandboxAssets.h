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
#ifndef _SANDBOX_ASSETS_H
#define _SANDBOX_ASSETS_H

#include "Graphics/FontFace.h"
#include "Graphics/Texture.h"
#include "Collision/World.h"
#include "Audio/SoundBuffer.h"

class SandboxAssets
{
public:
	static Procyon::FontFace*		sMainFont;
	static Procyon::Map* 			sMap;
    static Procyon::IImage*			sWindowIcon;
    static Procyon::Texture*		sPlayerTexture;
    static Procyon::Texture*		sTileTexture;
    static Procyon::Texture*		sTestTexture;
    static Procyon::SoundBuffer*	sJumpSound;

	static void Load();
	static void Destroy();
};

#endif /* _SANDBOX_ASSETS_H */
