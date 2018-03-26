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
#ifndef _GL_PROGRAM_H
#define _GL_PROGRAM_H

#include "ProcyonGL.h"

namespace Procyon {

namespace GL { 

	class GLShader;

	class GLProgram
	{
	public:
		GLProgram();
		GLProgram( const std::string& vertFilename, const std::string& fragFileName );
		GLProgram( const std::string& vertFilename, const std::string& fragFileName, const std::vector< std::string >& defines );
		~GLProgram();

		void 	Link( GLShader* shader[], int count );
		void 	Bind() const;

		GLuint 	GetUniformLocation( const std::string& str ) const;
		GLuint 	GetAttributeLocation( const std::string& str ) const;

		GLuint mProgramId;
	};

} /* namespace GL */

} /* namespace Procyon */

#endif /* _GL_PROGRAM_H */