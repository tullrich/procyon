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
#include "GLProgram.h"
#include "GLShader.h"

namespace Procyon {

namespace GL {

GLProgram::GLProgram()
{
	mProgramId = glCreateProgram();
}

GLProgram::GLProgram( const std::string& vertFilename, const std::string& fragFileName )
{
	mProgramId = glCreateProgram();

    GLShader vertShader( GL_VERTEX_SHADER, vertFilename );
    GLShader fragShader( GL_FRAGMENT_SHADER, fragFileName );

    // link
    GLShader* shaders[] = { &vertShader, &fragShader };
    Link( shaders, 2 );
}

GLProgram::~GLProgram()
{
	glDeleteProgram( mProgramId );
}

void GLProgram::Link( GLShader* shader[], int count )
{
	for( int i = 0; i < count; i++)
	{
		glAttachShader( mProgramId, shader[i]->mShaderId );
	}

	glLinkProgram( mProgramId );
}

void GLProgram::Bind() const
{
    glUseProgram( mProgramId );
}


GLuint GLProgram::GetUniformLocation( const std::string& str ) const
{
	return glGetUniformLocation( mProgramId, str.c_str() );
}

GLuint GLProgram::GetAttributeLocation( const std::string& str ) const
{
	return glGetAttribLocation( mProgramId, str.c_str() );
}

} /* namespace GL */

} /* namespace Procyon */
