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
#include "SoundFile.h"
#include <pulse/simple.h>
#include <pulse/context.h>
#include <pulse/error.h>
#include <pulse/mainloop.h>
#include <pulse/stream.h>

namespace Procyon {

	static void PAContextStateChangedProc( pa_context *c, void *userdata )
	{
		pa_context_state_t state = pa_context_get_state( c );

		bool good = PA_CONTEXT_IS_GOOD( state );

		PROCYON_DEBUG( "Audio", "PAContextStateChangedProc" );
	}


	AudioDevice::AudioDevice()
		: mContext( NULL )
	{ 
		static const pa_sample_spec ss = {
	        .format = PA_SAMPLE_S16NE,
	        .rate = 44100,
	        .channels = 2
   		};
		s = pa_simple_new(NULL,               // Use the default server.
		                  "ProcyonSimple",           // Our application's name.
		                  PA_STREAM_PLAYBACK,
		                  NULL,               // Use the default device.
		                  "Music",            // Description of our stream.
		                  &ss,                // Our sample format.
		                  NULL,               // Use default channel map
		                  NULL,               // Use default buffering attributes.
		                  NULL 	              // Ignore error code.
		                  );

		mMainLoop = pa_mainloop_new(); 

		mContext = pa_context_new( pa_mainloop_get_api( mMainLoop ), "Procyon" );
		pa_context_set_state_callback( mContext, PAContextStateChangedProc, (void*)this );

		if ( pa_context_connect( mContext, NULL, PA_CONTEXT_NOFLAGS, NULL ) < 0 )
		{
			int error = error = pa_context_errno( mContext );
			PROCYON_ERROR( "Audio", "Unable to establish connect to PulseAudio server.");
			throw std::runtime_error("AudioDevice");
		}

		while ( true )
		{
			pa_context_state_t state = pa_context_get_state( mContext );

			if ( !PA_CONTEXT_IS_GOOD( state )) 
			{
            	int error = pa_context_errno( mContext );
				PROCYON_ERROR( "Audio", "PulseAudio connection error: %i.", error);
				throw std::runtime_error("AudioDevice");
            }
            else if ( state == PA_CONTEXT_READY )
            {
				PROCYON_DEBUG( "Audio", "PulseAudio connection established!" );
            	break;
            }

			int error;
			pa_mainloop_iterate( mMainLoop, 0, &error );
		}

		mStream = pa_stream_new( mContext, "Procyon Stream", &ss, NULL );

		if ( pa_stream_connect_playback( mStream, NULL, NULL, PA_STREAM_NOFLAGS, NULL, NULL ) < 0 )
		{
			throw std::runtime_error("AudioDevice");
		}


		while ( true )
		{
			pa_stream_state_t state = pa_stream_get_state( mStream );

			if ( !PA_STREAM_IS_GOOD( state ) )
			{
            	int error = pa_context_errno( mContext );
				PROCYON_ERROR( "Audio", "PulseAudio stream error: %i.", error);
				throw std::runtime_error("AudioDevice");

			}

			if ( state == PA_STREAM_READY )
			{
				PROCYON_DEBUG( "Audio", "PulseAudio playback stream created!" );
            	break;
			}

			int error;
			pa_mainloop_iterate( mMainLoop, 0, &error );
		}

		SoundFile f;

		int error;
		if ( pa_simple_write( s, f.GetBuffer(), f.GetSize(), &error ) < 0 )
		{
			PROCYON_DEBUG( "Audio", "pa_simple_write error: %s", pa_strerror( error ) );
		}
	}

	AudioDevice::~AudioDevice()
	{
		pa_mainloop_free( mMainLoop );
		pa_context_unref( mContext ); // frees mContext

		pa_simple_free(s);
	}

	void AudioDevice::Process()
	{
		int error;
		if ( pa_mainloop_iterate( mMainLoop, 0, &error ) < 0 )
		{
			// error or exit request
		}
	}

} /* namespace Procyon */
