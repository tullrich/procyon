/*
===========================================================================

2DGame, a 2D game.

Copyright (C) 2015 Tim Ullrich.

This file is part of 2DGame.

2DGame is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

2DGame is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with 2DGame.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/

#include <iostream>
#include <cstdlib>
#include "logog.hpp"
#include <SFML/Window.hpp>

/*
================
CustomMalloc
================
*/
void* CustomMalloc( size_t size )
{
	return std::malloc( size );
}

/*
================
CustomFree
================
*/
void CustomFree( void* ptr )
{
	std::free( ptr );
}

void RunWindow()
{
	sf::Window window( sf::VideoMode( 800, 600 ), "My window" );

    // run the program as long as the window is open
    while ( window.isOpen() )
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            // "close requested" event: we close the window
            if ( event.type == sf::Event::Closed )
                window.close();
        }
    }
}

int main( int argc, char *argv[] )
{
	logog::INIT_PARAMS logogInit;
	logogInit.m_pfMalloc 	= &CustomMalloc;
	logogInit.m_pfFree 		= &CustomFree;

	LOGOG_INITIALIZE( &logogInit );
	{
		// filters must be instanced before sinks.
		// only pass category 'Unrecoverable'.
		logog::GetFilterDefault().Group( "Graphics" );


		// filter audio.
        logog::Filter filter;
        filter.Group( "Audio" );

		// sinks
		logog::LogFile errFile( "log.txt" );
		logog::Cout err;

		filter.UnpublishToMultiple( logog::AllTargets() );
		filter.PublishTo( err );

#undef LOGOG_GROUP
#undef LOGOG_CATEGORY
#define LOGOG_GROUP  "Graphics"
#define LOGOG_CATEGORY "Unrecoverable"

		// log stuff
        INFO( "Hello, logog!" );
        WARN( "This is a warning" );

#undef LOGOG_GROUP
#define LOGOG_GROUP "Audio"

        INFO( "This is an audio log!" );

#undef LOGOG_GROUP
#define LOGOG_GROUP "AI"

        INFO( "This is an AI log!" );

        RunWindow();
	}
	
    LOGOG_SHUTDOWN();

	return 0;
}