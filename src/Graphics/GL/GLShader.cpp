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
#include "GLShader.h"

namespace Procyon {

namespace GL { 

GLShader::GLShader( GLenum shaderType )
{
	mShaderId = glCreateShader( shaderType );
}

GLShader::GLShader( GLenum shaderType, const std::string& filepath )
{
    mShaderId = glCreateShader( shaderType );
    
    // Create vertex shader
    std::string shaderSrc;
    if( !ReadShaderSource( filepath, shaderSrc ) )
        throw std::runtime_error("Unable to parse shader");

    PROCYON_DEBUG("GL", "Parsed ShaderSrc: %s ", shaderSrc.c_str() );

    LoadShaderSource( shaderSrc );
}

GLShader::~GLShader()
{
	glDeleteShader( mShaderId );
}

void GLShader::LoadShaderSource( const std::string& source )
{
	const char* cvsrc = source.c_str();

    glShaderSource( mShaderId, 1, &cvsrc, NULL );
    glCompileShader( mShaderId );

    if ( !CheckShaderStatus() )
		throw std::runtime_error( "CheckShaderStatus" );
}

bool GLShader::CheckShaderStatus()
{
    GLint status;
    glGetShaderiv( mShaderId, GL_COMPILE_STATUS, &status );

    if( status != GL_TRUE )
    {
        GLint logLen = 0;
        glGetShaderiv( mShaderId, GL_INFO_LOG_LENGTH, &logLen );

        if( logLen )
        {
            std::vector<char> buf( logLen );

            glGetShaderInfoLog( mShaderId, logLen, NULL, buf.data() );

            PROCYON_ERROR("GL", "Shader compililation failed! Info: %s ", buf.data() );
        }

        return false;
    }
    return true;
}

bool GLShader::ReadShaderSource( const std::string &path, std::string &out )
{
    std::ifstream file( path );
    if( file )
    {
        out.assign(std::istreambuf_iterator<char>( file ), 
            std::istreambuf_iterator<char>());
        return true;
    }
    return false;
}

} /* namespace GL */

} /* namespace Procyon */