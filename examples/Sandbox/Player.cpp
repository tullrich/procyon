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
#include "Graphics/Sprite.h"
#include "Graphics/Renderer.h"
#include "Collision/World.h"
#include "Collision/Contact.h"
#include "Audio/Sound.h"
#include "Player.h"
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

	static Contact QueryAabbVsAabb_Old( const Aabb& aabb1, const Aabb& aabb2 )
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

	static bool TestAabbAxis( const glm::vec2& axis, float minA, float maxA, float minB, float maxB, glm::vec2* mtvAxis, float* mtvDist )
	{
        float d0 = (maxB - minA);   // 'Left' side
        float d1 = (maxA - minB);   // 'Right' side

		if (d0 <= 0.0f || d1 <= 0.0f)
		{
			return false;
		}

        float overlap = (d0 < d1) ? d0 : -d1;

		glm::vec2 seperation = axis * overlap;

        float sepLengthSquared = glm::dot(seperation, seperation);
		if ( sepLengthSquared < *mtvDist )
		{
			*mtvDist = sepLengthSquared;
			*mtvAxis = seperation;
		}

        return true;
	}

	static bool QueryAabbVsAabb( const Aabb& a, const Aabb& b, Contact* out )
	{
		float mtvDist = std::numeric_limits<float>::max();
		glm::vec2 mtvAxis;

		if ( !TestAabbAxis( glm::vec2(1.0f, 0.0f),
				a.GetMin().x, a.GetMax().x, b.GetMin().x, b.GetMax().x,
				&mtvAxis, &mtvDist ) )
		{
			return false;
		}

		if ( !TestAabbAxis( glm::vec2(0.0f, 1.0f),
				a.GetMin().y, a.GetMax().y, b.GetMin().y, b.GetMax().y,
				&mtvAxis, &mtvDist ) )
		{
			return false;
		}

		out->normal = glm::normalize(mtvAxis);
		out->distance = sqrt(mtvDist) * -1.001f;

   		return true;
	}

	Player::Player( World* world )
		: mVelocity( 0.0f, 0.0f )
		, mWorld( world )
		, mBounds( glm::vec2( TILE_PIXEL_SIZE / 1.25f ) + 4.0f*TILE_PIXEL_SIZE, glm::vec2( PLAYER_PIXEL_WIDTH, PLAYER_PIXEL_HEIGHT ) / 2.0f - .0001f )
		, mGrounded( true )
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
		if ( mGrounded )
		{
			mVelocity.x = PLAYER_DEFAULT_SPEED * mInput.x;
		}
		else
		{
			mVelocity.x += 3.5f * PLAYER_DEFAULT_SPEED * mInput.x * ft.dt;
		}
		mVelocity.y += PLAYER_GRAVITY_PIXELS * ft.dt;
		mVelocity = glm::clamp(mVelocity, -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);

		mBounds.mCenter += mVelocity * ft.dt;

		std::vector<glm::ivec2> intersecting;
		mWorld->TileIntersection(mBounds, intersecting );

		mGrounded = false;
		for ( auto& t : intersecting )
		{
			CollideTile( t, ft );
		}

		UpdatePosition( ft );

		if ( mGrounded )
		{
			mVelocity.y = 0.0f;
		}

		mInput = glm::vec2(0.0f);
	}

	void Player::Draw( Renderer* r ) const
	{
		r->DrawRectShape(GetPosition(), PLAYER_DIMS, 0.0f, glm::vec4(245/255.0f, 121/255.0f, 79/255.0f, 1.f));
	}

	void Player::CollideTile( const glm::ivec2& t, FrameTime ft )
	{
		Aabb tileBounds( ( glm::vec2( t ) + glm::vec2( 0.5f ) ) * (float)TILE_PIXEL_SIZE
			, glm::vec2( HALF_TILE_SIZE ) );

		Contact c;
		if ( QueryAabbVsAabb( mBounds, tileBounds, &c ) && !mWorld->IsInternalCollision( t, c ) )
		{
			PROCYON_DEBUG( "PlayerCollision", "Contact with tile center <%i, %i>" \
				" normal <%.2f, %.2f> distance %.20f"
				, t.x, t.y, c.normal.x, c.normal.y, c.distance * 100.0f );
			CollisionResponse( c, ft );
		}
	}

	void Player::Collide( const Aabb& other, FrameTime ft )
	{
		Contact c;
		if ( QueryAabbVsAabb( mBounds, other, &c ) )
		{
			CollisionResponse( c, ft );
		}
	}

	void Player::CollisionResponse( const Contact& c, FrameTime ft )
	{
		float separation = glm::max( c.distance, 0.0f );
		float penetration  = glm::min( c.distance, 0.0f );

		float normVel = glm::dot( mVelocity, c.normal ) + separation / ft.dt;

		if ( penetration < 0.0f )
		{
			mBounds.mCenter -= c.normal * penetration;
		}

		if( normVel < 0.0f )
		{
			mVelocity -= c.normal * normVel;

			// friction
			if ( c.normal.y > 0.0f )
			{
				glm::vec2 tangent( -c.normal.y, c.normal.x );
				float tanVelocity = glm::dot( mVelocity, tangent ) * 0.2f; // friction
				mVelocity -= tangent  * tanVelocity;

				mGrounded = true;
			}
		}
	}

	glm::vec2 Player::GetPosition() const
	{
		return mBounds.mCenter;
	}

	Aabb Player::GetBounds() const
	{
		return mBounds;
	}

	void Player::Jump()
	{
		mInput.y = 1.0f;
		if ( mGrounded )
		{
			mVelocity += glm::vec2( 0.0f, PLAYER_JUMP_VELOCITY );
        	mJumpSnd->Play( true );
			mGrounded = false;
		}
	}

	void Player::SetLeftRightInput( float input )
	{
		mInput.x = input;
	}

} /* namespace Procyon */
