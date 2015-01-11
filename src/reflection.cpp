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

#include "reflection.h"


ReflectionAutoRegister gAutoRegisteredClasses[MAX_REFLECTED_CLASSES];
unsigned int gAutoRegisteredClassCount = 0;

/*
================
InitReflectionTables

Create the sReflectionTable of every registered reflectable class.
Only call this once.
================
*/
void InitReflectionTables()
{
	TWODGAME_INFO( "Reflection", "Initializing all reflection tables (found %i/%i)."
		, gAutoRegisteredClassCount, MAX_REFLECTED_CLASSES );
	for( int i = 0; i < gAutoRegisteredClassCount; i++ )
	{
		(*gAutoRegisteredClasses[i].mRegistrationFunction)( i );
	}
}

/*
================
DestroyReflectionTables

Destroy the sReflectionTable of every registered reflectable class.
Only call this once.
================
*/
void DestroyReflectionTables()
{
	TWODGAME_INFO( "Reflection", "Destroying all reflection tables (found %i/%i)."
		, gAutoRegisteredClassCount, MAX_REFLECTED_CLASSES );
	for( int i = 0; i < gAutoRegisteredClassCount; i++ )
	{
		(*gAutoRegisteredClasses[i].mDestructionFunction)();
	}
}

const char* ReflectableClass::GetClassName()
{
	return GetClass()->GetName();
}

unsigned int ReflectableClass::GetClassId()
{
	return GetClass()->GetId();
}


/*
REFLECTION_TABLE_BEGIN( TestClass )
	REGISTER_BASE_CLASS( TestBase )
REFLECTION_TABLE_END()

void SetupClassDefinition( ClassDefinition &cls )
{
	cls.AddProperty("", &TestBase::min)
}*/