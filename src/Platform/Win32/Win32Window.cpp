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

#include "Win32Window.h"
#include "Win32GLContext.h"

#include <GLFW/glfw3.h>

namespace Procyon {
namespace Win32 {

#define GLFW_TRANSLATE_CASE(glfwkey, procyonkey)	\
	case glfwkey: return procyonkey;

	static ProcyonKeyCode TranslateGLFWKey( int key, int mods )
	{
		if ( mods & GLFW_MOD_SHIFT )
		{
			switch( key )
			{
				GLFW_TRANSLATE_CASE( GLFW_KEY_1, KEY_EXCLAM )
				GLFW_TRANSLATE_CASE( GLFW_KEY_2, KEY_AT )
				GLFW_TRANSLATE_CASE( GLFW_KEY_3, KEY_NUM_SIGN )
				GLFW_TRANSLATE_CASE( GLFW_KEY_4, KEY_DOLLAR )
				GLFW_TRANSLATE_CASE( GLFW_KEY_5, KEY_PERCENT )
				GLFW_TRANSLATE_CASE( GLFW_KEY_7, KEY_AMPERSAND )
				GLFW_TRANSLATE_CASE( GLFW_KEY_8, KEY_ASTERISK )
				GLFW_TRANSLATE_CASE( GLFW_KEY_9, KEY_PAREN_LEFT )
				GLFW_TRANSLATE_CASE( GLFW_KEY_0, KEY_PAREN_RIGHT )
				GLFW_TRANSLATE_CASE( GLFW_KEY_MINUS, KEY_UNDERSCORE )
				GLFW_TRANSLATE_CASE( GLFW_KEY_EQUAL, KEY_PLUS )
				GLFW_TRANSLATE_CASE( GLFW_KEY_SEMICOLON, KEY_COLON )
				GLFW_TRANSLATE_CASE( GLFW_KEY_APOSTROPHE, KEY_QUOTE_DBL )
				GLFW_TRANSLATE_CASE( GLFW_KEY_COMMA, KEY_LESS )
				GLFW_TRANSLATE_CASE( GLFW_KEY_PERIOD, KEY_GREATER )
				GLFW_TRANSLATE_CASE( GLFW_KEY_SLASH, KEY_QUESTION )
				GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT_BRACKET, KEY_BRACE_LEFT )
				GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT_BRACKET, KEY_BRACE_RIGHT )
				GLFW_TRANSLATE_CASE( GLFW_KEY_BACKSLASH, KEY_BAR )
				GLFW_TRANSLATE_CASE( GLFW_KEY_GRAVE_ACCENT, KEY_TILDE )
				//GLFW_TRANSLATE_CASE( KEY_CIRCUM )
			}
		}

		switch ( key )
		{
			GLFW_TRANSLATE_CASE( GLFW_KEY_0, KEY_0 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_1, KEY_1 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_2, KEY_2 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_3, KEY_3 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_4, KEY_4 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_5, KEY_5 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_6, KEY_6 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_7, KEY_7 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_8, KEY_8 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_9, KEY_9 )

			GLFW_TRANSLATE_CASE( GLFW_KEY_A, KEY_A )
			GLFW_TRANSLATE_CASE( GLFW_KEY_B, KEY_B )
			GLFW_TRANSLATE_CASE( GLFW_KEY_C, KEY_C )
			GLFW_TRANSLATE_CASE( GLFW_KEY_D, KEY_D )
			GLFW_TRANSLATE_CASE( GLFW_KEY_E, KEY_E )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F, KEY_F )
			GLFW_TRANSLATE_CASE( GLFW_KEY_G, KEY_G )
			GLFW_TRANSLATE_CASE( GLFW_KEY_H, KEY_H )
			GLFW_TRANSLATE_CASE( GLFW_KEY_I, KEY_I )
			GLFW_TRANSLATE_CASE( GLFW_KEY_J, KEY_J )
			GLFW_TRANSLATE_CASE( GLFW_KEY_K, KEY_K )
			GLFW_TRANSLATE_CASE( GLFW_KEY_L, KEY_L )
			GLFW_TRANSLATE_CASE( GLFW_KEY_M, KEY_M )
			GLFW_TRANSLATE_CASE( GLFW_KEY_N, KEY_N )
			GLFW_TRANSLATE_CASE( GLFW_KEY_O, KEY_O )
			GLFW_TRANSLATE_CASE( GLFW_KEY_P, KEY_P )
			GLFW_TRANSLATE_CASE( GLFW_KEY_Q, KEY_Q )
			GLFW_TRANSLATE_CASE( GLFW_KEY_R, KEY_R )
			GLFW_TRANSLATE_CASE( GLFW_KEY_S, KEY_S )
			GLFW_TRANSLATE_CASE( GLFW_KEY_T, KEY_T )
			GLFW_TRANSLATE_CASE( GLFW_KEY_U, KEY_U )
			GLFW_TRANSLATE_CASE( GLFW_KEY_V, KEY_V )
			GLFW_TRANSLATE_CASE( GLFW_KEY_W, KEY_W )
			GLFW_TRANSLATE_CASE( GLFW_KEY_X, KEY_X )
			GLFW_TRANSLATE_CASE( GLFW_KEY_Y, KEY_Y )
			GLFW_TRANSLATE_CASE( GLFW_KEY_Z, KEY_Z )

			GLFW_TRANSLATE_CASE( GLFW_KEY_SPACE, KEY_SPACE )
			GLFW_TRANSLATE_CASE( GLFW_KEY_APOSTROPHE, KEY_APOSTROPHE )
			GLFW_TRANSLATE_CASE( GLFW_KEY_COMMA, KEY_COMMA )
			GLFW_TRANSLATE_CASE( GLFW_KEY_MINUS, KEY_MINUS )
			GLFW_TRANSLATE_CASE( GLFW_KEY_PERIOD, KEY_PERIOD )
			GLFW_TRANSLATE_CASE( GLFW_KEY_SLASH, KEY_SLASH )
			GLFW_TRANSLATE_CASE( GLFW_KEY_SEMICOLON, KEY_SEMICOLON )
			GLFW_TRANSLATE_CASE( GLFW_KEY_EQUAL, KEY_EQUAL )
			GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT_BRACKET, KEY_BRACKET_LEFT )
			GLFW_TRANSLATE_CASE( GLFW_KEY_BACKSLASH, KEY_BACK_SLASH )
			GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT_BRACKET, KEY_BRACKET_RIGHT )
			GLFW_TRANSLATE_CASE( GLFW_KEY_GRAVE_ACCENT, KEY_GRAVE )

			GLFW_TRANSLATE_CASE( GLFW_KEY_F1, KEY_F1 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F2, KEY_F2 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F3, KEY_F3 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F4, KEY_F4 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F5, KEY_F5 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F6, KEY_F6 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F7, KEY_F7 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F8, KEY_F8 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F9, KEY_F9 )
			GLFW_TRANSLATE_CASE( GLFW_KEY_F10, KEY_F10 )

			GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT_SHIFT, KEY_SHIFT_L )
			GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT_SHIFT, KEY_SHIFT_R )
			GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT_CONTROL, KEY_CTRL_L )
			GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT_CONTROL, KEY_CTRL_R )
			GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT_ALT, KEY_ALT_L )
			GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT_ALT, KEY_ALT_R )
			GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT_SUPER, KEY_SUPER_L )
			GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT_SUPER, KEY_SUPER_R )
			GLFW_TRANSLATE_CASE( GLFW_KEY_CAPS_LOCK, KEY_CAPS_LOCK )
			//GLFW_TRANSLATE_CASE( KEY_META_L )
			//GLFW_TRANSLATE_CASE( KEY_META_R )

			GLFW_TRANSLATE_CASE( GLFW_KEY_UP, KEY_UP )
			GLFW_TRANSLATE_CASE( GLFW_KEY_DOWN, KEY_DOWN )
			GLFW_TRANSLATE_CASE( GLFW_KEY_LEFT, KEY_LEFT )
			GLFW_TRANSLATE_CASE( GLFW_KEY_RIGHT, KEY_RIGHT )

			GLFW_TRANSLATE_CASE( GLFW_KEY_BACKSPACE, KEY_BACKSPACE )
			GLFW_TRANSLATE_CASE( GLFW_KEY_TAB, KEY_TAB )
			GLFW_TRANSLATE_CASE( GLFW_KEY_ENTER, KEY_RETURN )
			GLFW_TRANSLATE_CASE( GLFW_KEY_PAUSE, KEY_PAUSE )
			GLFW_TRANSLATE_CASE( GLFW_KEY_ESCAPE, KEY_ESCAPE )
			GLFW_TRANSLATE_CASE( GLFW_KEY_DELETE, KEY_DELETE )

			default: return KEY_UNKNOWN;
		}
	}

	static KeyEventModifiers TranslateGLFWModifiers( int mods )
	{
		int procMods = 0;
		if ( mods & GLFW_MOD_SHIFT ) procMods |= MODIFIER_SHIFT;
		if ( mods & GLFW_MOD_CONTROL ) procMods |= MODIFIER_CTRL;
		if ( mods & GLFW_MOD_ALT ) procMods |= MODIFIER_ALT;;
		if ( mods & GLFW_MOD_SUPER ) procMods |= MODIFIER_SUPER;
		return ( KeyEventModifiers)procMods;
	}

	static MouseButton TranslateGLFWMouseButton( int button )
	{
		switch ( button )
		{
			case GLFW_MOUSE_BUTTON_LEFT: return MOUSE_BTN_LEFT;
			case GLFW_MOUSE_BUTTON_RIGHT: return MOUSE_BTN_RIGHT;
			case GLFW_MOUSE_BUTTON_MIDDLE: return MOUSE_BTN_MIDDLE;
			case GLFW_MOUSE_BUTTON_4: return MOUSE_BTN_FORWARD;
			case GLFW_MOUSE_BUTTON_5: return MOUSE_BTN_BACK;
			case GLFW_MOUSE_BUTTON_6: return MOUSE_BTN_UNKNOWN;
			case GLFW_MOUSE_BUTTON_7: return MOUSE_BTN_UNKNOWN;
			case GLFW_MOUSE_BUTTON_8: return MOUSE_BTN_UNKNOWN;
			default: return MOUSE_BTN_UNKNOWN;
		}
	}

	static void GLFWKeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
        if ( !listener )
            return;

		switch ( action )
		{
			case GLFW_PRESS:
			{
				InputEvent ievent( EVENT_KEY_DOWN );
				ievent.scancode = (unsigned)scancode;
				ievent.keysym = TranslateGLFWKey( key, mods );
				ievent.modifiers = TranslateGLFWModifiers( mods );
				listener->HandleInputEvent( ievent );
				break;
			}
			case GLFW_RELEASE:
			{
				InputEvent ievent( EVENT_KEY_UP );
                ievent.scancode = ( unsigned )scancode;
                ievent.keysym = TranslateGLFWKey( key, mods );
                ievent.modifiers = TranslateGLFWModifiers( mods );
                listener->HandleInputEvent( ievent );
				break;
			}
			case GLFW_REPEAT:
			{
				break;
			}
		}
	}

	static void GLFWUnicodeCallback( GLFWwindow* window, unsigned int codepoint, int mods )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
		if ( !listener )
			return;

		InputEvent ievent( EVENT_TEXT );
		ievent.unicode = codepoint;
		listener->HandleInputEvent( ievent );
	}

	static void GLFWCursorPositionCallback( GLFWwindow* window, double xpos, double ypos )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
		if ( !listener )
			return;

		InputEvent ievent( EVENT_MOUSE_MOVE );
		ievent.mousebutton 		= MOUSE_BTN_UNKNOWN;
		ievent.rawx 			= (int)xpos;
		ievent.rawy 			= (int)ypos;
		ievent.mousex			= (float)xpos;
		ievent.mousey 			= (float)ypos;
		listener->HandleInputEvent( ievent );
	}

	static void GLFWMouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
		if ( !listener )
			return;

		switch ( action )
		{
			case GLFW_PRESS:
			{
				InputEvent ievent( EVENT_MOUSE_DOWN );
		        ievent.mousebutton = TranslateGLFWMouseButton( button );
				listener->HandleInputEvent( ievent );
				break;
			}
			case GLFW_RELEASE:
			{
				InputEvent ievent( EVENT_MOUSE_UP );
		        ievent.mousebutton = TranslateGLFWMouseButton( button );
                listener->HandleInputEvent( ievent );
				break;
			}
		}
	}

	static void GLFWScrollCallback( GLFWwindow* window, double xoffset, double yoffset )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
		if ( !listener )
			return;

		InputEvent ievent( EVENT_MOUSE_DOWN );
		ievent.mousebutton = ( yoffset > 0 ) ? MOUSE_SCROLL_UP : MOUSE_SCROLL_DOWN;
		ievent.rawx = 0;
		ievent.rawy = 0;
		ievent.mousex = (float)xoffset;
		ievent.mousey = (float)yoffset;
		listener->HandleInputEvent( ievent );
	}

	static void GLFWWindowSizeCallback( GLFWwindow* window, int width, int height )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
		if ( !listener )
			return;

		InputEvent ievent( EVENT_WINDOW_CHANGED );
		glfwGetWindowPos(window, &ievent.windowx, &ievent.windowy);
		ievent.height = height;
		ievent.width = width;
		listener->HandleInputEvent( ievent );
	}

	static void GLFWWindowCloseCallback( GLFWwindow* window )
	{
		//window closing
	}

	static void GLFWWindowPosCallback( GLFWwindow* window, int xpos, int ypos )
	{
		Win32Window* procyonWin = reinterpret_cast< Win32Window* >( glfwGetWindowUserPointer( window ) );
		if ( !procyonWin )
			return;

		IInputEventListener* listener = procyonWin->GetEventListener();
		if ( !listener )
			return;

		InputEvent ievent( EVENT_WINDOW_CHANGED );
		glfwGetWindowSize(window, &ievent.width, &ievent.height);
		ievent.windowx = xpos;
		ievent.windowy = ypos;
		listener->HandleInputEvent( ievent );

	}

	Win32Window::Win32Window( const std::string& title, unsigned height, unsigned width )
		: mWindow( nullptr )
		, mContext( nullptr )
	{
		/* Initialize the library */
		if ( !glfwInit() )
		{
			glfwTerminate();
		}

		/* Create a windowed mode window and its OpenGL context */
		mWindow = glfwCreateWindow( height, width, title.c_str(), NULL, NULL );
		if ( !mWindow )
		{
			glfwTerminate();
		}

		glfwSetWindowUserPointer( mWindow, this );
		glfwSetKeyCallback( mWindow, GLFWKeyCallback );
        glfwSetCharModsCallback( mWindow, GLFWUnicodeCallback );
		glfwSetCursorPosCallback( mWindow, GLFWCursorPositionCallback );
		glfwSetMouseButtonCallback( mWindow, GLFWMouseButtonCallback );
		glfwSetScrollCallback( mWindow, GLFWScrollCallback );
		glfwSetWindowSizeCallback( mWindow, GLFWWindowSizeCallback );
		glfwSetWindowCloseCallback( mWindow, GLFWWindowCloseCallback );
		glfwSetWindowPosCallback( mWindow, GLFWWindowPosCallback);
	}

	Win32Window::~Win32Window()
	{
		if ( mContext )
		{
			delete mContext;
		}
		glfwDestroyWindow( mWindow );
		glfwTerminate();
	}

	bool Win32Window::IsOpen()
	{
		return !glfwWindowShouldClose( mWindow );
	}

	void Win32Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Win32Window::SetTitle( const std::string& title )
	{
	}

	void Win32Window::SetIcon( const IImage& icon )
	{
	}

	GL::IGLContext* Win32Window::GetGLContext()
	{
		if( !mContext )
		{
			mContext = new Win32GLContext( this );
		}

		return mContext;
	}

} /* namespace Win32 */
} /* namespace Procyon */

namespace Procyon {

	IWindow* Window_Create( const std::string& title, unsigned height, unsigned width )
	{
		return new Win32::Win32Window( title, height, width );
	}

} /* namespace Procyon */
