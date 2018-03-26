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
#ifndef _IMAGE_H
#define _IMAGE_H

#include "ProcyonCommon.h"

namespace Procyon {

	class IImage
	{
	public:
		virtual int						GetWidth() const = 0;
		virtual int						GetHeight() const = 0;
		virtual int						Components() const = 0;
		virtual const unsigned char* 	Data() const = 0;
	};

	class ImageBase : public IImage
	{
	public:
										ImageBase();
		virtual 						~ImageBase() { };

		virtual int						GetWidth() const;
		virtual int						GetHeight() const;
		virtual int						Components() const;
		virtual const unsigned char* 	Data() const;

	protected:
		int 			mWidth;
		int 			mHeight;
		int 			mComponents;
		unsigned char*	mData;
	};

	class FileImage : public ImageBase
	{
	public:
				FileImage( const std::string& filename );
		virtual ~FileImage();
	};

	class MutableImage : public ImageBase
	{
	public:
				MutableImage( const IImage& in, int components );
				MutableImage( int width, int height, int components );
		virtual ~MutableImage();


		unsigned char* MutableData();
	};
} /* namespace Procyon */

#endif /* _IMAGE_H */
