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
#ifndef _RENDER_CORE_H
#define _RENDER_CORE_H

#include "ProcyonCommon.h"

namespace Procyon {

	class Texture;
	class Camera2D;

	/*
	================
	Renderer

	Available RenderCommand operation types.
	================
	*/
	enum RenderCommandOp
	{
		RENDER_OP_QUAD,
		RENDER_OP_PRIMITIVE,
		RENDER_OP_POLYGON,
		RENDER_OP_AA_LINE
	};

	enum PrimitiveMode
	{
		PRIMITIVE_LINE,
		PRIMITIVE_TRIANGLE,
	};

	struct BatchedQuad
	{
		float position[2];
		float size[2];
		float rotation;
		float uvoffset[2];
		float uvsize[2];
		float color[4];
	};

	struct PrimitiveVertex
	{
		float position[2];
	};

	struct ColorVertex
	{
		float position[2];
		float color[4];
	};

	struct AALineVertex
	{
		float position[2];
		float normal[2];
	};

	enum RenderFlags
	{
		RENDER_SCREEN_SPACE = BIT( 0 )
	};

	/*
	================
	RenderCommand

	Encapsulates all state necessary to make a draw call by the renderer.
	These should be sorted to minimize state changes.
	================
	*/
	struct RenderCommand
	{
		RenderCommandOp op;

		int 					offset;
		char 					flags;

		union
		{
			struct // RENDER_OP_QUAD
			{
				const BatchedQuad* 		quaddata;
				int 					instancecount;
				const Texture*			texture;
			};

			struct // RENDER_OP_PRIMITIVE
			{
				const PrimitiveVertex* 	verts;
				int 					vertcount;
				PrimitiveMode			primmode;
				float					color[4]; // w: alpha
			};

			struct // RENDER_OP_POLYGON
			{
				const ColorVertex* 		colorverts;
				int 					colorvertcount;
				PrimitiveMode			colorprimmode;
			};

			struct// RENDER_OP_AA_LINE
			{
				const AALineVertex* 	lineverts;
				int 					linevertcount;
				float 					linewidth;
				float 					feather;
				float					linecolor[4]; // w: alpha
			};
		};

	};

	/*
	================
	RenderFrameStats

	Aggregated render statistics- use Renderer:GetFrameStats() to retrieve.
	================
	*/
	struct RenderFrameStats
	{
		int batches;
		int batchmax;
		int batchmin;
		int totalquads;
		int totalprimitives;
	};

	/*
	================
	Renderer
	================
	*/
	class RenderCore
	{
	public:
		virtual  						~RenderCore() { }

		virtual	void 					AddCommand( const RenderCommand& cmd ) = 0;
		virtual void 					AddOrAppendCommand( const RenderCommand& cmd ) = 0;
		virtual bool 					RenderCommandsPending() const = 0;
		virtual void 					Flush( const Camera2D& camera ) = 0;

		virtual void 					ResetStats() = 0;
		virtual const RenderFrameStats& GetFrameStats() const = 0;

		static RenderCore*				Allocate();
	};

} /* namespace Procyon */

#endif /* _RENDER_CORE_H */
