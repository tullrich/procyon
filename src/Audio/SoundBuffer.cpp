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
#include "SoundBuffer.h"
#include "SoundFile.h"

namespace Procyon {

	SoundBuffer::SoundBuffer()
		: mBufferId( AL_NONE )
	{
		alGenBuffers( 1, &mBufferId );
	}

	SoundBuffer::SoundBuffer( const std::string& filepath )
		: mBufferId( AL_NONE )
	{
		alGenBuffers( 1, &mBufferId );
		
		SoundFile snd( filepath, true );
		SetData( snd );
	}

	SoundBuffer::~SoundBuffer()
	{
		alDeleteBuffers( 1, &mBufferId );
	}

	void SoundBuffer::SetData( const SoundFile& snd )
	{
		//TODO: Could support single byte formats here too.
		ALenum format;
		switch( snd.GetChannelCount() )
		{
			case 1:
				format = AL_FORMAT_MONO16; break;
			case 2:
				format = AL_FORMAT_STEREO16; break;
			default:
			{
				PROCYON_ERROR( "Audio", "SoundBuffer::SetData called with SoundFile ('%s') of" \
					" unsupported format: %i channels!"
					, snd.GetFilePath().c_str(), snd.GetChannelCount() );
				return;
			}
		}
		alBufferData( mBufferId, format, snd.GetBuffer(), snd.GetByteSize(), snd.GetSampleRate() );
	}

	ALuint SoundBuffer::GetHandle() const
	{
		return mBufferId;
	}

	void SoundBuffer::SetSampleRate( int samplerate )
	{
		alBufferi( mBufferId, AL_FREQUENCY, samplerate ); 
	}

	void SoundBuffer::SetChannelCount( int channels )
	{
		alBufferi( mBufferId, AL_CHANNELS, glm::max( channels, 1 ) ); 
	}

	int SoundBuffer::GetSize() const
	{
		int size = 0;
		alGetBufferi( mBufferId, AL_SIZE, &size );
		return size;
	}

} /* namespace Procyon */
