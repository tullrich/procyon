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
		virtual int						Width() const = 0;
		virtual int						Height() const = 0;
		virtual int						Components() const = 0;
		virtual const unsigned char* 	Data() const = 0;
	};

	class ImageBase : public IImage
	{
	public:
										ImageBase();
		virtual 						~ImageBase() { };

		virtual int						Width() const;
		virtual int						Height() const;
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

	class MemoryImage : public ImageBase
	{
	public:
				MemoryImage( const IImage& in, int components );
				MemoryImage( int width, int height, int components );
		virtual ~MemoryImage();


		unsigned char* MutableData();
	};


	typedef std::unique_ptr<IImage> IImagePtr;
	typedef std::unique_ptr<FileImage> FileImagePtr;
	typedef std::unique_ptr<MemoryImage> MemoryImagePtr;

} /* namespace Procyon */

#endif /* _IMAGE_H */