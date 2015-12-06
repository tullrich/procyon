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
#ifndef _SOUND_H
#define _SOUND_H

#include "ProcyonAL.h"

namespace Procyon {

	class SoundBuffer;
	
	class Sound
	{
	public:
				Sound();
				Sound( const SoundBuffer* buffer );
				~Sound();

		void 	SetBuffer( const SoundBuffer* buffer );

		void 	Play( bool restart = false );
		void 	Stop();
		void 	Pause();

		void 	SetPitch( float pitch );
		void 	SetGain( float gain );

		void 	SetFalloffDistance( float falloffDist );
		void 	SetFalloffRate( float falloffRate );
		void 	SetHalfFalloffDistance( float halfFalloffDist );

		void 	SetPosition( const glm::vec3& position );
		void 	SetVelocity( const glm::vec3& velocity );
		void 	SetDirection( const glm::vec3& direction );
		void 	SetIs3D( bool is3d );
		void 	SetLooping( bool loop );

	protected:
		const SoundBuffer* 	mBuffer;
		ALuint 				mSourceId;
	};
	
} /* namespace Procyon */

#endif /* _SOUND_H */
