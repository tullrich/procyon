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
#ifndef _GL_MATERIAL_H
#define _GL_MATERIAL_H

#include "ProcyonGL.h"

namespace Procyon {

namespace GL { 

	class GLTexture;
	class GLProgram;

	class GLMaterial
	{
		struct SamplerState
		{
			const GLTexture* 	texture;
			std::string 		name;
			int					location;

			SamplerState()
				: texture( NULL )
				, location( -1 )
			{
			}
		};

		enum UniformType
		{
			UNIFORM_FLOAT,
			UNIFORM_INT,
			UNIFORM_UINT,
			UNIFORM_MAT_3_3,
			UNIFORM_MAT_4_4
		};

		struct UniformState
		{
			int 		location;
			UniformType type;
			char		components;

			union
			{
				float 		f[4];
				unsigned 	u[4];
				int 		i[4];
				float 		m3[3][3];
				float 		m4[4][4];
			};

			UniformState()
				: location( -1 )
				, type( UNIFORM_FLOAT )
				, m4{ 0.0f }
				, components( 1 )
			{
			}
		};

	public:
				GLMaterial( const GLProgram* program );

		const GLProgram* GetProgram() const;
		
		void	Bind() const;

		void 	SetSampler( const std::string& name, const GLTexture* texture );

		// float uniform setters
		void	SetUniform( const std::string& name, float value );
		void	SetUniform( const std::string& name, const glm::vec2& value );
		void	SetUniform( const std::string& name, const glm::vec3& value );
		void	SetUniform( const std::string& name, const glm::vec4& value, char components = 4 );

		// int uniform setters
		void	SetUniform( const std::string& name, int value );
		void	SetUniform( const std::string& name, const glm::ivec2& value );
		void	SetUniform( const std::string& name, const glm::ivec3& value );
		void	SetUniform( const std::string& name, const glm::ivec4& value, char components = 4 );

		// unsigned uniform setters
		void	SetUniform( const std::string& name, unsigned value );
		void	SetUniform( const std::string& name, const glm::uvec2& value );
		void	SetUniform( const std::string& name, const glm::uvec3& value );
		void	SetUniform( const std::string& name, const glm::uvec4& value, char components = 4 );

		// mat uniform setter
		void	SetUniform( const std::string& name, const glm::mat3& value );
		void	SetUniform( const std::string& name, const glm::mat4& value );

	protected:
		void  	RemoveSampler( const std::string& name );

		typedef std::vector< SamplerState > SamplerArray;
		typedef std::unordered_map< std::string, UniformState > UniformMap;

		const GLProgram* 	mProgram;
		SamplerArray 		mSamplers;
		UniformMap			mUniforms;
	};

	typedef std::shared_ptr<GLMaterial> GLMaterialPtr;

} /* namespace GL */

} /* namespace Procyon */

#endif /* _GL_MATERIAL_H */