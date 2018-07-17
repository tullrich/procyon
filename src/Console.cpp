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
#include "Console.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shape.h"
#include "Graphics/Camera.h"
#include "Graphics/FontFace.h"
#include "Graphics/Text.h"
#include "Platform/Window.h"

using namespace Procyon::GL;

/*
===================

Console.cpp

Static console implementation used by the engine for debug io. The console will soon be made to execute registered commands from all over the engine via Console_RegisterCommand and Console_Execute().

This implementation is also capable of rendering a debug overlay using Console_Open(), Console_Close(), and Console_Render(); by default triggered via '~'.

===================
*/


/*
================
Console style vars
================
*/

// x resolution of the console window
#define CONSOLE_X_RES 960.0f

// y resolution of the console window
#define CONSOLE_Y_RES 270.0f

// font file loaded by the console
//#define CONSOLE_FONT_FILE "fonts/OpenSans-Regular.ttf"
//#define CONSOLE_FONT_FILE "fonts/DejaVuSans.ttf"
#define CONSOLE_FONT_FILE "fonts/DejaVuSansMono.ttf"
//#define CONSOLE_FONT_FILE "fonts/arial.ttf"

//#define CONSOLE_FONT_FILE "fonts/Aladin-Regular.ttf"
//#define CONSOLE_FONT_FILE "fonts/nulshock bd.ttf"
//#define CONSOLE_FONT_FILE "fonts/PassionOne-Regular.ttf"
//#define CONSOLE_FONT_FILE "fonts/Economica-Regular.ttf"
//#define CONSOLE_FONT_FILE "fonts/Jurassic Park.ttf"
//#define CONSOLE_FONT_FILE "fonts/PoiretOne-Regular.ttf"

// console font height in pixels
#define CONSOLE_FONT_HEIGHT 11

// prompt text
#define PROMPT_TEXT "$ >"

// right-padding amount applied to the prompt text
#define PROMPT_PADDING 7.0f

// cursor bar blink period in seconds
#define CURSOR_BLINK_PERIOD 1.0f

// rate that the console will slide open in seconds
#define OPEN_TIME 0.1f

// outer border size
#define CONSOLE_BORDER_SIZE 2.0f

// inner border text padding
#define CONSOLE_INNER_PADDING 2.0f

// prompt text color
static const glm::vec4 sPromptColor( 0.4f, 1.0f, 0.4f, 1.0f );

// error text color
static const glm::vec4 sErrorColor( 1.0f, 0.7f, 0.7f, 1.0f );

// system text color
static const glm::vec4 sSystemColor( 0.75f, 0.75f, 0.75f, 1.0f );

// standard text print color
static const glm::vec4 sTextColor( 1.0f );

/*
================
Computed vars
================
*/

// number of visible lines of text not including the prompt line
#define VISIBLE_LINES (int)(( CONSOLE_Y_RES - CONSOLE_FONT_HEIGHT) / CONSOLE_FONT_HEIGHT)

// total padding from console edge to the text start/end
#define TEXT_PADDING ( CONSOLE_BORDER_SIZE + CONSOLE_INNER_PADDING )

namespace Procyon {

    // initialized state of the console
    static bool 			sConsoleInitialized = false;

    // open state of the console
    static bool				sConsoleOpen 		= false;
	static std::string 		sLastInput;

    // console Renderables
    static Shape* 			sBackground			= NULL;
    static Shape* 			sBackground2		= NULL;
    static Text* 			sPromptText 		= NULL;
    static Text* 			sInputText			= NULL;

    // renderables storing the visible history
    static Text* 			sHistory[ VISIBLE_LINES ];

    // camera used for view transformation during rendering
    static Camera2D* 		sConsoleCamera 		= NULL;

    // font used for rendering
    static FontFace* 		sConsoleFont 		= NULL;

    // the next element of sHistory to be used for output.
    static int 				sNextHistoryLine 	= 0;

    // controls the visibility of the cursor (> 0 represents visible)
    static float 			sBlinkTimer 		= 0.0f;

    /*
    ================
    HeightForRow

    Returns the y position (in the coordinate system of sConsoleCamera) to position an sHistory element when representing the provided row.
    ================
    */
    static float HeightForRow( int row )
    {
        // -1 indicates prompt line
        const FontMetrics metrics = sConsoleFont->GetMetrics( CONSOLE_FONT_HEIGHT );
        return TEXT_PADDING + metrics.line_height * (row+1);
    }

    /*
    ================
    InitBackground

    Inits the sBackground and sBackground2 renderables.
    ================
    */
    static void InitBackground()
    {
        // the colored border on all edges except the top
        sBackground = new Shape();
        sBackground->SetColor( glm::vec4( 0.4f, 0.4, 0.4, 1.0f) );
        sBackground->SetPosition( 0.0f, CONSOLE_Y_RES );
        sBackground->SetScale( CONSOLE_X_RES, -CONSOLE_Y_RES );
		sBackground->SetOrigin( glm::vec2( 0.5f ) );

        // the inner background
        sBackground2 = new Shape();
        sBackground2->SetColor( glm::vec4( 0.1, 0.1, 0.1f, 1.0f) );
        sBackground2->SetPosition( CONSOLE_BORDER_SIZE, CONSOLE_Y_RES );
        sBackground2->SetScale( CONSOLE_X_RES - 2.0f * CONSOLE_BORDER_SIZE
            , -CONSOLE_Y_RES + CONSOLE_BORDER_SIZE );
		sBackground2->SetOrigin( glm::vec2( 0.5f ) );
    }

    /*
    ================
    InitHistory

    Inits the sHisory renderables.
    ================
    */
    static void InitHistory()
    {
        for ( int i = 0; i < VISIBLE_LINES; i++ )
        {
            sHistory[ i ] = new Text( "", sConsoleFont, CONSOLE_FONT_HEIGHT );
            sHistory[ i ]->SetColor( sTextColor );
            sHistory[ i ]->SetPosition( TEXT_PADDING, HeightForRow( i ) );
        }
    }

    /*
    ================
    InitInputLine

    Inits the prompt line renderables.
    ================
    */
    static void InitInputLine()
    {
        sPromptText = new Text( PROMPT_TEXT, sConsoleFont, CONSOLE_FONT_HEIGHT );
        sPromptText->SetColor( sPromptColor );
        sPromptText->SetPosition( TEXT_PADDING, HeightForRow(-1) );

        const int promptSize = ( int )( sPromptText->GetTextDimensions().x + PROMPT_PADDING );

        sInputText = new Text( "", sConsoleFont, CONSOLE_FONT_HEIGHT );
        sInputText->SetColor( glm::vec4( 1.0f ) );
        sInputText->SetPosition( TEXT_PADDING + promptSize, HeightForRow(-1) );
    }

    /*
    ================
    Console_Init

    Initializes the console. This creates gl assets so it must be done after the rendering subsystem.

    TODO: Split this into Console_Init() and Console_InitGraphics()
    ================
    */
    void Console_Init()
    {
        // bail if already initialized
        if ( sConsoleInitialized )
        {
            PROCYON_WARN( "Console", "Multiple calls to Console_Init()" );
            return;
        }

        PROCYON_DEBUG( "Console", "Initializing Console" );

        sConsoleInitialized = true;
        sConsoleOpen 		= false;
        sBlinkTimer 		= 0.0f;
        sNextHistoryLine 	= 0;
        sConsoleFont 		= new FontFace( CONSOLE_FONT_FILE, 20 );

        // Setup the console camera
        sConsoleCamera = new Camera2D();
        sConsoleCamera->OrthographicProj( 0.0f, CONSOLE_X_RES, -CONSOLE_Y_RES, 0.0f );

        InitBackground();
        InitHistory();
        InitInputLine();
    }

    /*
    ================
    Console_Destroy

    Destroy the consoles assets.
    ================
    */
    void Console_Destroy()
    {
        if ( !sConsoleInitialized )
        {
            PROCYON_WARN( "Console", "Console_Destroy() called before Console_Init()" );
            return;
        }

        delete sConsoleFont;
        delete sConsoleCamera;

        delete sBackground;
        delete sBackground2;

        for ( int i = 0; i < VISIBLE_LINES; i++ )
        {
            delete sHistory[ i ];
        }

        delete sPromptText;
        delete sInputText;

        sConsoleInitialized = false;
    }

    /*
    ================
    Console_IsOpen

    Returns true if the console is open.
    ================
    */
    bool Console_IsOpen()
    {
        return sConsoleOpen;
    }

    /*
    ================
    Console_SetOpen

    Sets the open state of the console.
    ================
    */
    void Console_SetOpen( bool open )
    {
        sConsoleOpen = open;
        PROCYON_DEBUG( "Console", "Console %s", ( sConsoleOpen ) ? "opened" : "closed" );
    }

    /*
    ================
    Console_SetOpen

    Toggle the open state of the console.
    ================
    */
    void Console_Toggle()
    {
        Console_SetOpen( !sConsoleOpen );
    }

    /*
    ================
    Console_Process

    Advance the state of the console.
    ================
    */
    void Console_Process( FrameTime t )
    {
        // scroll open/close by altering the projection
        float heightDelta = ( -CONSOLE_Y_RES / OPEN_TIME ) * t.dt * ( ( sConsoleOpen ) ? 1.0f : -1.0f );
        sConsoleCamera->OrthographicProj( 0.0f, CONSOLE_X_RES, -CONSOLE_Y_RES
            , glm::clamp( sConsoleCamera->GetHeight() - CONSOLE_Y_RES - heightDelta, 0.0f, CONSOLE_Y_RES ) );

        // oscilate the blink timer, > 0 represents visible
        sBlinkTimer = sinf( t.tsl * ( float )M_PI * 2.0f / CURSOR_BLINK_PERIOD );
    }

    /*
    ================
    Console_Render

    Renders the console.
    ================
    */
    void Console_Render( Renderer* renderer )
    {
        // only render if open
        if ( sConsoleCamera->GetHeight() > CONSOLE_Y_RES )
        {
            // render everything in the console coordinate system.
            renderer->PushCamera( *sConsoleCamera );

            // render the background
            renderer->Draw( sBackground );
            renderer->Draw( sBackground2 );

            // render the history text
            for ( int i = 0; i < VISIBLE_LINES; i++ )
            {
                renderer->Draw( sHistory[ i ] );
            }

            // render the input line
            renderer->Draw( sPromptText );
            renderer->Draw( sInputText );

            // render the cursor
            if ( sBlinkTimer > 0.0f )
            {
                float cursorX = sInputText->GetTextDimensions().x
                    + sPromptText->GetTextDimensions().x + PROMPT_PADDING;

                const FontMetrics metrics = sConsoleFont->GetMetrics( CONSOLE_FONT_HEIGHT );
                renderer->DrawLine(
                    glm::vec2( TEXT_PADDING + cursorX + 1.0f, TEXT_PADDING ),
                    glm::vec2( TEXT_PADDING + cursorX + 1.0f, TEXT_PADDING + metrics.line_height ),
                    sPromptColor );
            }

            renderer->PopCamera();
        }
    }

    /*
    ================
    Console_PrintLine

    Prints a massage to the console in the provided color. This advances the history by 1 and will drop previous messages off the end of the buffer.
    ================
    */
    void Console_PrintLine( const std::string& msg, const glm::vec4& rgb )
    {
        sHistory[ sNextHistoryLine ]->SetText( msg );
        sHistory[ sNextHistoryLine ]->SetColor( rgb );

        // set the position of every render line to their new 'row' position
        // computed from sNextHistoryLine
        for ( int i = 0; i < VISIBLE_LINES; i++ )
        {
            int row = ( i + VISIBLE_LINES - sNextHistoryLine ) % VISIBLE_LINES;
            sHistory[ i ]->SetPosition( TEXT_PADDING, HeightForRow( row ) );
        }

        // advance the history bottom row
        sNextHistoryLine = ( VISIBLE_LINES + sNextHistoryLine - 1 ) % VISIBLE_LINES;
    }

    /*
    ================
    Console_PrintLine

    thunk with the default color (sTextColor).
    ================
    */
    void Console_PrintLine( const std::string& msg )
    {
        Console_PrintLine( msg, sTextColor );
    }

    /*
    ================
    Console_PrintLine

    Clear all history text.
    ================
    */
    void Console_Clear()
    {
        for ( int i = 0; i < VISIBLE_LINES; i++ )
        {
            sHistory[ i ]->SetText( "" );
            sHistory[ i ]->SetColor( sTextColor );
        }
        sNextHistoryLine = 0;
    }

    /*
    ================
    Console_Execute

    Execute a string and call it's handler function.

    @return true if the command succeeded in calling a handler.
    ================
    */
    bool Console_Execute( const std::string& cmd )
    {
        // prefix and append to the history buffer
        std::stringstream ss;
        ss << "> " << cmd;
        Console_PrintLine( ss.str() );

        // hardcoded commands for now
        if ( cmd == "help" )
        {
            Console_PrintLine( "Usage: <cmd> [args]");
        }
        else if ( cmd == "msaa" )
        {
            static bool msaaOn = false;

            if ( !msaaOn )
            {
                msaaOn = true;
                //glEnable( GL_MULTISAMPLE );
                //glEnable( GL_LINE_SMOOTH );
                Console_PrintLine( "MSAA on", sSystemColor );
            }
            else
            {
                msaaOn = false;
                //glDisable( GL_MULTISAMPLE );
                //glDisable( GL_LINE_SMOOTH );
                Console_PrintLine( "MSAA off", sSystemColor );
            }
        }
        else if ( cmd == "clear" )
        {
            Console_Clear();
        }
        else if ( cmd == "debug_text" )
        {
            sDebugText = !sDebugText;
            Console_PrintLine( std::string( "text debugging " ) + ( ( sDebugText ) ? "enabled": "disabled" ), sSystemColor );
        }
		else if ( cmd == "debug_lines")
		{
            sDebugLines = !sDebugLines;
            Console_PrintLine( std::string( "line debugging " ) + ( ( sDebugLines ) ? "enabled": "disabled" ), sSystemColor );
		}
        else
        {
            Console_PrintLine( "Unknown command!",  sErrorColor );
            return false; // failure - unknown command
        }

        return true; // success
    }

    /*
    ================
    Console_Execute

    Lets the console handle input events (mainly concerned with key events).

    @return true if the event should be absorbed.
    ================
    */
    bool Console_HandleEvent( const InputEvent& ev )
    {
        if ( ev.type == EVENT_KEY_DOWN || ev.type == EVENT_KEY_REPEAT )
        {
            if ( ev.keysym == KEY_RETURN )
            {
                // execute the input text if not empty
                if ( !sInputText->GetText().empty() )
                {
                    Console_Execute( sInputText->GetText() );
					sLastInput = sInputText->GetText();
                    sInputText->SetText( "" );
                }
            }
			else if ( ev.keysym == KEY_UP )
			{
				sInputText->SetText( sLastInput );
			}
            else if ( ev.keysym == KEY_ESCAPE )
            {
                // close on escape
                Console_SetOpen( false );
            }
            else if ( ev.keysym == KEY_BACKSPACE )
            {
                // erase on backspace
                if ( sInputText->CharacterCount() > 0 )
                {
                    sInputText->EraseBack();
                }
            }

			// absorb all key down events
            return true;
		}
        else if ( ev.type == EVENT_KEY_UP )
        {
            // absorb keyups as well
            return true;
        }
	    else if ( ev.type == EVENT_TEXT )
		{
            if ( ev.unicode > 0x1F && ev.unicode < 0x80 ) // ascii non-control only
            {
                // handle a character key

                // the future width of the input text field.
                float inputWidth = sInputText->GetTextDimensions().x
                    + sConsoleFont->GetMetrics( CONSOLE_FONT_HEIGHT ).max_advance;

                // the width available to the input text field.
                float available = CONSOLE_X_RES 			// start with the total x res
                    - 2.0f * TEXT_PADDING					// remove the border + padding
                    - sPromptText->GetTextDimensions().x 	// remove the prompt width
                    - PROMPT_PADDING;						// remove the padding between prompt and input text field

                if ( inputWidth <= available )
                {
                    PROCYON_DEBUG( "Console", "Keydown Event '%li' '%c'", ev.unicode, ev.unicode );

                    // append the char to the input buffer.
                    sInputText->Append( std::string( 1, (char)( ev.unicode & 0x7F ) ) );
                }
            }

            // absorb all text events
            return true;
        }

        // don't absorb anything else
        return false;
    }

} /* namespace Procyon */
