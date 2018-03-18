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
#include "PolyLine.h"
#include "Platform/Mouse.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderCore.h"
#include "Graphics/GL/ProcyonVaser.h"
#include "Player.h"

using namespace Procyon;

std::vector< glm::highp_vec2 > sPoints;
int sPointsStart = 0;
std::vector< glm::highp_vec2 > sPoints2;

VASEr::polyline_opt sOpts;
VASEr::tessellator_opt sTessOpts;

PolyLine::PolyLine()
{
	memset( &sOpts, 0, sizeof( VASEr::polyline_opt ) );
	memset( &sTessOpts, 0, sizeof( VASEr::tessellator_opt ) );

	sOpts.tess = &sTessOpts;
	sTessOpts.tessellate_only = true;
}

void PolyLine::Process( FrameTime t, Player* player)
{
	if ( Mouse::OnButtonDown( MOUSE_BTN_LEFT ) )
	{
		sPoints2.push_back( Mouse::GetPosition() );
	}

	static const float screen_x = 500.0f;
	static const int maxSamples = 200;
	static const float x_scale = screen_x / (float)maxSamples;
	static float accum = 0.0f;
	static float sampleHz = 1.0f / 15.0f;
	accum += t.dt;
	while (accum >= sampleHz)
	{
		accum -= sampleHz;
		float x = sPointsStart % maxSamples * x_scale - screen_x / 2.0f;
		float y = sin(sPointsStart * sampleHz) * 100.0f;

		if (sPoints.size() != maxSamples)
		{
			sPoints.push_back(player->GetPosition());
		}
		else
		{
			sPoints[sPointsStart % maxSamples] = player->GetPosition();
		}
		sPointsStart++;
	}
}

void PolyLine::Draw( Renderer* r ) const
{
	if (false && sPoints.size() >= 4)
	{
		sOpts.cap = VASEr::PLC_round;
		sOpts.feather = true;
		sOpts.feathering = 1.0;
		VASEr::VASErin::vertex_array_holder holder;
		sTessOpts.holder = &holder;

		const double weight = 3.0;
		const VaseColor color = GlmToVaseColor(glm::vec4(245/255.0f, 121/255.0f, 79/255.0f, 1.0f));
		//VASEr::segment( GlmToVaseVec2( sPoints[ 0 ] ), GlmToVaseVec2( sPoints[ 1 ] ), color, weight, &sOpts);

		std::vector< VaseVec2 > stuff(sPoints.size());
		std::vector< VaseColor > stuffColor(sPoints.size());
		for (size_t i = 0; i < sPoints.size(); i++)
		{
			PROCYON_DEBUG("PolyLine", "sPoints[ %i ] <%f, %f>", i, sPoints[i].x, sPoints[i].y);
			VaseVec2& d = stuff[i];
			d.x = sPoints[i].x;
			d.y = sPoints[i].y;

			VaseColor& c = stuffColor[i];
			c.r = (sPoints[i].x + 250.0f) / 250.0f;
			c.g = (sPoints[i].y + 250.0f) / 250.0f;
			c.b = 0.0;
			c.a = 0.6f;
		}
		//VASEr::polyline( stuff.data(), stuffColor.data(), weight, stuff.size(), &sOpts );

		VASEr::polybezier_opt bazOpts;
		bazOpts.poly = &sOpts;
		VASEr::polybezier(stuff.data(), color, weight, (int)stuff.size(), &bazOpts);

		if (holder.glmode == GL_TRIANGLES)
		{
			PROCYON_DEBUG("PolyLine", "vertex_array_holder.vert %i mode: Triangles", holder.vert.size());

			assert(holder.vert.size()/2 == holder.color.size()/4);
			std::vector< ColorVertex > native(holder.vert.size()/2);
			for (int i = 0; i < holder.vert.size()/2; i++)
			{
				ColorVertex& vert = native[i];
				vert.position[0] = holder.vert[i * 2];
				vert.position[1] = holder.vert[i * 2 + 1];
				vert.color[0] = holder.color[i * 4];
				vert.color[1] = holder.color[i * 4 + 1];
				vert.color[2] = holder.color[i * 4 + 2];
				vert.color[3] = holder.color[i * 4 + 3];
			}

			RenderCommand cmd;
			cmd.op               = RENDER_OP_POLYGON;
			cmd.flags            = 0;
			cmd.colorprimmode    = PRIMITIVE_TRIANGLE;
			cmd.colorverts       = (ColorVertex*)native.data();
			cmd.colorvertcount   = (int)native.size();
			r->GetRenderCore()->AddOrAppendCommand(cmd);
		}
		else
		{
			const char* mode = NULL;
			switch (holder.glmode)
			{
			case GL_TRIANGLES: mode = "Triangles"; break;
			case GL_TRIANGLE_STRIP: mode = "Triangle Strip"; break;
			default: mode = "Other"; break;
			}
			PROCYON_DEBUG("PolyLine", "Not Triangles: vertex_array_holder.vert %i mode: %s", holder.vert.size(), mode);
		}
	}
}

void PolyLine::SetEndpoint( const glm::vec2& point ) 
{
	if (sPoints2.size() > 0 )
	{
		sPoints2[sPoints2.size() - 1] = point;
	}
}