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

#ifndef _WIN32_WINDOW_H
#define _WIN32_WINDOW_H

#include "ProcyonCommon.h"
#include "Platform/Window.h"

struct GLFWwindow;

namespace Procyon {
namespace Win32 {

	class Win32GLContext;

	class Win32Window : public WindowBase
	{
	public:
						Win32Window( const std::string& title, unsigned width, unsigned height );
		virtual 		~Win32Window();

		virtual bool IsOpen();
		virtual void PollEvents();
		virtual void SetTitle( const std::string& title );
		virtual void SetIcon( const IImage& icon );
		virtual GL::IGLContext* GetGLContext();
		virtual void* GetNativeHandle() const;
		virtual glm::ivec2 GetSize() const;

		GLFWwindow* GetInternal() { return mWindow; }

	protected:
		GLFWwindow* mWindow;
		Win32GLContext* mContext;
	};

} /* namespace Win32 */
} /* namespace Procyon */

#endif /* _WIN32_WINDOW_H */
