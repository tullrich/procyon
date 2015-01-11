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
#include "logging.h"
#include <SFML/Window.hpp>

#include "reflection.h"

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

class TestBase : public ReflectableClass
{
public:

	DECLARE_REFLECTION_TABLE( TestBase )

	int min;
};

class TestDerived : public TestBase
{
	DECLARE_REFLECTION_TABLE( TestDerived )
};


REFLECTION_TABLE_EMPTY( TestBase )

REFLECTION_TABLE_BEGIN( TestDerived )
REFLECTION_TABLE_END()

int main( int argc, char *argv[] )
{
	logog::INIT_PARAMS logogInit;
	logogInit.m_pfMalloc 	= &CustomMalloc;
	logogInit.m_pfFree 		= &CustomFree;

	LOGOG_INITIALIZE( &logogInit );
	{
		logog::Cout err;

		InitReflectionTables();

		TestBase tb;
		TWODGAME_DEBUG("System", "Get class name '%s'", tb.GetClassName());


		TestDerived td;
		TWODGAME_DEBUG("System", "Get class name '%s'", td.GetClassName());


        //RunWindow();

	}
	
    LOGOG_SHUTDOWN();

	return 0;
}