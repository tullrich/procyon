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
#ifndef _LINE_SHAPE_H
#define _LINE_SHAPE_H

#include "ProcyonCommon.h"
#include "Shape.h"

namespace Procyon {

	namespace GL {
	
		class GLGeometry;
		class GLBuffer;
		typedef std::shared_ptr<GLBuffer> GLBufferPtr;
	}
	
	class LineShape : public Shape
	{
	public:
	    						LineShape( const glm::vec2& start, const glm::vec2& end );

	    void 					SetStart( const glm::vec2& start );
	    void 					SetEnd( const glm::vec2& end );

    	virtual void 			Render( const GL::GLProgram* program ) const;
	    
	protected:

	    glm::vec2 			mStart;
	    glm::vec2 			mEnd;
	    GL::GLGeometry* 	mGeometry;
	    GL::GLBufferPtr  	mBuffer;
	};
	
} /* namespace Procyon */

#endif /* _LINE_SHAPE_H */
