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

#define PIXELS_PER_METER 128.0f
#define PixelsToMeters( pixels ) 						\
			(float)pixels / (float)PIXELS_PER_METER
#define MetersToPixels( meters )						\
			(float)meters * (float)PIXELS_PER_METER

#define PLAYER_PIXEL_HEIGHT TILE_PIXEL_SIZE
#define PLAYER_PIXEL_WIDTH TILE_PIXEL_SIZE
#define PLAYER_DIMS (glm::vec2(PLAYER_PIXEL_WIDTH, PLAYER_PIXEL_HEIGHT))

#define PLAYER_DEFAULT_SPEED (PIXELS_PER_METER*3.0f)
#define PLAYER_MAX_SPEED PLAYER_DEFAULT_SPEED
#define PLAYER_AIR_CONTROL_RATIO 0.6f
#define PLAYER_JUMP_VELOCITY (PIXELS_PER_METER*4.0f)
#define PLAYER_GRAVITY -9.8f
#define PLAYER_GRAVITY_PIXELS (PIXELS_PER_METER*PLAYER_GRAVITY)

namespace Procyon {

	class Sprite;
	class Renderable;
	class Renderer;
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

		void				Draw( Renderer* r ) const;

		glm::vec2			GetPosition() const;
		Aabb				GetBounds() const;

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
		glm::vec2	mAcceleration;
		Sprite* 	mSprite;
		glm::vec2 	mPenetrationCorrection;
		Sound*		mJumpSnd;
		bool		mGrounded;
	};
	
} /* namespace Procyon */

#endif /* _PLAYER_H */
