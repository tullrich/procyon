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
#ifndef _PROCYON_VASER_H
#define _PROCYON_VASER_H

#include "ProcyonGL.h"

struct VaseVec2 { double x,y;};
struct VaseColor { float r,g,b,a;};

inline VaseVec2 GlmToVaseVec2( const glm::vec2& v )
{
	VaseVec2 ret;
	ret.x = (double)v.x;
	ret.y = (double)v.y;
	return ret;
}

inline VaseColor GlmToVaseColor( const glm::vec4& v )
{
	VaseColor ret;
	ret.r = v.r;
	ret.g = v.g;
	ret.b = v.b;
	ret.a = v.a;
	return ret;
}

typedef VaseVec2 Vec2;
typedef VaseColor Color;

#include "vaser.h"

#endif /* _PROCYON_VASER_H */
