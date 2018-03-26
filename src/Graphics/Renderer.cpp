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
#include "Renderer.h"
#include "RenderCore.h"
#include "Renderable.h"
#include "Texture.h"
#include "Platform/Window.h"
#include "Graphics/GL/GLContext.h"

namespace Procyon {

	bool sDebugLines = false;

	Renderer::Renderer( IWindow* window )
		: mWindow( window )
		, mClearColor( 0.0f, 1.0f, 0.0f, 1.0f )
	{
		if ( mWindow )
		{
			mWindow->GetGLContext();
		}
		mRenderCore = RenderCore::Allocate();
		ResetCameras();
	}

	Renderer::~Renderer()
	{
		delete mRenderCore;
	}

	void Renderer::Flush()
	{
		//TODO: compare camera here and maybe prevent a flush?

		// flush the pipeline
		if ( mRenderCore->RenderCommandsPending() )
			mRenderCore->Flush( mCameras.top() );
	}

	const Camera2D& Renderer::PushCamera()
	{
		Flush();
		mCameras.push( mCameras.top() );
		return mCameras.top();
	}

	const Camera2D& Renderer::PushCamera( const Camera2D& camera )
	{
		Flush();
		mCameras.push( camera );
		return mCameras.top();
	}

	void Renderer::PopCamera()
	{
		Flush();
		mCameras.pop();
	}

	const Camera2D& Renderer::GetCamera()
	{
		return mCameras.top();
	}

	void Renderer::ResetCameras( const Camera2D& camera /*= Camera2D()*/ )
	{
		Flush();
		while( mCameras.size() > 0 )
			mCameras.pop();

		mCameras.push( camera );
	}

	void Renderer::SetClearColor( const glm::vec4 color )
	{
		mClearColor = color;
	}

	const glm::vec4& Renderer::GetClearColor() const
	{
		return mClearColor;
	}

	void Renderer::BeginRender()
	{
		if ( mWindow )
		{
			mWindow->GetGLContext()->MakeCurrent();
			glViewport( 0, 0, mWindow->GetWidth(), mWindow->GetHeight() );
		}

		glClearColor( mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
		glDisable( GL_MULTISAMPLE );

		mRenderCore->ResetStats();
	}

	void Renderer::Draw( const Renderable* r )
	{
		r->PostRenderCommands( this, mRenderCore );
	}

    void Renderer::DrawWireframeRect( const Rect& rect, const glm::vec4& color, bool screenSpace )
    {
        if ( screenSpace )
        {
            DrawLine( rect.GetTopLeft(), rect.GetTopRight(), color );
            DrawLine( rect.GetTopLeft(), rect.GetBottomLeft(), color );
            DrawLine( rect.GetBottomRight(), rect.GetTopRight(), color );
            DrawLine( rect.GetBottomRight(), rect.GetBottomLeft(), color );
        }
        else
        {
            DrawWorldLine( rect.GetTopLeft(), rect.GetTopRight(), color );
            DrawWorldLine( rect.GetTopLeft(), rect.GetBottomLeft(), color );
            DrawWorldLine( rect.GetBottomRight(), rect.GetTopRight(), color );
            DrawWorldLine( rect.GetBottomRight(), rect.GetBottomLeft(), color );
        }
    }

	void Renderer::EndRender()
	{
		mRenderCore->Flush( mCameras.top() );

		if ( mWindow )
		{
			mWindow->GetGLContext()->SwapBuffers();
		}
	}

	const RenderCore* Renderer::GetRenderCore() const
	{
		return mRenderCore;
	}

	RenderCore* Renderer::GetRenderCore()
	{
		return mRenderCore;
	}

	void Renderer::DrawLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color )
	{
        glm::vec2 lineverts[] =
        {
            start, 	// v0
            end		// v1
        };

        RenderCommand cmd;
        cmd.op               = RENDER_OP_PRIMITIVE;
        cmd.primmode         = PRIMITIVE_LINE;
        cmd.verts            = (PrimitiveVertex*) &lineverts[0];
        cmd.vertcount        = 2;
        cmd.flags            = RENDER_SCREEN_SPACE;
        cmd.color[0]         = color.x;
        cmd.color[1]         = color.y;
        cmd.color[2]         = color.z;
        cmd.color[3]         = color.w;
        mRenderCore->AddOrAppendCommand( cmd );
	}

	void Renderer::DrawAALine( const glm::vec2& start, const glm::vec2& end, float width, float feather, const glm::vec4& color )
	{
		glm::vec2 dir = glm::normalize( end - start );
		glm::vec2 norm1 = glm::vec2( -dir.y, dir.x );
		glm::vec2 norm2 = glm::vec2( dir.y, -dir.x );

        AALineVertex lineverts[] =
        {
			{ start.x, start.y, norm1.x, norm1.y },
			{ start.x, start.y, norm2.x, norm2.y },
			{ end.x, end.y, norm1.x, norm1.y },
			{ end.x, end.y, norm2.x, norm2.y }
        };

		RenderCommand cmd;
		cmd.op             	= RENDER_OP_AA_LINE;
		cmd.flags           = 0;
		cmd.lineverts		= &lineverts[0];
		cmd.linevertcount	= 4;
		cmd.linewidth 		= width;
		cmd.feather			= feather;
		cmd.linecolor[0]    = color.x;
		cmd.linecolor[1]    = color.y;
		cmd.linecolor[2]	= color.z;
		cmd.linecolor[3]	= color.w;
        mRenderCore->AddOrAppendCommand( cmd );
	}

	static double SignedArea( const glm::vec2& P1, const glm::vec2& P2, const glm::vec2& P3 )
	{
	    return (P2.x-P1.x)*(P3.y-P1.y) - (P3.x-P1.x)*(P2.y-P1.y);
	}

	static glm::vec2 RayIntersection( const glm::vec2& A1, glm::vec2 A2
		, const glm::vec2& B1, glm::vec2 B2 )
	{
		glm::vec2 D1 = glm::normalize( A2 - A1 );
		glm::vec2 D2 = glm::normalize( B2 - B1 );
		float u = (A1.y * D2.x + D2.y*B1.x - B1.y*D2.x - D2.y*A1.x ) / (D1.x*D2.y - D1.y*D2.x);
		return A1 + D1 * u;
	}

	static std::vector< glm::vec2 > BasicArc( const glm::vec2& P, float r, float dangle, float angle1, float angle2 )
	{
		float mod_diff = std::fmodf( angle2 - angle1, 2.0f * (float)M_PI );
		float signed_dist = mod_diff;
		if ( signed_dist > (float)M_PI )
			signed_dist -= 2.0f * (float)M_PI;
		else if ( signed_dist < (float)-M_PI )
			signed_dist += 2.0f * (float)M_PI;

		PROCYON_DEBUG( "PolyLine", "angle1 %f, angle2 %f, signed_dist %f"
	 		, angle1, angle2, signed_dist );

		angle2 = angle1 + signed_dist;
		bool incremental = true;
		if ( signed_dist < 0 ) {
		    incremental = false; //means decremental
		}

		std::vector< glm::vec2 > ret;
		if ( incremental) {
		    for ( float a=angle1; a < angle2; a+=dangle)
		    {
		        float x = cos(a);    float y = sin(a);
				ret.push_back( glm::vec2( P.x + x * r, P.y + y * r ) );
		    }
		} else {
		    for ( float a=angle1; a > angle2; a-=dangle)
		    {
		        float x=cos(a);    float y=sin(a);
				ret.push_back( glm::vec2( P.x+x*r,P.y + y * r ) );
		    }
		}
		return ret;
	}

	struct AnchorGeometry
	{
		void AddQuad( const glm::vec2& Q1, const glm::vec2& Q2, const glm::vec2& Q3
			, const glm::vec2& Q4, const glm::vec4& color )
		{
			ColorVertex temp[] =
			{
				{ Q1.x, Q1.y, color.x, color.y, color.z, color.w },
				{ Q2.x, Q2.y, color.x, color.y, color.z, color.w },
				{ Q3.x, Q3.y, color.x, color.y, color.z, color.w },
				{ Q1.x, Q1.y, color.x, color.y, color.z, color.w },
				{ Q4.x, Q4.y, color.x, color.y, color.z, color.w },
				{ Q3.x, Q3.y, color.x, color.y, color.z, color.w }
			};
			mVerts2.insert( mVerts2.end(), temp, std::end( temp ) );
		}

		void AddTri( const glm::vec2& Q1, const glm::vec2& Q2, const glm::vec2& Q3
		 	,  const glm::vec4& color )
		{
			ColorVertex temp[] =
			{
				{ Q1.x, Q1.y, color.x, color.y, color.z, color.w },
				{ Q2.x, Q2.y, color.x, color.y, color.z, color.w },
				{ Q3.x, Q3.y, color.x, color.y, color.z, color.w },
			};
			mVerts2.insert( mVerts2.end(), temp, std::end( temp ) );
		}

		void AddQuad( const glm::vec2& Q1, const glm::vec4& C1
			, const glm::vec2& Q2, const glm::vec4& C2
			, const glm::vec2& Q3, const glm::vec4& C3
			, const glm::vec2& Q4, const glm::vec4& C4 )
		{
			ColorVertex temp[] =
			{
				{ Q1.x, Q1.y, C1.x, C1.y, C1.z, C1.w },
				{ Q2.x, Q2.y, C2.x, C2.y, C2.z, C2.w },
				{ Q3.x, Q3.y, C3.x, C3.y, C3.z, C3.w },
				{ Q1.x, Q1.y, C1.x, C1.y, C1.z, C1.w },
				{ Q4.x, Q4.y, C4.x, C4.y, C4.z, C4.w },
				{ Q3.x, Q3.y, C3.x, C3.y, C3.z, C3.w }
			};
			mVerts2.insert( mVerts2.end(), temp, std::end( temp ) );
		}

		std::vector< glm::vec2 > mVerts;
		std::vector< glm::vec4 > mColors;
		std::vector< ColorVertex > mVerts2;
	};

	struct AnchorMetrics
	{
		glm::vec2 P[ 3 ]; 	// Anchor points
		float width, halfWidth;		// Line width
		glm::vec2 Tn[ 2 ]; 	// Normals to P1-P0 and P2-P1
		glm::vec2 T[ 2 ];	// Normals scaled to the inner half line width
		glm::vec2 To[ 2 ];	// Normals scaled to the half line width
		glm::vec2 vP, vPo;		//
		PolyLineJoinMode join;
		float miterLimit, feather;
		float outerWidth, outerHWidth;
		float innerWidth, innerHWidth;
	};

	static void ComputeAnchorMetrics( AnchorMetrics& m )
	{
		m.feather = glm::clamp( m.feather, 0.0f, 1.0f );

		m.outerWidth = m.width * m.feather;
		m.outerHWidth = m.outerWidth / 2.0f;
		m.innerWidth = m.width - m.width * m.feather;
		m.innerHWidth = m.innerWidth / 2.0f;
		m.halfWidth = m.width / 2.0f;

		glm::vec2 temp;
		temp = m.P[ 1 ] - m.P[ 0 ];
		m.Tn[ 0 ] = glm::normalize( glm::vec2( -temp.y, temp.x ) );
		temp = m.P[ 2 ] - m.P[ 1 ];
		m.Tn[ 1 ] = glm::normalize( glm::vec2( -temp.y, temp.x ) );
		if ( SignedArea( m.P[ 0 ], m.P[ 1 ], m.P[ 2 ] ) > 0.0 )
		{
			m.Tn[ 0 ] = -m.Tn[ 0 ];
			m.Tn[ 1 ] = -m.Tn[ 1 ];
		}
		m.T[ 0 ] = m.Tn[ 0 ] * m.innerHWidth;
		m.T[ 1 ] = m.Tn[ 1 ] * m.innerHWidth;
		m.To[ 0 ] = m.Tn[ 0 ] * m.halfWidth;
		m.To[ 1 ] = m.Tn[ 1 ] * m.halfWidth;

		glm::vec2 intersect = RayIntersection(
			m.T[ 0 ] + m.P[ 0 ], m.T[ 0 ] + m.P[ 1 ],
			m.T[ 1 ] + m.P[ 2 ], m.T[ 1 ] + m.P[ 1 ]
		);
		m.vP = intersect - m.P[ 1 ];

		glm::vec2 intersect2 = RayIntersection(
			m.To[ 0 ] + m.P[ 0 ], m.To[ 0 ] + m.P[ 1 ],
			m.To[ 1 ] + m.P[ 2 ], m.To[ 1 ] + m.P[ 1 ]
		);
		m.vPo = intersect2 - m.P[ 1 ];

		if( m.join == PolyLineJoinMode::MITER && glm::length( m.vP ) > ( m.miterLimit * 0.5f ) )
		{
			m.join = PolyLineJoinMode::BEVEL;
		}
	}

	static void TesselateAnchor( AnchorMetrics& m, AnchorGeometry& out, glm::vec4 color )
	{
		if ( m.join == PolyLineJoinMode::BEVEL )
		{
			out.AddQuad( m.T[ 0 ] + m.P[ 0 ], m.T[ 0 ] + m.P[ 1 ], m.P[ 1 ] - m.vP, m.P[ 0 ] - m.T[ 0 ], color );
			out.AddQuad( m.T[ 1 ] + m.P[ 2 ], m.T[ 1 ] + m.P[ 1 ], m.P[ 1 ] - m.vP, m.P[ 2 ] - m.T[ 1 ], color );
			out.AddTri( m.P[ 1 ] + m.T[ 0 ], m.P[ 1 ] + m.T[ 1 ], m.P[ 1 ] - m.vP, color );

			// Feather
			glm::vec4 border( color.x, color.y, color.z, 0.0f );
			if ( sDebugLines )
			{
				color = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
				border = glm::vec4( color.x, color.y, color.z, 1.0f );
			}
			out.AddQuad(
				m.To[ 0 ] + m.P[ 0 ], border,
				m.T[ 0 ] + m.P[ 0 ], color,
				m.T[ 0 ] + m.P[ 1 ], color,
				m.To[ 0 ] + m.P[ 1 ], border );
			out.AddQuad(
				-m.To[ 0 ] + m.P[ 0 ], border,
				-m.T[ 0 ] + m.P[ 0 ], color,
				m.P[ 1 ] - m.vP, color,
				m.P[ 1 ] - m.vPo, border );
			if ( sDebugLines )
			{
				color = glm::vec4( 0.0f, 01.0f, 0.0f, 1.0f );
				border = glm::vec4( color.x, color.y, color.z, border.w );
			}
			out.AddQuad(
				m.To[ 1 ] + m.P[ 2 ], border,
				m.T[ 1 ] + m.P[ 2 ], color,
				m.T[ 1 ] + m.P[ 1 ], color,
				m.To[ 1 ] + m.P[ 1 ], border );
			out.AddQuad(
				-m.To[ 1 ] + m.P[ 2 ], border,
				-m.T[ 1 ] + m.P[ 2 ], color,
				m.P[ 1 ] - m.vP, color,
				m.P[ 1 ] - m.vPo, border );

			if ( sDebugLines )
			{
				color = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
				border = glm::vec4( color.x, color.y, color.z, border.w );
			}
			out.AddQuad(
				m.To[ 0 ] + m.P[ 1 ], border,
				m.T[ 0 ] + m.P[ 1 ], color,
				m.T[ 1 ] + m.P[ 1 ], color,
				m.To[ 1 ] + m.P[ 1 ], border );

		}
		else if ( m.join == PolyLineJoinMode::MITER )
		{
			out.mVerts =
			{
				m.T[ 0 ] + m.P[ 0 ], m.T[ 0 ] + m.P[ 1 ], m.P[ 1 ] - m.vP,
				m.T[ 0 ] + m.P[ 0 ], m.P[ 1 ] - m.vP, m.P[ 0 ] - m.T[ 0 ],
				m.T[ 1 ] + m.P[ 2 ], m.T[ 1 ] + m.P[ 1 ], m.P[ 1 ] - m.vP,
				m.P[ 2 ] + m.T[ 1 ], m.P[ 1 ] - m.vP, m.P[ 2 ] - m.T[ 1 ],
				m.P[ 1 ] + m.T[ 0 ], m.P[ 1 ] + m.T[ 1 ], m.P[ 1 ] - m.vP,
				m.P[ 1 ] + m.T[ 0 ], m.P[ 1 ] + m.T[ 1 ], m.P[ 1 ] + m.vP,
			};
		}
		else if ( m.join == PolyLineJoinMode::ROUND )
		{
			out.mVerts =
			{
				m.T[ 0 ] + m.P[ 0 ], m.T[ 0 ] + m.P[ 1 ], m.P[ 1 ] - m.vP,
				m.T[ 0 ] + m.P[ 0 ], m.P[ 1 ] - m.vP, m.P[ 0 ] - m.T[ 0 ],
				m.T[ 1 ] + m.P[ 2 ], m.T[ 1 ] + m.P[ 1 ], m.P[ 1 ] - m.vP,
				m.P[ 2 ] + m.T[ 1 ], m.P[ 1 ] - m.vP, m.P[ 2 ] - m.T[ 1 ],
			};

			glm::vec2 jointInner = m.P[ 1 ] - m.vP;
			glm::vec2 arcStart = m.T[ 0 ] + m.P[ 1 ] - jointInner;
			glm::vec2 arcEnd = m.T[ 1 ] + m.P[ 1 ] - jointInner;
			float radius = glm::length( arcStart );
			float angle1 = acos( glm::normalize( arcStart ).x );
			float angle2 = acos( glm::normalize( arcEnd ).x );
			if ( arcStart.y < 0.0f ) { angle1 = 2.0f * (float)M_PI - angle1; }
			if ( arcEnd.y < 0.0f ) { angle2 = 2.0f * (float)M_PI - angle2; }
			PROCYON_DEBUG( "PolyLine", "Angle1 %f, Angle2 %f", 180.0f * angle1 / (float)M_PI, 180.0f * angle2 / (float)M_PI );
			std::vector< glm::vec2 > arc = BasicArc( jointInner, radius, (float)M_PI/36.0f, angle1, angle2 );
			PROCYON_DEBUG( "PolyLine", "arc Size %i", arc.size() );
			for ( size_t i = 0; i < arc.size(); ++i )
			{
				out.mVerts.push_back( jointInner );
				out.mVerts.push_back( arc[ i ] );

				if ( i + 1 == arc.size() )
				{
					out.mVerts.push_back( m.T[ 1 ] + m.P[ 1 ] );
				}
				else
				{
					out.mVerts.push_back( arc[ i + 1 ] );
				}
			}
		}
		else
		{
			assert( false );
		}
	}

	void Renderer::DrawAnchor( const glm::vec2* P, const glm::vec4& color, float width,
		PolyLineJoinMode joinMode, float miterLimit )
	{
		AnchorMetrics m;
		m.width = width;
		m.join = joinMode;
		m.miterLimit = miterLimit;
		m.feather = 0.5f;
		glm::vec4 color2 = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
		memcpy( &m.P, P, sizeof( glm::vec2 ) * 3 );
		ComputeAnchorMetrics( m );

		AnchorGeometry geometry;
		TesselateAnchor( m, geometry, color2 );

		PROCYON_DEBUG( "PolyLine", "DrawPolyLine <%f, %f> <%f, %f> <%f, %f>"
			, P[ 0 ].x, P[ 0 ].y
			, P[ 1 ].x, P[ 1 ].y
			, P[ 2 ].x, P[ 2 ].y );

		//PROCYON_DEBUG( "PolyLine", "T[ 0 ] <%f, %f> T[ 2 ] <%f, %f>", T[ 0 ].x, T[ 0 ].y, T[ 2 ].x, T[ 2 ].y );

		if ( true )
		{
			RenderCommand cmd;
	        cmd.op               = RENDER_OP_POLYGON;
	        cmd.flags            = RENDER_SCREEN_SPACE;
	        cmd.colorprimmode    = PRIMITIVE_TRIANGLE;
	        cmd.colorverts       = (ColorVertex*) geometry.mVerts2.data();
	        cmd.colorvertcount   = (int)geometry.mVerts2.size();
	        mRenderCore->AddOrAppendCommand( cmd );
		}
		else
		{
		    RenderCommand cmd;
	        cmd.flags			 = RENDER_SCREEN_SPACE;
			cmd.op 				 = RENDER_OP_PRIMITIVE;
			cmd.primmode 		 = PRIMITIVE_TRIANGLE;

			const size_t triCount = geometry.mVerts.size() / 3;
			for ( size_t i = 0; i < triCount; ++i )
			{
				cmd.verts = (PrimitiveVertex*)&geometry.mVerts[ i * 3 ];
				cmd.vertcount = 3;
		        cmd.color[0]  = color.x * (float)(i+1) / (float)triCount;
		        cmd.color[1]  = color.y * (float)(i+1) / (float)triCount;
		        cmd.color[2]  = color.z * (float)(i+1) / (float)triCount;
				cmd.color[3]  = 1.0f;
			    mRenderCore->AddOrAppendCommand( cmd );
			}
		}
 	}

	void Renderer::DrawPolyLine( const std::vector< glm::vec2 >& points, const glm::vec4& color
		, float width, PolyLineJoinMode joinMode, PolyLineCapMode capMode, float miterLimit /*= 1.0f */ )
	{
		if ( points.size() <= 2 )
		{
			PROCYON_WARN( "PolyLine", "must provide more than two points to draw DrawPolyLine");
			return;
		}

		glm::vec2 anchor[3];
		for ( size_t i = 0; i < points.size() - 2; ++i )
		{
			anchor[ 0 ] = points[ i ];
			if ( i != 0 )
			{
				anchor[ 0 ] += ( points[ i + 1 ] - points[ i ] ) / 2.0f;
			}
			anchor[ 1 ] = points[ i + 1 ];
			anchor[ 2 ] = points[ i + 2 ];
			if ( i + 3 != points.size() )
			{
				anchor[ 2 ] += ( points[ i + 1 ] - points[ i + 2 ] ) / 2.0f;
			}

			DrawAnchor( anchor, color, width, joinMode, miterLimit );
		}
	}

	void Renderer::DrawWorldLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color )
	{
        glm::vec2 lineverts[] =
        {
            start, 	// v0
            end		// v1
        };

        RenderCommand cmd;
        cmd.op               = RENDER_OP_PRIMITIVE;
        cmd.primmode         = PRIMITIVE_LINE;
        cmd.verts            = (PrimitiveVertex*) &lineverts[0];
        cmd.vertcount        = 2;
        cmd.flags            = 0;
        cmd.color[0]         = color.x;
        cmd.color[1]         = color.y;
        cmd.color[2]         = color.z;
        cmd.color[3]         = color.w;
        mRenderCore->AddOrAppendCommand( cmd );
	}


	void Renderer::DrawTexture( const Texture* tex, const glm::vec2& pos, const glm::vec2& dim, float orient, Rect textureRect /*= Rect() */ )
	{
        BatchedQuad quaddata;
        quaddata.position[0] = pos.x;
        quaddata.position[1] = pos.y;
        quaddata.size[0]     = dim.x;
        quaddata.size[1]     = dim.y;
        quaddata.rotation    = orient;
        quaddata.uvoffset[0] = textureRect.GetTopLeft().x;
        quaddata.uvoffset[1] = textureRect.GetTopLeft().y;
        quaddata.uvsize[0]   = textureRect.GetWidth();
        quaddata.uvsize[1]   = textureRect.GetHeight();
        quaddata.color[0]     = 1.0f;
        quaddata.color[1]     = 1.0f;
        quaddata.color[2]     = 1.0f;
		quaddata.color[3]     = 1.0f;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.flags            = 0;
        cmd.texture          = tex;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        mRenderCore->AddOrAppendCommand( cmd );
	}

    void Renderer::DrawFullscreenTexture( const Texture* tex )
    {
        BatchedQuad quaddata;
        quaddata.position[0] = 0.0f;
        quaddata.position[1] = 0.0f;
        quaddata.size[0]     = tex->Width();
        quaddata.size[1]     = tex->Height();
        quaddata.rotation    = 0.0f;
        quaddata.uvoffset[0] = 0.0f;
        quaddata.uvoffset[1] = 0.0f;
        quaddata.uvsize[0]   = 1.0f;
        quaddata.uvsize[1]   = 1.0f;
		quaddata.color[0]    = 1.0f;
		quaddata.color[1]    = 1.0f;
		quaddata.color[2]    = 1.0f;
		quaddata.color[3]    = 1.0f;;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.flags            = RENDER_SCREEN_SPACE;
        cmd.texture          = tex;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        mRenderCore->AddOrAppendCommand( cmd );

    }

	void Renderer::DrawRectShape( const glm::vec2& pos, const glm::vec2& dim, float orient, const glm::vec4& color )
	{
		BatchedQuad quaddata;
		quaddata.position[0] = pos.x;
		quaddata.position[1] = pos.y;
		quaddata.size[0]     = dim.x;
		quaddata.size[1]     = dim.y;
		quaddata.rotation    = orient;
		quaddata.uvoffset[0] = 0.0f;
		quaddata.uvoffset[1] = 0.0f;
		quaddata.uvsize[0]   = 0.0f;
		quaddata.uvsize[1]   = 0.0f;
		quaddata.color[0]     = color.x;
		quaddata.color[1]     = color.y;
		quaddata.color[2]     = color.z;
		quaddata.color[3]     = color.w;

		RenderCommand cmd;
		cmd.op               = RENDER_OP_QUAD;
		cmd.flags            = 0;
		cmd.texture          = NULL;
		cmd.instancecount    = 1;
		cmd.quaddata         = &quaddata;
		mRenderCore->AddOrAppendCommand( cmd );
	}

} /* namespace Procyon */
