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
#ifndef _AUDIO_DEVICE
#define _AUDIO_DEVICE

#include "ProcyonAL.h"

namespace Procyon {
	
	class AudioDevice
	{
	public:
				AudioDevice();
				~AudioDevice();

		// Listener control
		void 	SetListenerGain( float gain );
		void 	SetListenerPosition( const glm::vec3& position );
		void 	SetListenerVelocity( const glm::vec3& velocity );
		void 	SetListenerOrientation( const glm::quat& orient );

	protected:
		ALCdevice* 	mDevice;
		ALCcontext* mContext;

		ALuint 		mSourceId;
	};
	
} /* namespace Procyon */

#endif /* _AUDIO_DEVICE */
