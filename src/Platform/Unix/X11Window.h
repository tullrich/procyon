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

#ifndef _X11_WINDOW_H
#define _X11_WINDOW_H

#include "ProcyonCommon.h"
#include "Window.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

namespace Procyon {
namespace Unix {

	class X11GLContext;

	struct X11EventQueue
	{
		xcb_generic_event_t* prev = nullptr;
		xcb_generic_event_t* cur = nullptr;
		xcb_generic_event_t* next = nullptr;
	};

	class X11Window : public WindowBase
	{
	public:
						X11Window( const std::string& title, unsigned width, unsigned height );
		virtual 		~X11Window();

		virtual bool 	IsOpen();
		virtual void 	PollEvents();
		virtual void 	SetTitle( const std::string& title );
		virtual void 	SetIcon( const IImage& icon );
		virtual glm::ivec2 GetSize() const;

		Display* 		GetXDisplay();
		xcb_window_t 	GetXWindow();
		GL::IGLContext*	GetGLContext();

	protected:

		struct Modifiers
		{
			uint capslock;
			uint shiftlock;
			uint numlock;
			uint modeswitch;
			uint meta;
			uint alt;
			uint super;
			uint hyper;
			uint ctrl;

			Modifiers()
				: capslock( 0 )
				, shiftlock( 0 )
				, numlock( 0 )
				, modeswitch( 0 )
				, meta( 0 )
				, alt( 0 )
				, super( 0 )
				, hyper( 0 )
				, ctrl( 0 )
			{
			}
		};

		void 			Destroy();

		uint 			FindModifierMask( xcb_get_modifier_mapping_reply_t* modMapReply
							, xcb_keysym_t keysym );
		bool 			InitSymbolTable();
		bool 			InitWindow( const std::string& title, unsigned width, unsigned height );
		bool			InitWindowProtocol();

		xcb_keysym_t 	KeyCodeStateToKeySym( xcb_keycode_t code, uint16_t state );
		ProcyonKeyCode 	TranslateKeySym( KeySym sym );
		long 			KeySymToUnicode( KeySym sym );
		unsigned  		TranslateKeyState( uint state );
		MouseButton  	TranslateMouseButton( xcb_button_t state );

		bool				mIsOpen;
		Display*			mDisplay;
		xcb_connection_t*	mConnection;
		xcb_screen_t*		mScreen;
		xcb_window_t		mWindow;

		xcb_key_symbols_t*  mSymsTable;

		X11GLContext* 		mContext;

		Modifiers 			mModifiers;

		int					mWidth;
		int 				mHeight;

		X11EventQueue		mEventQueue;
	};

} /* namespace Unix */
} /* namespace Procyon */

#endif /* _X11_WINDOW_H */
