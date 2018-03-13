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
#ifndef _SHAPE_H
#define _SHAPE_H

#include "ProcyonCommon.h"
#include "Transformable.h"
#include "Renderable.h"

namespace Procyon {
	
	class Shape : public Transformable, public Renderable
	{
	public:
	    						Shape();

	    void 					SetColor( float r, float g, float b, float a );
	    void 					SetColor( const glm::vec4& color );
	    void 					SetAlpha( float alpha );

	    glm::vec4 				GetColor() const;
	    float 					GetAlpha() const;

		void 					PostRenderCommands( Renderer* r, RenderCore* rc ) const;

	protected:
	    glm::vec4       	mColor;
	};

} /* namespace Procyon */

#endif /* _SHAPE_H */
