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
#include "SoundFile.h"
#include <sndfile.h>

namespace Procyon {

	SoundFile::SoundFile( const std::string& filepath, bool forceToMono /*= false*/ )
		: mFilePath( filepath )
		, mBuffer( NULL )
		, mSize( 0 )
	{
		if ( !BufferContents() )
		{
			delete mBuffer;
			throw std::runtime_error( "SoundFile" );
		}

		if ( forceToMono && mInfo.channels > 1 )
		{
			ForceToMono();
		}
	}

	SoundFile::~SoundFile()
	{
		delete mBuffer;
	}

	bool SoundFile::BufferContents()
	{
		// see http://www.mega-nerd.com/libsndfile/api.html
		SF_INFO info;
		info.format = 0;
 		SNDFILE* snd = sf_open( mFilePath.c_str(), SFM_READ, &info );

		if ( !snd )
		{
			sf_close( snd );
			PROCYON_ERROR( "Audio", "Unable to open sound file '%s': %s."
				, mFilePath.c_str(), sf_strerror( NULL ) );
			return false; // return failure
		}

		int size = info.frames * info.channels;
		mBuffer = new short[ size ];
		sf_count_t read = sf_readf_short( snd, mBuffer, info.frames );

		if ( read != info.frames )
		{
			sf_close( snd );
			PROCYON_ERROR( "Audio", "Failed to fully read SoundFile '%s' %i."
				, read );
			return false; // return failure
		}

		if ( sf_close( snd ) )
		{
			PROCYON_ERROR( "Audio", "Error closing SoundFile '%s'."
				, mFilePath.c_str() );
			return false; // return failure
		}

		mInfo.frames 		= info.frames;
		mInfo.samplerate 	= info.samplerate;
		mInfo.channels 		= info.channels;
		mSize 				= size;

		// return success
		return true;
	}

	void SoundFile::ForceToMono()
	{
		short* tmpBuffer = new short[ mInfo.frames ];

		for ( int f = 0; f < mInfo.frames; f++ )
		{
			int avg = 0;
			for ( int c = 0; c < mInfo.channels; c++ )
			{
				avg += mBuffer[ f * mInfo.channels + c ];
			}
			tmpBuffer[ f ] = (short)( avg / mInfo.channels );
		}

		delete mBuffer;

		mBuffer 			= tmpBuffer;
		mSize 				= mInfo.frames;
		mInfo.channels 		= 1;
	}

 	const short* SoundFile::GetBuffer() const
 	{
 		return mBuffer;
 	}

 	int SoundFile::GetByteSize() const
 	{
 		return mSize * sizeof( short );
 	}

	int SoundFile::GetChannelCount() const
	{
		return mInfo.channels;
	}

	const std::string& SoundFile::GetFilePath() const
	{
		return mFilePath;
	}

	int SoundFile::GetSampleRate() const
	{
		return mInfo.samplerate;
	}
} /* namespace Procyon */
