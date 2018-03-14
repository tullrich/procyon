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
#ifndef _GL_RENDER_CORE_H
#define _GL_RENDER_CORE_H

#include "ProcyonGL.h"
#include "Graphics/RenderCore.h"

#define MAX_RENDER_CMDS 10002

namespace Procyon {

namespace GL {

	class GLBuffer;
	class GLProgram;

	class GLRenderCore : public RenderCore
	{
	public:
										GLRenderCore();
		virtual  						~GLRenderCore();

		virtual void 					AddCommand( const RenderCommand& cmd );
		virtual void 					AddOrAppendCommand( const RenderCommand& cmd );

		virtual bool 					RenderCommandsPending() const;

		virtual void 					Flush( const Camera2D& camera );

		virtual void 					ResetStats();
		virtual const RenderFrameStats& GetFrameStats() const;

	protected:
		bool 				PushData( const unsigned char* data, int size );
		bool 				PushCommandData( const RenderCommand& cmd );

		void 				RenderQuadBatch( const RenderCommand& rc, const Camera2D& camera );
		void 				RenderPrimitive( const RenderCommand& rc, const Camera2D& camera );
		void 				RenderPolygon( const RenderCommand& rc, const Camera2D& camera );
		void 				RenderAntiAliasedLine( const RenderCommand& rc, const Camera2D& camera  );


		// The buffer of render commands- cleared each frame.
		RenderCommand 		mCmdBuffer[ MAX_RENDER_CMDS ];

		// Current size of mCmdBuffer.
		int 				mRenderCommandCount;

		RenderFrameStats	mFrameStats;

		GLBuffer* 			mBuffer;
		GLBuffer* 			mOffBuffer;

		GLBuffer* 			mQuadBuffer;
		GLBuffer* 			mQuadIndices;

		GLProgram* 			mDefaultProg;
		GLProgram* 			mDefaultPrimitiveProg;
		GLProgram* 			mDefaultPolygonProg;
		GLProgram* 			mDefaultLineProg;
	};

} // namespace GL

} // namespace Procyon

#endif // _GL_RENDER_CORE_H
