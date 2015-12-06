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
#ifndef _SOUND_FILE_H
#define _SOUND_FILE_H

#include "ProcyonCommon.h"

namespace Procyon {

	struct SoundInfo
	{
		int frames;
		int channels;
		int samplerate;

		SoundInfo()
			: frames( 0 )
			, channels( 0 )
			, samplerate( 0 )
		{
		}
	};
	
	class SoundFile
	{
 	public:
 							SoundFile( const std::string& filepath, bool forceToMono = false );
 							~SoundFile();

 		const short* 		GetBuffer() const;
 		int 				GetByteSize() const;
 		int 				GetChannelCount() const;
 		const std::string& 	GetFilePath() const;
 		int 				GetSampleRate() const;

 	protected:
 		bool 				BufferContents();
 		void 				ForceToMono();

 		std::string 		mFilePath;
 		int 				mSize;
 		short* 				mBuffer;
 		SoundInfo 			mInfo;
	};
	
} /* namespace Procyon */

#endif /* _SOUND_FILE_H */
