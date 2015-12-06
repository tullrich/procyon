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
#ifndef _SOUND_BUFFER_H
#define _SOUND_BUFFER_H

#include "ProcyonAL.h"

namespace Procyon {

	class SoundFile;
	
	class SoundBuffer
	{
	public:
				SoundBuffer();
				SoundBuffer( const std::string& filepath );
				~SoundBuffer();

		void 	SetData( const SoundFile& sf );
		ALuint 	GetHandle() const;


		void 	SetSampleRate( int samplerate );
		void 	SetChannelCount( int channels );
		int 	GetSize() const;

	protected:
		ALuint 		mBufferId;
	};
	
} /* namespace Procyon */

#endif /* _SOUND_BUFFER_H */
