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

#include "X11Window.h"
#include "X11GLContext.h"
#include "Image.h"

#include <X11/Xlib-xcb.h> /* for XGetXCBConnection, link with libX11-xcb */
#include <X11/keysym.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_image.h>

namespace Procyon {
namespace Unix {

struct XCBError
{
	XCBError( xcb_connection_t* con, xcb_void_cookie_t cookie )
		: mError( NULL )
		, mConnection( con )
	{
		mError = xcb_request_check( mConnection, cookie );
	}

	~XCBError()
	{
        free( mError );
	}

	operator bool() const
	{
		mError != NULL;
	}

	int Code() const
	{
		return mError->error_code;
	}

	xcb_generic_error_t* 	mError;
	xcb_connection_t*		mConnection;
};

X11Window::X11Window( const std::string& title, unsigned width, unsigned height )
	: mIsOpen( false )
	, mDisplay( NULL )
	, mConnection( NULL )
	, mScreen( NULL )
	, mWindow( 0 )
	, mSymsTable( NULL )
	, mContext( NULL )
	, mWidth( width )
	, mHeight( height )
{
	// Open Xlib Display
	mDisplay = XOpenDisplay( 0 );
	if( !mDisplay )
	{
		PROCYON_ERROR( "X11", "Unable to open display." );
    	throw std::runtime_error("X11Window");
	}

	// Open XCB Connection
    mConnection = XGetXCBConnection( mDisplay );
  	if( !mConnection )
    {
    	Destroy();
		PROCYON_ERROR( "X11", "Unable to open xcb connection" );
    	throw std::runtime_error("X11Window");
    }
    XSetEventQueueOwner( mDisplay, XCBOwnsEventQueue );

    if ( !InitSymbolTable() )
    {
    	Destroy();
		PROCYON_ERROR( "X11", "Unable to setup x symbol table." );
    	throw std::runtime_error("X11Window");
    }

    // Grab the first screen
    mScreen = xcb_setup_roots_iterator( xcb_get_setup( mConnection ) ).data;

    // Open the window
    if( !InitWindow( title, width, height ) )
    {
    	Destroy();
    	throw std::runtime_error("X11Window");
    }

	mIsOpen = true;
}

X11Window::~X11Window()
{
    Destroy();
}

void X11Window::Destroy()
{
	xcb_flush( mConnection );

	if( mContext )
	{
		delete mContext;
		mContext = NULL;
	}

	if( mWindow )
	{
		xcb_destroy_window( mConnection, mWindow );
		mWindow = 0;
	}

	if ( mSymsTable )
	{
		xcb_key_symbols_free( mSymsTable );
		mSymsTable = NULL;
	}

	if( mDisplay )
	{
   		XCloseDisplay( mDisplay );
   		mScreen = NULL;
   		mConnection = NULL; // connection is automatically shutdown
   		mDisplay = NULL;
	}
}

uint X11Window::FindModifierMask( xcb_get_modifier_mapping_reply_t* modMapReply, xcb_keysym_t keysym )
{
    xcb_keycode_t *modMap = xcb_get_modifier_mapping_keycodes( modMapReply );

	xcb_keycode_t* search = xcb_key_symbols_get_keycode( mSymsTable, keysym );
    const int w = modMapReply->keycodes_per_modifier;
    for (int bit = 0; bit < 8; ++bit)
    {
        uint mask = 1 << bit;
	    for (int x = 0; x < w; ++x)
	    {
	    	xcb_keycode_t keyCode = modMap[x + bit * w];
			if ( search && keyCode == *search )
			{
				return mask;
			}
	    }
	}
	free( search );
	return 0; // not bound;
}

bool X11Window::InitSymbolTable()
{
    mSymsTable = xcb_key_symbols_alloc( mConnection );

	xcb_get_modifier_mapping_cookie_t modMapCookie = xcb_get_modifier_mapping( mConnection );

	xcb_generic_error_t *error = 0;
    xcb_get_modifier_mapping_reply_t *modMapReply 
    	= xcb_get_modifier_mapping_reply( mConnection, modMapCookie, &error);
	
    if (error) 
    {
        free(error);
        return false;
    }

	mModifiers.capslock 	= FindModifierMask( modMapReply, XK_Caps_Lock );
	mModifiers.shiftlock 	= FindModifierMask( modMapReply, XK_Shift_Lock );
	mModifiers.numlock 		= FindModifierMask( modMapReply, XK_Num_Lock );
	mModifiers.modeswitch 	= FindModifierMask( modMapReply, XK_Mode_switch );

	mModifiers.meta 		= FindModifierMask( modMapReply, XK_Meta_L ); // X11 cannot distinguish
	if( !mModifiers.meta )
	{
		mModifiers.meta 	= FindModifierMask( modMapReply, XK_Meta_R );
	}

	mModifiers.alt 			= FindModifierMask( modMapReply, XK_Alt_L ); // X11 cannot distinguish
	if( !mModifiers.alt )
	{
		mModifiers.alt 		= FindModifierMask( modMapReply, XK_Alt_R );
	}

	mModifiers.super 		= FindModifierMask( modMapReply, XK_Super_L ); // X11 cannot distinguish
	if( !mModifiers.super )
	{
		mModifiers.super 	= FindModifierMask( modMapReply, XK_Super_R );
	}

	mModifiers.hyper 		= FindModifierMask( modMapReply, XK_Hyper_L ); // X11 cannot distinguish
	if( !mModifiers.hyper )
	{
		mModifiers.hyper 	= FindModifierMask( modMapReply, XK_Hyper_R );
	}

	mModifiers.ctrl 		= FindModifierMask( modMapReply, XK_Control_L ); // X11 cannot distinguish
	if( !mModifiers.ctrl )
	{
		mModifiers.ctrl 	= FindModifierMask( modMapReply, XK_Control_R );
	}

	PROCYON_DEBUG( "X11", "Left Ctrl %i", FindModifierMask( modMapReply, XK_Control_L ) );
	PROCYON_DEBUG( "X11", "Right Ctrl %i", FindModifierMask( modMapReply, XK_Control_R ) );

    return true;
}

bool X11Window::InitWindow( const std::string& title, unsigned width, unsigned height )
{
    uint32_t eventmask =
    	XCB_EVENT_MASK_KEY_PRESS |			// keyboard down
    	XCB_EVENT_MASK_KEY_RELEASE |		// keyboard up
    	XCB_EVENT_MASK_BUTTON_PRESS |		// mouse down
    	XCB_EVENT_MASK_BUTTON_RELEASE | 	// mouse up
    	XCB_EVENT_MASK_POINTER_MOTION |		// mouse motion
    	XCB_EVENT_MASK_STRUCTURE_NOTIFY; 	// window resize
    uint32_t valuelist[] = { eventmask, 0 };
    uint32_t valuemask = XCB_CW_EVENT_MASK;

    mWindow = xcb_generate_id( mConnection );

	XCBError error( mConnection, 
		xcb_create_window_checked( mConnection
		, XCB_COPY_FROM_PARENT
		, mWindow
		, mScreen->root
		, 0, 0
		, width, height
		, 0
		, XCB_WINDOW_CLASS_INPUT_OUTPUT
		, mScreen->root_visual
		, valuemask
		, valuelist
	));

	if( error.mError != NULL )
    {
		PROCYON_ERROR( "X11", "Unable to create xcb window, error code: %i.", error.Code() );
        return false;
    }

    if( !InitWindowProtocol() )
    {
		PROCYON_ERROR( "X11", "Unable to setup X protocol." );
    	return false;
    }

    SetTitle( title );

    xcb_map_window( mConnection, mWindow ); // show

	xcb_flush( mConnection );

	return true;
}

bool X11Window::InitWindowProtocol()
{
	xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(mConnection
		, xcb_intern_atom(mConnection, 1, 12, "WM_PROTOCOLS"), 0);

	xcb_intern_atom_reply_t* reply2 = xcb_intern_atom_reply(mConnection
		, xcb_intern_atom(mConnection, 0, 16, "WM_DELETE_WINDOW"), 0);

	xcb_change_property(mConnection, XCB_PROP_MODE_REPLACE, mWindow
		, (*reply).atom, 4, 32, 1, &(*reply2).atom);

	return true;
}


bool X11Window::IsOpen()
{
	return mIsOpen;
}

// Logic found at http://tronche.com/gui/x/xlib/input/keyboard-encoding.html
xcb_keysym_t X11Window::KeyCodeStateToKeySym( xcb_keycode_t code, uint16_t state )
{
	xcb_keysym_t ks1;
	xcb_keysym_t ks2;

	if ( !(state & mModifiers.modeswitch) )
	{
		ks1 = xcb_key_symbols_get_keysym( mSymsTable, code, 0 );
		ks2 = xcb_key_symbols_get_keysym( mSymsTable, code, 1 );
	}
	else
	{
		ks1 = xcb_key_symbols_get_keysym( mSymsTable, code, 2 );
		ks2 = xcb_key_symbols_get_keysym( mSymsTable, code, 3 );
	}
	 

	if ( (state & mModifiers.numlock) && xcb_is_keypad_key( code ) )
	{
		PROCYON_DEBUG( "X11", "Case 1" );
		if( (state & XCB_MOD_MASK_SHIFT) || (state & mModifiers.shiftlock))
		{
			return ks1;
		}
		else
		{
			return ks2;
		}
	}
	else if ( !(state & (XCB_MOD_MASK_SHIFT | XCB_MOD_MASK_LOCK) ) )
	{
		PROCYON_DEBUG( "X11", "Case 2" );
		return ks1;
	}
	else if ( !(state & XCB_MOD_MASK_SHIFT) && (state & mModifiers.capslock) )
	{
		PROCYON_DEBUG( "X11", "Case 3" );

		KeySym lower, upper;
		XConvertCase( ks1, &lower, &upper );

		return upper;
	}
	else if ( (state & XCB_MOD_MASK_SHIFT) && (state & mModifiers.capslock) )
	{
		PROCYON_DEBUG( "X11", "Case 4" );

		KeySym lower, upper;
		XConvertCase( ks2, &lower, &upper );

		return upper;
	}
	else if ( ( state & XCB_MOD_MASK_SHIFT) || (state & mModifiers.shiftlock) )
	{
		PROCYON_DEBUG( "X11", "Case 5" );
		return ks2;
	}

	PROCYON_DEBUG( "X11", "Case XCB_NO_SYMBOL" );
	return XCB_NO_SYMBOL;
}

bool X11Window::Poll( InputEvent& ievent )
{
	xcb_generic_event_t *event;

	bool ret = false;

	if ( (event = xcb_poll_for_event (mConnection) ) ) 
	{

	    ievent = InputEvent();

	    switch ( event->response_type & ~0x80 )
	    {
	    case XCB_KEY_PRESS: // key down
	    {
	        xcb_key_press_event_t *press = (xcb_key_press_event_t *)event;
	        xcb_keysym_t keysym = xcb_key_symbols_get_keysym( mSymsTable, press->detail, 0 );

	        if( keysym != XCB_NO_SYMBOL && !xcb_is_modifier_key( press->detail ) )
	        {
	        	xcb_keysym_t modifiedKeysym = KeyCodeStateToKeySym( press->detail, press->state );

	        	ievent = InputEvent( EVENT_KEY_DOWN );
	        	ievent.keycode 		= (unsigned)press->detail;
	        	ievent.keysym 		= TranslateKeySym( keysym );
	        	ievent.unicode 		= KeySymToUnicode( modifiedKeysym );
	        	ievent.modifiers 	= TranslateKeyState( press->state );
	        	ret = true;

				PROCYON_DEBUG( "X11", "Got Sym: %s %i %i", XKeysymToString( keysym )
					, press->detail,  press->state );
	        }

			PROCYON_DEBUG( "X11", "XCB_KEY_PRESS" );
	        break;

	    }
	    case XCB_KEY_RELEASE: // key up
	    {
	        xcb_key_release_event_t *release = (xcb_key_release_event_t *)event;
	        xcb_keysym_t keysym = xcb_key_symbols_get_keysym( mSymsTable, release->detail, 0 );

	        if( keysym != XCB_NO_SYMBOL && !xcb_is_modifier_key( release->detail ) )
	        {
	        	xcb_keysym_t modifiedKeysym = KeyCodeStateToKeySym( release->detail, release->state );

	        	ievent = InputEvent( EVENT_KEY_UP );
	        	ievent.keycode 		= (unsigned)release->detail;
	        	ievent.keysym 		= TranslateKeySym( keysym );
	        	ievent.unicode 		= KeySymToUnicode( modifiedKeysym );
	        	ievent.modifiers 	= TranslateKeyState( release->state );
	        	ret = true;

				PROCYON_DEBUG( "X11", "Got Sym: %s %i %i", XKeysymToString( keysym )
					, release->detail,  release->state );
	        }

			PROCYON_DEBUG( "X11", "XCB_KEY_RELEASE" );
	        break;

	    }
	    case XCB_BUTTON_PRESS: // mouse down
	    {
	        xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;

	        ievent = InputEvent( EVENT_MOUSE_DOWN );
	        ievent.mousebutton 		= TranslateMouseButton( press->detail );
	        ievent.rawx 			= press->event_x;
	        ievent.rawy 			= press->event_y;
	        ievent.mousex			= press->event_x / (float)mWidth;
	        ievent.mousey 			= press->event_y / (float)mHeight;
	        ret = true;

			PROCYON_DEBUG( "X11", "XCB_BUTTON_PRESS %i", press->detail );
	        break;
	    }
	    case XCB_BUTTON_RELEASE: // mouse up
	    {
	        xcb_button_release_event_t *release = (xcb_button_release_event_t *)event;

	        ievent = InputEvent( EVENT_MOUSE_UP );
	        ievent.mousebutton 		= TranslateMouseButton( release->detail );
	        ievent.rawx 			= release->event_x;
	        ievent.rawy 			= release->event_y;
	        ievent.mousex			= release->event_x / (float)mWidth;
	        ievent.mousey 			= release->event_y / (float)mHeight;
	        ret = true;

			PROCYON_DEBUG( "X11", "XCB_BUTTON_RELEASE" );
	        break;
	    }
	    case XCB_MOTION_NOTIFY: // mouse motion
	    {
	        xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;

	        ievent = InputEvent( EVENT_MOUSE_MOVE );
	        ievent.mousebutton 		= TranslateMouseButton( motion->detail );
	        ievent.rawx 			= motion->event_x;
	        ievent.rawy 			= motion->event_y;
	        ievent.mousex			= motion->event_x / (float)mWidth;
	        ievent.mousey 			= motion->event_y / (float)mHeight;
	        ret = true;

			//PROCYON_DEBUG( "X11", "XCB_MOTION_NOTIFY" );
	    	break;
	    }
        case XCB_CLIENT_MESSAGE: // close
        {
            xcb_client_message_event_t* e = reinterpret_cast<xcb_client_message_event_t*>(event);
			PROCYON_DEBUG( "X11", "XCB_CLIENT_MESSAGE" );
			mIsOpen = false;
            break;
        }
        case XCB_CONFIGURE_NOTIFY: // resize
        {
            xcb_configure_notify_event_t* e = reinterpret_cast<xcb_configure_notify_event_t*>(event);

            mWidth = e->width;
            mHeight = e->height;

	        ievent = InputEvent( EVENT_WINDOW_CHANGED );
	        ievent.windowx = e->x;
	        ievent.windowy = e->y;
	        ievent.width = e->width;
	        ievent.height = e->height;
	        ret = true;

			PROCYON_DEBUG( "X11", "XCB_CONFIGURE_NOTIFY" );
            break;
        }
        case XCB_DELETE_PROPERTY:
        case XCB_LIST_PROPERTIES:
        case XCB_GET_KEYBOARD_CONTROL:
        {
        	break; // These come down the pipe occasionally.
        }
	    default:

			PROCYON_DEBUG( "X11", "Unknown xcb event (%i).", event->response_type & ~0x80 );
	        break;
	    }

	    free( event );
	}

	return ret;
}

void X11Window::SetTitle( const std::string& title )
{
	if ( !mConnection || !mWindow )
		return;

	// Set the window name  property.
	xcb_icccm_set_wm_name( mConnection
		, mWindow
		, XCB_ATOM_STRING
		, 8
		, title.size()
		, title.c_str() );

	// Seems Gnome displays the class/instance names for certain things, so we need to set it too.
	// X11 expects the class + instance name in a contiguous c-string separated by a nullchar.
	std::string wmClass = title; 	// Class name is set to title for now (probably wrong).
	wmClass.push_back( '\0' );
	wmClass += title; 				// as is the instance name.
	xcb_icccm_set_wm_class( mConnection
		, mWindow
		, wmClass.size()
		, wmClass.c_str() );
}

void X11Window::SetIcon( const IImage& icon )
{
	if ( !mConnection || !mScreen || !mWindow )
		return;

	PROCYON_DEBUG( "X11", "SetIcon with image Width: %i Height: %i Components: %i"
		, icon.Width(), icon.Height(), icon.Components() );

	MemoryImage tempImg( icon, 4 ); // convert to 4 components
	uint8_t* tempPixels = tempImg.MutableData();
	for ( int x = 0; x < icon.Width() * icon.Height(); x++ )
	{
		// RGBA -> BGRA
		std::swap( tempPixels[ x * 4 + 0 ], tempPixels[ x * 4 + 2 ] );
	}

	xcb_pixmap_t pixmap = xcb_generate_id( mConnection );
	XCBError error( mConnection,
		xcb_create_pixmap_checked( mConnection
			, mScreen->root_depth
			, pixmap
			, mWindow
			, icon.Width()
			, icon.Height() )
	);

	if( error.mError != NULL )
    {
		PROCYON_WARN( "X11", "Failed to set window icon- unable to create pixmap, error code: %i"
			, error.Code() );
        return;
    }

    xcb_gcontext_t gc = xcb_generate_id( mConnection );
    xcb_create_gc( mConnection, gc, pixmap, 0, NULL);

    xcb_put_image_checked( mConnection
    	, XCB_IMAGE_FORMAT_Z_PIXMAP
    	, pixmap
    	, gc
    	, icon.Width()
    	, icon.Height()
    	, 0
    	, 0
    	, 0
    	, mScreen->root_depth
    	, icon.Width() * icon.Height() * 4
    	, tempPixels );

	xcb_icccm_wm_hints_t wmHints;
	xcb_icccm_wm_hints_set_none( &wmHints );
	xcb_icccm_wm_hints_set_icon_pixmap( &wmHints, pixmap );
	xcb_icccm_set_wm_hints( mConnection, mWindow, &wmHints );

	xcb_free_gc( mConnection, gc );
}

Display* X11Window::GetXDisplay()
{
	return mDisplay;
}

xcb_window_t X11Window::GetXWindow()
{
	return mWindow;
}

X11GLContext* X11Window::GetGLContext()
{
	if( !mContext )
	{
		mContext = new X11GLContext( this );
	}

	return mContext;
}

#define X11_TRANSLATE_CASE(x11key, procyonkey)	\
	case x11key: return procyonkey;

ProcyonKeyCode X11Window::TranslateKeySym( KeySym sym )
{
	KeySym lower, upper;
	XConvertCase( sym, &lower, &upper );

	switch( lower )
	{
		// Numeric keys
		X11_TRANSLATE_CASE( XK_0, KEY_0 )
		X11_TRANSLATE_CASE( XK_1, KEY_1 )
		X11_TRANSLATE_CASE( XK_2, KEY_2 )
		X11_TRANSLATE_CASE( XK_3, KEY_3 )
		X11_TRANSLATE_CASE( XK_4, KEY_4 )
		X11_TRANSLATE_CASE( XK_5, KEY_5 )
		X11_TRANSLATE_CASE( XK_6, KEY_6 )
		X11_TRANSLATE_CASE( XK_7, KEY_7 )
		X11_TRANSLATE_CASE( XK_8, KEY_8 )
		X11_TRANSLATE_CASE( XK_9, KEY_9 )

		// Alphabetical keys
		X11_TRANSLATE_CASE( XK_a, KEY_A )
		X11_TRANSLATE_CASE( XK_b, KEY_B )
		X11_TRANSLATE_CASE( XK_c, KEY_C )
		X11_TRANSLATE_CASE( XK_d, KEY_D )
		X11_TRANSLATE_CASE( XK_e, KEY_E )
		X11_TRANSLATE_CASE( XK_f, KEY_F )
		X11_TRANSLATE_CASE( XK_g, KEY_G )
		X11_TRANSLATE_CASE( XK_h, KEY_H )
		X11_TRANSLATE_CASE( XK_i, KEY_I )
		X11_TRANSLATE_CASE( XK_j, KEY_J )
		X11_TRANSLATE_CASE( XK_k, KEY_K )
		X11_TRANSLATE_CASE( XK_l, KEY_L )
		X11_TRANSLATE_CASE( XK_m, KEY_M )
		X11_TRANSLATE_CASE( XK_n, KEY_N )
		X11_TRANSLATE_CASE( XK_o, KEY_O )
		X11_TRANSLATE_CASE( XK_p, KEY_P )
		X11_TRANSLATE_CASE( XK_q, KEY_Q )
		X11_TRANSLATE_CASE( XK_r, KEY_R )
		X11_TRANSLATE_CASE( XK_s, KEY_S )
		X11_TRANSLATE_CASE( XK_t, KEY_T )
		X11_TRANSLATE_CASE( XK_u, KEY_U )
		X11_TRANSLATE_CASE( XK_v, KEY_V )
		X11_TRANSLATE_CASE( XK_w, KEY_W )
		X11_TRANSLATE_CASE( XK_x, KEY_X )
		X11_TRANSLATE_CASE( XK_y, KEY_Y )
		X11_TRANSLATE_CASE( XK_z, KEY_Z )

		// Misc Latin keys
		X11_TRANSLATE_CASE( XK_space, KEY_SPACE )
		X11_TRANSLATE_CASE( XK_exclam, KEY_EXCLAM )
		X11_TRANSLATE_CASE( XK_quotedbl, KEY_QUOTE_DBL )
		X11_TRANSLATE_CASE( XK_numbersign, KEY_NUM_SIGN )
		X11_TRANSLATE_CASE( XK_dollar, KEY_DOLLAR )
		X11_TRANSLATE_CASE( XK_percent, KEY_PERCENT )
		X11_TRANSLATE_CASE( XK_ampersand, KEY_AMPERSAND )
		X11_TRANSLATE_CASE( XK_apostrophe, KEY_APOSTROPHE )
		X11_TRANSLATE_CASE( XK_parenleft, KEY_PAREN_LEFT )
		X11_TRANSLATE_CASE( XK_parenright, KEY_PAREN_RIGHT )
		X11_TRANSLATE_CASE( XK_asterisk, KEY_ASTERISK )
		X11_TRANSLATE_CASE( XK_plus, KEY_PLUS )
		X11_TRANSLATE_CASE( XK_comma, KEY_COMMA )
		X11_TRANSLATE_CASE( XK_minus, KEY_MINUS )
		X11_TRANSLATE_CASE( XK_period, KEY_PERIOD )
		X11_TRANSLATE_CASE( XK_slash, KEY_SLASH )
		X11_TRANSLATE_CASE( XK_colon, KEY_COLON )
		X11_TRANSLATE_CASE( XK_semicolon, KEY_SEMICOLON )
		X11_TRANSLATE_CASE( XK_less, KEY_LESS )
		X11_TRANSLATE_CASE( XK_equal, KEY_EQUAL )
		X11_TRANSLATE_CASE( XK_greater, KEY_GREATER )
		X11_TRANSLATE_CASE( XK_question, KEY_QUESTION)
		X11_TRANSLATE_CASE( XK_at, KEY_AT )
		X11_TRANSLATE_CASE( XK_bracketleft, KEY_BRACKET_LEFT )
		X11_TRANSLATE_CASE( XK_backslash, KEY_BACK_SLASH )
		X11_TRANSLATE_CASE( XK_bracketright, KEY_BRACKET_RIGHT )
		X11_TRANSLATE_CASE( XK_asciicircum, KEY_CIRCUM )
		X11_TRANSLATE_CASE( XK_underscore, KEY_UNDERSCORE )
		X11_TRANSLATE_CASE( XK_grave, KEY_GRAVE )
		X11_TRANSLATE_CASE( XK_braceleft, KEY_BRACE_LEFT )
		X11_TRANSLATE_CASE( XK_bar, KEY_BAR )
		X11_TRANSLATE_CASE( XK_braceright, KEY_BRACE_RIGHT )
		X11_TRANSLATE_CASE( XK_asciitilde, KEY_TILDE )

		// Function keys
		X11_TRANSLATE_CASE( XK_F1, KEY_F1 )
		X11_TRANSLATE_CASE( XK_F2, KEY_F2 )
		X11_TRANSLATE_CASE( XK_F3, KEY_F3 )
		X11_TRANSLATE_CASE( XK_F4, KEY_F4 )
		X11_TRANSLATE_CASE( XK_F5, KEY_F5 )
		X11_TRANSLATE_CASE( XK_F6, KEY_F6 )
		X11_TRANSLATE_CASE( XK_F7, KEY_F7 )
		X11_TRANSLATE_CASE( XK_F8, KEY_F8 )
		X11_TRANSLATE_CASE( XK_F9, KEY_F9 )
		X11_TRANSLATE_CASE( XK_F10, KEY_F10 )

		// Arrow keys
		X11_TRANSLATE_CASE( XK_Up, KEY_UP )
		X11_TRANSLATE_CASE( XK_Down, KEY_DOWN )
		X11_TRANSLATE_CASE( XK_Left, KEY_LEFT )
		X11_TRANSLATE_CASE( XK_Right, KEY_RIGHT )

		// Modifier keys
		X11_TRANSLATE_CASE( XK_Shift_L, KEY_SHIFT_L )
		X11_TRANSLATE_CASE( XK_Shift_R, KEY_SHIFT_R )
		X11_TRANSLATE_CASE( XK_Control_L, KEY_CTRL_L )
		X11_TRANSLATE_CASE( XK_Control_R, KEY_CTRL_R )
		X11_TRANSLATE_CASE( XK_Meta_L, KEY_META_L )
		X11_TRANSLATE_CASE( XK_Meta_R, KEY_META_R )
		X11_TRANSLATE_CASE( XK_Alt_L, KEY_ALT_L )
		X11_TRANSLATE_CASE( XK_Alt_R, KEY_ALT_R )
		X11_TRANSLATE_CASE( XK_Super_L, KEY_SUPER_L)
		X11_TRANSLATE_CASE( XK_Super_R, KEY_SUPER_R )
		X11_TRANSLATE_CASE( XK_Caps_Lock, KEY_CAPS_LOCK )

		// Misc keys
		X11_TRANSLATE_CASE( XK_BackSpace, KEY_BACKSPACE )
		X11_TRANSLATE_CASE( XK_Tab, KEY_TAB )
		X11_TRANSLATE_CASE( XK_Return, KEY_RETURN )
		X11_TRANSLATE_CASE( XK_Pause, KEY_PAUSE )
		X11_TRANSLATE_CASE( XK_Escape, KEY_ESCAPE )
		X11_TRANSLATE_CASE( XK_Delete, KEY_DELETE )
		default: return KEY_UNKNOWN;
	}
}

long X11Window::KeySymToUnicode( KeySym sym )
{
	// Only supporting Latin-1 keysym -> unicode conversion for now,
	// otherwise this gets way too complex.
    if ((sym > 0x001f && sym <= 0x007e) ||
        (sym >= 0x00a0 && sym <= 0x00ff))
        return sym;

    // Ascii control characters that are not easily mapped.
    switch (sym)
    {
    	case 65293: return 0x000a; // Return
    	case 65289: return 0x0009; // Tab
    	case 65307: return 0x001b; // Escape
    	case 65288: return 0x0008; // Backspace
    	default: return -1; // no match
    }
}

unsigned X11Window::TranslateKeyState( uint state )
{
	unsigned modifier = 0;
	modifier |= (state & mModifiers.capslock) ? MODIFIER_CAPS : 0;
	modifier |= (state & mModifiers.shiftlock) ? MODIFIER_SHIFT : 0;
	modifier |= (state & mModifiers.meta) ? MODIFIER_META : 0;
	modifier |= (state & mModifiers.alt) ? MODIFIER_ALT : 0;
	modifier |= (state & mModifiers.super) ? MODIFIER_SUPER : 0;
	modifier |= (state & mModifiers.hyper) ? MODIFIER_HYPER : 0;
	modifier |= (state & mModifiers.ctrl) ? MODIFIER_CTRL : 0;
	return modifier;
}

MouseButton X11Window::TranslateMouseButton( xcb_button_t state )
{
	switch( state )
	{
		case 1: return MOUSE_BTN_LEFT;
		case 2: return MOUSE_BTN_MIDDLE;
		case 3: return MOUSE_BTN_RIGHT;
		case 4: return MOUSE_SCROLL_UP;
		case 5: return MOUSE_SCROLL_DOWN;
		case 6: return MOUSE_SCROLL_LEFT;
		case 7: return MOUSE_SCROLL_RIGHT;
		case 8: return MOUSE_BTN_FORWARD;
		case 9: return MOUSE_BTN_BACK;
		default: return MOUSE_BTN_UNKNOWN;
	}
}

} /* namespace Unix */
} /* namespace Procyon */
