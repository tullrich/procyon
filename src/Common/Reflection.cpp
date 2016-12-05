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

#include "Reflection.h"

void ClassReflectionTable::AddParent( const ClassReflectionTable* parent )
{
	PROCYON_INFO( "Reflection", "Setting '%s' as parent class of '%s'.", parent->GetName(), mClassName );
	mParents[ mParentCount++ ] = parent;
}

bool ClassReflectionTable::HasImmediateParent( const ClassReflectionTable* search ) const
{
	for( int i = 0; i < mParentCount; i++)
	{
		if(mParents[ i ] == search)
			return true;

	}
	return false;
}

bool ClassReflectionTable::HasParent( const ClassReflectionTable* search ) const
{
	for( int i = 0; i < mParentCount; i++)
	{
		if(mParents[ i ] == search || mParents[ i ]->HasParent( search ))
			return true;

	}
	return false;
}

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
	PROCYON_INFO( "Reflection", "Initializing all reflection tables (found %i/%i)."
		, gAutoRegisteredClassCount, MAX_REFLECTED_CLASSES );
	for( unsigned int i = 0; i < gAutoRegisteredClassCount; i++ )
	{
		( *gAutoRegisteredClasses[ i ].mRegistrationFunction )( i );
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
	PROCYON_INFO( "Reflection", "Destroying all reflection tables (found %i/%i)."
		, gAutoRegisteredClassCount, MAX_REFLECTED_CLASSES );
	for( unsigned int i = 0; i < gAutoRegisteredClassCount; i++ )
	{
		(*gAutoRegisteredClasses[i].mDestructionFunction)();
	}
}

const char* ReflectableClass::GetClassName() const
{
	return GetClass()->GetName();
}

unsigned int ReflectableClass::GetClassId() const
{
	return GetClass()->GetId();
}

bool ReflectableClass::InstanceOf( const ReflectableClass *cls ) const
{
	return GetClass() == cls->GetClass() || GetClass()->HasParent( cls->GetClass() );
}

bool ReflectableClass::InstanceOf( const ClassReflectionTable *rt ) const
{
	return GetClass() == rt || GetClass()->HasParent( rt );
}