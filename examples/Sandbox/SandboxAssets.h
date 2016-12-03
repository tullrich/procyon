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

#include "FontFace.h"
#include "World.h"
#include "SoundBuffer.h"
#include "GLTexture.h"

using namespace Procyon::GL;
using namespace Procyon;

class SandboxAssets
{
public:
	static FontFace*	sMainFont;
	static Map* 		sMap;
    static IImage*      sWindowIcon;
    static GLTexture*   sPlayerTexture;
    static GLTexture*   sTileTexture;
    static GLTexture*   sTestTexture;
    static SoundBuffer* sJumpSound;

	static void Load();
	static void Destroy();
};

#endif /* _SANDBOX_ASSETS_H */
