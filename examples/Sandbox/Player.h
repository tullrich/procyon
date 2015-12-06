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
#ifndef _PLAYER_H
#define _PLAYER_H

#include "ProcyonCommon.h"
#include "Aabb.h"

namespace Procyon {

	class Sprite;
	class Renderable;
	class World;
	class Tile;
	class Contact;
	class Sound;

	class Player
	{
	public:
							Player( World* world );
							~Player();

		void 				Process( FrameTime ft );

		const Renderable* 	GetRenderable() const;

		glm::vec2			GetPosition() const;

		// input
		void 				Jump();
		void 				SetLeftRightInput( float input );

	protected:
		void 				UpdatePosition( FrameTime ft );

		void 				CollideTile( const glm::ivec2& t, FrameTime ft );
		void 				Collide( const Aabb& other, FrameTime ft );

		void 				CollisionResponse( const Contact& c, FrameTime ft );

		World* 		mWorld;
		Aabb		mBounds;
		glm::vec2 	mVelocity;
		Sprite* 	mSprite;
		glm::vec2 	mPenetrationCorrection;
		Sound*		mJumpSnd;
	};
	
} /* namespace Procyon */

#endif /* _PLAYER_H */
