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
#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "ProcyonCommon.h"

namespace Procyon {

	struct InputEvent;
	class Renderer;
	class Camera2D;

	void Console_Init();
	void Console_Destroy();

	bool Console_IsOpen();
	void Console_SetOpen( bool open );
	void Console_Toggle();

	void Console_Process( FrameTime t );
	void Console_Render( Renderer* renderer );

	void Console_PrintLine( const std::string& msg, const glm::vec4& rgb );
	void Console_PrintLine( const std::string& msg );
	void Console_Clear();
	bool Console_Execute( const std::string& cmd );

	bool Console_HandleEvent( const InputEvent& ev );

} /* namespace Procyon */

#endif /* _CONSOLE_H */
