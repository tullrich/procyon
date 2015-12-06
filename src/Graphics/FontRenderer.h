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
#ifndef _FONT_RENDERER_H
#define _FONT_RENDERER_H

#include "ProcyonCommon.h"
#include "Quad.h"

namespace Procyon {

	namespace GL {
		class GLProgram;
		class GLMaterial;
	}
	class Camera2D;
	class FontFace;

	class FontRenderer
	{
	public:
	    FontRenderer();
	    ~FontRenderer();

	    void RenderText( const Camera2D& camera, const FontFace& font
	        , float x, float y, const std::string& string);

	protected:
	    GL::GLProgram*          mProgram;
	    GL::GLMaterial*         mMaterial;
	    Quad                    mQuad;
	};

} /* Procyon */

#endif /* _FONT_RENDERER_H */
