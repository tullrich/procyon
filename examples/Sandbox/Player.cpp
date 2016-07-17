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
#include "Player.h"
#include "Sprite.h"
#include "World.h"
#include "Contact.h"
#include "Sound.h"
#include "SandboxAssets.h"

namespace Procyon {

	static bool AabbVsAabb( const Aabb& aabb1, const Aabb& aabb2 )
	{
		glm::vec2 min1 = aabb1.GetMin();
		glm::vec2 min2 = aabb2.GetMin();
		glm::vec2 max1 = aabb1.GetMax();
		glm::vec2 max2 = aabb2.GetMax();

		bool xoverlap = max1.x > min2.x && min1.x < max2.x;
		bool yoverlap = max1.y > min2.y && min1.y < max2.y;

		return xoverlap && yoverlap;
	}

	static Contact QueryAabbVsAabb( const Aabb& aabb1, const Aabb& aabb2 )
	{
		Contact out;

		// Convert to AABB vs. Point
		Aabb scaledAabb( aabb2.mCenter, aabb1.mHalfExtent + aabb2.mHalfExtent );
		glm::vec2 point( aabb1.mCenter );

		// Find the contact normal via the major-axis
		glm::vec2 centerDist = aabb1.mCenter - aabb2.mCenter;
		if( abs( centerDist.x ) > abs( centerDist.y ) )
		{
			out.normal = glm::vec2( (centerDist.x > 0) ? 1.0f : -1.0f, 0.0f );
		}
		else
		{
			out.normal = glm::vec2( 0.0f, (centerDist.y > 0) ? 1.0f : -1.0f );
		}

		// Find the contact distance via plane/point dist
		glm::vec2 planeSurfacePoint = scaledAabb.mHalfExtent  * out.normal + scaledAabb.mCenter;
		glm::vec2 planeToPoint = point - planeSurfacePoint;
		out.distance = glm::dot( planeToPoint, out.normal );

		return out;
	}

	Player::Player( World* world )
		: mVelocity( 0.0f, 0.0f )
		, mWorld( world )
		, mBounds( glm::vec2( TILE_PIXEL_SIZE / 1.25f ) + TILE_PIXEL_SIZE / 2.0f, glm::vec2( TILE_PIXEL_SIZE / 2.0f ) - .0001f )
	{
		mSprite = new Sprite( SandboxAssets::sPlayerTexture );
		mSprite->SetOrigin( 0.5f, 0.5f );
		mSprite->SetDimensions( mBounds.mHalfExtent * 2.0f );
		mSprite->SetPosition( mBounds.mCenter );

		mJumpSnd = new Sound( SandboxAssets::sJumpSound );
	}

	Player::~Player()
	{
		delete mSprite;
		delete mJumpSnd;
	}

	void Player::UpdatePosition( FrameTime ft )
	{
		mBounds.mCenter += mVelocity * ft.dt;

		if ( mPenetrationCorrection.length() > 0.0f )
		{
			mBounds.mCenter += mPenetrationCorrection;
			mPenetrationCorrection = glm::vec2();
		}

		mSprite->SetPosition( mBounds.mCenter );
	}

	void Player::Process( FrameTime ft )
	{
		// Gravity
		mVelocity += glm::vec2( 0.0f, 32.0f * -9.8f * ft.dt );
		PROCYON_DEBUG( "Player", "vel %f, %f", mVelocity.x, mVelocity.y );

		glm::vec2 projPos = mBounds.mCenter + mVelocity * ft.dt;

		Aabb projBounds( projPos, mBounds.mHalfExtent );
		projBounds.Contain( mBounds );

		std::vector<glm::ivec2> intersecting;
		mWorld->TileIntersection( projBounds, intersecting );

		for ( auto& t : intersecting )
		{
			CollideTile( t, ft );
		}

		UpdatePosition( ft );
	}

	void Player::CollideTile( const glm::ivec2& t, FrameTime ft )
	{
		Aabb tileBounds( ( glm::vec2( t ) + glm::vec2( 0.5f ) ) * (float)TILE_PIXEL_SIZE
			, glm::vec2( HALF_TILE_SIZE ) );

		Contact c = QueryAabbVsAabb( mBounds, tileBounds );

		if ( !mWorld->IsInternalCollision( t, c ) )
		{

			PROCYON_DEBUG( "PlayerCollision", "Contact with tile center <%i, %i>" \
				" normal <%.2f, %.2f> distance %.20f"
				, t.x, t.y, c.normal.x, c.normal.y, c.distance * 100.0f );
			CollisionResponse( c, ft );
		}
		else
		{
			/*PROCYON_DEBUG( "PlayerCollision", "Ignoring internal collision with tile <%i, %i> Normal <%f, %f>"
				, t->GetGridCoords().x, t->GetGridCoords().y, c.normal.x, c.normal.y );*/
		}
	}

	void Player::Collide( const Aabb& other, FrameTime ft )
	{
		Contact c = QueryAabbVsAabb( mBounds, other );

		PROCYON_DEBUG( "PlayerCollision", "4. Narrowphase contactNormal <%f, %f>, contactDist %f"
			, c.normal.x, c.normal.y, c.distance );
		CollisionResponse( c, ft );
	}

	void Player::CollisionResponse( const Contact& c, FrameTime ft )
	{
		float separation = glm::max( c.distance, 0.0f );
		float penetration  = glm::min( c.distance, 0.0f );

		float normVel = glm::dot( mVelocity, c.normal ) + separation / ft.dt;

		if ( penetration < 0.0f )
		{
			mPenetrationCorrection -= c.normal * penetration;/*
			PROCYON_DEBUG( "PlayerCollision", "4.5 CollisionResponse mPenetrationCorrection <%f, %f>"
				, mPenetrationCorrection.x, mPenetrationCorrection.y );*/
		}

		if( normVel < 0.0f )
		{/*
			PROCYON_DEBUG( "PlayerCollision", "5. CollisionResponse velCorrection <%f, %f>"
				, (c.normal * normVel).x, (c.normal * normVel).y );*/
			mVelocity -= c.normal * normVel;

			if ( c.normal.y > 0.0f ) // friction
			{
				glm::vec2 tangent( -c.normal.y, c.normal.x );
				float tanVelocity = glm::dot( mVelocity, tangent ) * 0.2f; // friction
				mVelocity -= tangent  * tanVelocity;
			}
		}
	}

	const Renderable* Player::GetRenderable() const
	{
		return mSprite;
	}

	glm::vec2 Player::GetPosition() const
	{
		return mBounds.mCenter;
	}

	void Player::Jump()
	{
		if ( mVelocity.y >= 0.0f )
		{
			mVelocity += glm::vec2( 0.0f, 225.0f );
        	//mJumpSnd->Play( true );
		}
	}

	void Player::SetLeftRightInput( float input )
	{
		const float kSpeed = 75.0f;
		mVelocity.x = glm::clamp( kSpeed * input, -kSpeed, kSpeed );
	}

} /* namespace Procyon */
