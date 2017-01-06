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
#include "Sound.h"
#include "SoundBuffer.h"

namespace Procyon {

	Sound::Sound()
		: mSourceId( AL_NONE )
		, mBuffer( NULL )
	{
        PROCYON_AL_CHECKED( alGenSources( 1, &mSourceId ) );
	}

	Sound::Sound( const SoundBuffer* buffer )
		: mSourceId( AL_NONE )
		, mBuffer( NULL )
	{
        PROCYON_AL_CHECKED( alGenSources( 1, &mSourceId ) );
		SetBuffer( buffer );
	}

	Sound::~Sound()
	{
        PROCYON_AL_CHECKED( alDeleteSources( 1, &mSourceId ) );
	}

	void Sound::SetBuffer( const SoundBuffer* buffer )
	{
		if ( buffer )
		{
            PROCYON_AL_CHECKED( alSourcei( mSourceId, AL_BUFFER, buffer->GetHandle() ) );
		}
		else
		{
            PROCYON_AL_CHECKED( alSourcei( mSourceId, AL_BUFFER, AL_NONE ) );
		}
		mBuffer = buffer;
	}

	void Sound::Play( bool restart /*= false*/ )
	{
		if ( mBuffer )
		{
			if ( restart )
			{
                PROCYON_AL_CHECKED( alSourceRewind( mSourceId ) );
			}
			
            PROCYON_AL_CHECKED( alSourcePlay( mSourceId ) );
		}
	}

	void Sound::Stop()
	{
		if ( mBuffer )
		{
            PROCYON_AL_CHECKED( alSourceStop( mSourceId ) );
		}
	}

	void Sound::Pause()
	{
		if ( mBuffer )
		{
            PROCYON_AL_CHECKED( alSourcePause( mSourceId ) );
		}
	}

	void Sound::SetPitch( float pitch )
	{
        PROCYON_AL_CHECKED( alSourcef( mSourceId, AL_PITCH, glm::max( pitch, 0.0f ) ) );
	}

	void Sound::SetGain( float gain )
	{
        PROCYON_AL_CHECKED( alSourcef( mSourceId, AL_GAIN, glm::max( gain, 0.0f ) ) );
	}

	// Max distance this sound can be heard
	void Sound::SetFalloffDistance( float falloffDist )
	{
        PROCYON_AL_CHECKED( alSourcef( mSourceId, AL_MAX_DISTANCE, glm::max( falloffDist, 0.0f ) ) );
	}

	void Sound::SetFalloffRate( float falloffRate )
	{
        PROCYON_AL_CHECKED( alSourcef( mSourceId, AL_ROLLOFF_FACTOR, glm::max( falloffRate, 0.0f ) ) );
	}

	// the distance under which the volume for the source
	// would normally drop by half (before being influenced
	// by rolloff factor or AL_MAX_DISTANCE)
	void Sound::SetHalfFalloffDistance( float halfFalloffDist )
	{
        PROCYON_AL_CHECKED( alSourcef( mSourceId, AL_REFERENCE_DISTANCE, glm::max( halfFalloffDist, 0.0f ) ) );
	}

	void Sound::SetPosition( const glm::vec3& position )
	{
        PROCYON_AL_CHECKED( alSourcefv( mSourceId, AL_POSITION, glm::value_ptr( position ) ) );
	}

	void Sound::SetVelocity( const glm::vec3& velocity )
	{
        PROCYON_AL_CHECKED( alSourcefv( mSourceId, AL_VELOCITY, glm::value_ptr( velocity ) ) );
	}

	void Sound::SetDirection( const glm::vec3& direction )
	{
        PROCYON_AL_CHECKED( alSourcefv( mSourceId, AL_DIRECTION, glm::value_ptr( direction ) ) );
	}

	void Sound::SetIs3D( bool is3d )
	{
        PROCYON_AL_CHECKED( alSourcei( mSourceId, AL_SOURCE_RELATIVE, ( is3d ) ? AL_TRUE : AL_FALSE ) );
	}

	void Sound::SetLooping( bool loop )
	{
        PROCYON_AL_CHECKED( alSourcei( mSourceId, AL_LOOPING, ( loop ) ? AL_TRUE : AL_FALSE ) );
	}

	// TODO: the following NYI...
	// alSourceQueueBuffers + alSourceUnqueueBuffers
	// Cone properties? AL_CONE_OUTER_GAIN + AL_CONE_INNER_ANGLE + AL_CONE_OUTER_ANGLE

} /* namespace Procyon */
