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

#ifndef _WINDOW_H
#define _WINDOW_H

#include "ProcyonCommon.h"
#include "KeyCodes.h"

namespace Procyon {

	class IImage;

	namespace GL
	{
		class IGLContext;
	}

	const char* ProcyonKeyCodeToStr( ProcyonKeyCode kc );

	enum InputEventType
	{
		EVENT_UNKNOWN,
		EVENT_KEY_DOWN,
		EVENT_KEY_UP,
		EVENT_TEXT,
		EVENT_MOUSE_DOWN,
		EVENT_MOUSE_UP,
		EVENT_MOUSE_MOVE,
		EVENT_WINDOW_CHANGED,
		EVENT_TYPE_COUNT
	};

	// Note that X11 implementation cannot distinguish between L and R
	enum KeyEventModifiers
	{
		MODIFIER_L_SHIFT 	= BIT( 1 ),
		MODIFIER_R_SHIFT 	= BIT( 2 ),
		MODIFIER_SHIFT 		= MODIFIER_L_SHIFT | MODIFIER_R_SHIFT,

		MODIFIER_L_CTRL		= BIT( 3 ),
		MODIFIER_R_CTRL		= BIT( 4 ),
		MODIFIER_CTRL 		= MODIFIER_L_CTRL | MODIFIER_R_CTRL,

		MODIFIER_L_ALT		= BIT( 5 ),
		MODIFIER_R_ALT		= BIT( 6 ),
		MODIFIER_ALT 		= MODIFIER_L_ALT | MODIFIER_R_ALT,

		MODIFIER_L_META	 	= BIT( 7 ),
		MODIFIER_R_META 	= BIT( 8 ),
		MODIFIER_META 		= MODIFIER_L_META | MODIFIER_R_META,

		MODIFIER_L_SUPER 	= BIT( 9 ),
		MODIFIER_R_SUPER 	= BIT( 10 ),
		MODIFIER_SUPER 		= MODIFIER_L_SUPER | MODIFIER_R_SUPER,

		MODIFIER_L_HYPER 	= BIT( 11 ),
		MODIFIER_R_HYPER 	= BIT( 12 ),
		MODIFIER_HYPER 		= MODIFIER_L_HYPER | MODIFIER_R_HYPER,

		MODIFIER_CAPS 		= BIT( 13 )
	};

	enum MouseButton
	{
		MOUSE_BTN_UNKNOWN,
		MOUSE_BTN_LEFT,
		MOUSE_BTN_RIGHT,
		MOUSE_BTN_MIDDLE,
		MOUSE_BTN_FORWARD,
		MOUSE_BTN_BACK,
		MOUSE_SCROLL_UP,
		MOUSE_SCROLL_DOWN,
		MOUSE_SCROLL_LEFT,
		MOUSE_SCROLL_RIGHT
	};

	struct InputEvent
	{
		InputEventType type;

		union
		{
			struct // EVENT_KEY_DOWN and EVENT_KEY_UP
			{
				unsigned		scancode; // platform scancode
				ProcyonKeyCode	keysym;
				unsigned		modifiers;
			};
			struct // EVENT_TEXT
			{
				long 			unicode;
			};
			struct // EVENT_MOUSE_DOWN, EVENT_MOUSE_UP, and EVENT_MOUSE_MOVE
			{
				MouseButton	mousebutton;
				int 		rawx;
				int 		rawy;
				float 		mousex;
				float 		mousey;
			};
			struct // EVENT_WINDOW_CHANGED
			{
				int windowx;
				int windowy;
				int height;
				int width;
			};
		};

		InputEvent()
			: type( EVENT_UNKNOWN )
		{
		}

		InputEvent( InputEventType t )
			: type( t )
		{
		}
	};

	class IInputEventListener
	{
	public:
        virtual void HandleInputEvent( const InputEvent& ev ) = 0;
	};

	class IWindow
	{
	public:
		virtual ~IWindow() { };

		virtual bool IsOpen() = 0;
		virtual void SetEventListener( IInputEventListener* listener ) = 0;
		virtual void PollEvents() = 0;
		virtual void SetTitle( const std::string& title ) = 0;
		virtual void SetIcon( const IImage& icon ) = 0;
		virtual GL::IGLContext* GetGLContext() = 0;
	};

	class WindowBase : public IWindow
	{
	public:
		WindowBase();

		virtual void SetEventListener( IInputEventListener* listener );
		IInputEventListener* GetEventListener();
	protected:
		IInputEventListener* mListener;
	};

	IWindow* Window_Create( const std::string& title, unsigned height, unsigned width );
} /* namespace Procyon */

#endif /* _WINDOW_H */
