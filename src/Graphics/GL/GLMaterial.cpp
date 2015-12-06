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
#include "GLMaterial.h"
#include "GLTexture.h"
#include "GLProgram.h"

namespace Procyon {

namespace GL {

	GLMaterial::GLMaterial( const GLProgram* program )
		: mProgram( program )
	{
	}

	const GLProgram* GLMaterial::GetProgram() const
	{
		return mProgram;
	}

	void GLMaterial::Bind() const
	{
		mProgram->Bind();

		int i = 0;
		for ( auto sampler : mSamplers )
		{
			if ( sampler.location != -1 )
			{
				glUniform1i( sampler.location, i );
				glActiveTexture( GL_TEXTURE0 +  i );
				sampler.texture->Bind();
				i++;
			}
		}

		for( auto pair : mUniforms )
		{
			UniformState& uniform = pair.second;

			if ( uniform.location != -1 )
			{
				switch ( uniform.type )
				{
				case UNIFORM_FLOAT:
					switch( uniform.components)
					{
					case 1:
						glUniform1fv( uniform.location, 1, &uniform.f[0] );
						break;
					case 2:
						glUniform2fv( uniform.location, 1, &uniform.f[0] );
						break;
					case 3:
						glUniform3fv( uniform.location, 1, &uniform.f[0] );
						break;
					case 4:
						glUniform4fv( uniform.location, 1, &uniform.f[0] );
						break;
					}
					break;
				case UNIFORM_INT:
					switch( uniform.components)
					{
					case 1:
						glUniform1iv( uniform.location, 1, &uniform.i[0] );
						break;
					case 2:
						glUniform2iv( uniform.location, 1, &uniform.i[0] );
						break;
					case 3:
						glUniform3iv( uniform.location, 1, &uniform.i[0] );
						break;
					case 4:
						glUniform4iv( uniform.location, 1, &uniform.i[0] );
						break;
					}
					break;
				case UNIFORM_UINT:
					switch( uniform.components)
					{
					case 1:
						glUniform1uiv( uniform.location, 1, &uniform.u[0] );
						break;
					case 2:
						glUniform2uiv( uniform.location, 1, &uniform.u[0] );
						break;
					case 3:
						glUniform3uiv( uniform.location, 1, &uniform.u[0] );
						break;
					case 4:
						glUniform4uiv( uniform.location, 1, &uniform.u[0] );
						break;
					}
					break;
				case UNIFORM_MAT_3_3:
					glUniformMatrix3fv( uniform.location, 1, false, &uniform.m3[0][0] );
					break;
				case UNIFORM_MAT_4_4:
					glUniformMatrix4fv( uniform.location, 1, false, &uniform.m4[0][0] );
					break;
				}
			}
		}
	}

	void GLMaterial::SetSampler( const std::string& name, const GLTexture* texture)
	{
		RemoveSampler( name );

		SamplerState sampler;
		sampler.name 		= name;
		sampler.texture 	= texture;
		sampler.location 	= mProgram->GetUniformLocation( name );
		mSamplers.push_back(sampler);
	}

	void GLMaterial::SetUniform( const std::string& name, float value )
	{
		SetUniform( name, glm::vec4( value, 0.0f, 0.0f, 0.0f ), 1);
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::vec2& value )
	{
		SetUniform( name, glm::vec4( value, 0.0f, 0.0f ), 2);
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::vec3& value )
	{
		SetUniform( name, glm::vec4( value, 0.0f ), 3 );
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::vec4& value
		, char components /*= 4 */ )
	{
		UniformState& uniform = mUniforms[ name ];
		uniform.type		= UNIFORM_FLOAT;
		uniform.components	= glm::clamp( components, (char)1, (char)4 );
		uniform.location 	= mProgram->GetUniformLocation( name );
		memcpy( &uniform.f[0], glm::value_ptr( value ), sizeof( glm::vec4 ) );
	}

	void GLMaterial::SetUniform( const std::string& name, int value )
	{
		SetUniform( name, glm::ivec4( value, 0, 0, 0 ), 1);
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::ivec2& value )
	{
		SetUniform( name, glm::ivec4( value, 0, 0 ), 2);
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::ivec3& value )
	{
		SetUniform( name, glm::ivec4( value, 0 ), 3 );
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::ivec4& value
		, char components /*= 4 */ )
	{
		UniformState& uniform = mUniforms[ name ];
		uniform.type		= UNIFORM_INT;
		uniform.components	= glm::clamp( components, (char)1, (char)4 );
		uniform.location 	= mProgram->GetUniformLocation( name );
		memcpy( &uniform.i[0], glm::value_ptr( value ), sizeof( glm::ivec4 ) );
	}

	void GLMaterial::SetUniform( const std::string& name, unsigned value )
	{
		SetUniform( name, glm::uvec4( value, 0, 0, 0 ), 1);
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::uvec2& value )
	{
		SetUniform( name, glm::uvec4( value, 0, 0 ), 2);
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::uvec3& value )
	{
		SetUniform( name, glm::uvec4( value, 0 ), 3 );
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::uvec4& value
		, char components /*= 4 */ )
	{
		UniformState& uniform = mUniforms[ name ];
		uniform.type		= UNIFORM_UINT;
		uniform.components	= glm::clamp( components, (char)1, (char)4 );
		uniform.location 	= mProgram->GetUniformLocation( name );
		memcpy( &uniform.u[0], glm::value_ptr( value ), sizeof( glm::uvec4 ) );
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::mat3& value )
	{
		UniformState& uniform = mUniforms[ name ];
		uniform.type		= UNIFORM_MAT_3_3;
		uniform.location 	= mProgram->GetUniformLocation( name );
		memcpy( &uniform.m3[0][0], glm::value_ptr( value ), sizeof( glm::mat3 ) );
	}

	void GLMaterial::SetUniform( const std::string& name, const glm::mat4& value )
	{
		UniformState& uniform = mUniforms[ name ];
		uniform.type		= UNIFORM_MAT_4_4;
		uniform.location 	= mProgram->GetUniformLocation( name );
		memcpy( &uniform.m4[0][0], glm::value_ptr( value ), sizeof( glm::mat4 ) );
	}

	void GLMaterial::RemoveSampler( const std::string& name )
	{
		mSamplers.erase( std::remove_if( mSamplers.begin(), mSamplers.end()
			, [&name]( const SamplerState& a ) { return a.name == name; } ), mSamplers.end() );
	}

} /* namespace GL */

} /* namespace Procyon */