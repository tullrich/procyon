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

#ifndef _GRID_H
#define _GRID_H

#include "ProcyonCommon.h"

namespace Procyon {
	class Renderer;
}

class Grid
{
public:

	// Construct Grid with tile dimensions
			Grid( float gridSize );

	// Set/Get if this grid should actually render
	void 	SetEnabled( bool enabled ) { mEnabled = enabled; }
	bool 	GetEnabled() const { return mEnabled; }

	// Set/Get the grid tile size to render
	void 	SetGridSize( float gridsize );
	float 	GetGridSize() const { return mGridSize; }

	// Set the major line frequency
	void 	SetMajorLineFrequency( int frequency );

	// Render the grid using debug lines.
	void 	Render( Procyon::Renderer* r, const glm::ivec2& size );

protected:

	// if false Render results in a no-op
	bool 	mEnabled;

	// the grid tile size to use
	float 	mGridSize;

	// 'major' line frequency. 1 is every line, 2 is every other line, etc.
	int 	mMajorFrequency;
};

#endif /* _GRID_H */