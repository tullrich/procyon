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
#include "GLTexture.h"

namespace Procyon {

	Renderer::Renderer()
		: mRenderCore( new RenderCore() )
	{
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

	void Renderer::BeginRender()
	{
		mRenderCore->ResetStats();
	}

	void Renderer::Draw( const Renderable* r )
	{
		r->PostRenderCommands( this, mRenderCore );
	}

    void Renderer::DrawWireframeRect( const Rect& rect, const glm::vec4& color )
    {
        DrawLine( rect.GetTopLeft(), rect.GetTopRight(), color );
        DrawLine( rect.GetTopLeft(), rect.GetBottomLeft(), color );
        DrawLine( rect.GetBottomRight(), rect.GetTopRight(), color );
        DrawLine( rect.GetBottomRight(), rect.GetBottomLeft(), color );
    }

	void Renderer::EndRender()
	{
		mRenderCore->Flush( mCameras.top() );
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
        cmd.program          = NULL;
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

	void Renderer::DrawWorldLine( const glm::vec2& start, const glm::vec2& end, const glm::vec4& color )
	{
        glm::vec2 lineverts[] =
        {
            start, 	// v0
            end		// v1
        };

        RenderCommand cmd;
        cmd.op               = RENDER_OP_PRIMITIVE;
        cmd.program          = NULL;
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


	void Renderer::DrawTexture( const GL::GLTexture* tex, const glm::vec2& pos, const glm::vec2& dim, float orient, struct Rect textureRect /*= Rect() */ )
	{
        BatchedQuad quaddata;
        quaddata.position[0] = pos.x;
        quaddata.position[1] = pos.y;
        quaddata.size[0]     = dim.x;
        quaddata.size[1]     = dim.y;
        quaddata.rotation    = orient;
        quaddata.uvoffset[0] = 0.0f;
        quaddata.uvoffset[1] = 0.0f;
        quaddata.uvsize[0]   = 1.0f;
        quaddata.uvsize[1]   = 1.0f;
        quaddata.tint[0]     = 0.0f;
        quaddata.tint[1]     = 0.0f;
        quaddata.tint[2]     = 0.0f;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.program          = NULL;
        cmd.flags            = 0;
        cmd.texture          = tex;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        mRenderCore->AddOrAppendCommand( cmd );
	}

    void Renderer::DrawFullscreenTexture( const GL::GLTexture* tex )
    {
        const Camera2D & cam = GetCamera();

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
        quaddata.tint[0]     = 0.0f;
        quaddata.tint[1]     = 0.0f;
        quaddata.tint[2]     = 0.0f;

        RenderCommand cmd;
        cmd.op               = RENDER_OP_QUAD;
        cmd.program        = NULL;
        cmd.flags            = RENDER_SCREEN_SPACE;
        cmd.texture          = tex;
        cmd.instancecount    = 1;
        cmd.quaddata         = &quaddata;
        mRenderCore->AddOrAppendCommand( cmd );

    }

} /* namespace Procyon */
