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

#define PIXELS_PER_METER 32.0f
#define PixelsToMeters( pixels ) 						\
			(float)pixels / (float)PIXELS_PER_METER
#define MetersToPixels( meters )						\
			(float)meters * (float)PIXELS_PER_METER

#define PLAYER_PIXEL_HEIGHT (TILE_PIXEL_SIZE*2)
#define PLAYER_PIXEL_WIDTH TILE_PIXEL_SIZE
#define PLAYER_DIMS (glm::vec2(PLAYER_PIXEL_WIDTH, PLAYER_PIXEL_HEIGHT))

#define PLAYER_DEFAULT_SPEED (PIXELS_PER_METER*3.0f)
#define PLAYER_AIR_CONTROL_RATIO 0.6f
#define PLAYER_JUMP_VELOCITY (PIXELS_PER_METER*11.0f)
#define PLAYER_GRAVITY (PIXELS_PER_METER*-27.5f)
#define PLAYER_MAX_SPEED (glm::vec2(PLAYER_DEFAULT_SPEED, -PLAYER_GRAVITY))
#define PLAYER_ARIAL_DAMPING (0.05f)
#define PLAYER_ARIAL_ACCELERATION (PLAYER_DEFAULT_SPEED*4.5f)

namespace Procyon {
	class Renderable;
	class Renderer;
    class AnimatedSprite;
	class World;
	class Tile;
	class Contact;
	class Sound;
	class Text;
}

class Player
{
public:
						Player( Procyon::World* world );
						~Player();

	void 				Process( Procyon::FrameTime ft );

	void				Draw( Procyon::Renderer* r ) const;

	glm::vec2			GetPosition() const;
	Procyon::Aabb		GetBounds() const;

	// input
	void 				Jump();
	void				Teleport( const glm::vec2& pos );
	void 				SetLeftRightInput( float input );

protected:
	void 				UpdatePosition( Procyon::FrameTime ft );

	void 				CollideTile( const glm::vec2& delta, const glm::ivec2& t, Procyon::FrameTime ft );
	void 				Collide( const Procyon::Aabb& other, Procyon::FrameTime ft );

	void 				CollisionResponse( const Procyon::Contact& c, Procyon::FrameTime ft );

	Procyon::World* 	     mWorld;
	Procyon::Aabb		     mBounds;
	glm::vec2	             mInput;
	glm::vec2 			     mVelocity;
	Procyon::AnimatedSprite* mSprite;
	glm::vec2 			     mPenetrationCorrection;
	Procyon::Sound*		     mJumpSnd;
	Procyon::Text*		     mPlayerText;
	bool				     mGrounded;
};

#endif /* _PLAYER_H */
