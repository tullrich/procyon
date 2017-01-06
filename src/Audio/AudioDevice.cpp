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
#include "AudioDevice.h"

namespace Procyon {

	AudioDevice::AudioDevice()
		: mDevice( NULL )
		, mContext( NULL )
	{ 
        // create device
		mDevice = alcOpenDevice(NULL);
		if ( !mDevice )
		{
            PROCYON_ERROR( "OpenAL", "Unable to open device" );
            return;
		}

        // create context
        mContext = alcCreateContext( mDevice, NULL );
        if ( !mContext )
        {
            PROCYON_ERROR( "OpenAL", "Unable to create context" );
            return;
        }

        // set active context
        if ( alcMakeContextCurrent( mContext ) != AL_TRUE )
        {
            PROCYON_ERROR( "OpenAL", "Unable to make context current" );
            return;
        }

		SetListenerPosition( glm::vec3() );
		SetListenerOrientation( glm::quat() );
	}

	AudioDevice::~AudioDevice()
	{
		alcMakeContextCurrent( NULL );

        if ( mContext )
        {
            alcDestroyContext( mContext );
        }
        if ( mDevice )
        {
            if ( alcCloseDevice( mDevice ) != AL_TRUE )
            {
                PROCYON_ERROR( "OpenAL", "Unable to close device" );
            }
        }
	}

	void AudioDevice::SetListenerGain( float gain )
	{
		// single float
        PROCYON_AL_CHECKED( alListenerf( AL_GAIN, glm::max( gain, 0.0f ) ) );
	}

	void AudioDevice::SetListenerPosition( const glm::vec3& position )
	{
		// 3 floats
        PROCYON_AL_CHECKED( alListenerfv( AL_POSITION, glm::value_ptr( position ) ) );
	}

	void AudioDevice::SetListenerVelocity( const glm::vec3& velocity )
	{
		// 3 floats
        PROCYON_AL_CHECKED( alListenerfv( AL_VELOCITY, glm::value_ptr( velocity ) ) );
	}

	void AudioDevice::SetListenerOrientation( const glm::quat& orient )
	{
		// orientation expressed as “at” and “up” vectors
		glm::vec3 atUp[2] = 
		{ 
			glm::vec3( 0.0f, 0.0f, -1.0f ) * orient, 
			glm::vec3( 0.0f, 1.0f, 0.0f ) * orient 
		};
        PROCYON_AL_CHECKED( alListenerfv( AL_ORIENTATION, glm::value_ptr( atUp[0] ) ) );
	}

} /* namespace Procyon */
