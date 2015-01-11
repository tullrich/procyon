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

#ifndef _CLASS_H
#define _CLASS_H

#include <iostream>
#include "logging.h"

/*

===================

Reflection macros and utilities.

Supports reflection of classes, member functions, and member variables.
===================

If you just want to make use of this system you only need to do two things:

1. Add the DECLARE_REFLECTION_TABLE() macro inside your class definition. Note that it must be
made public!

	class TestBase
	{
	public:

		DECLARE_REFLECTION_TABLE( TestBase )
	}

2. 

*/

// very little overhead so feel free to raise this.
#define MAX_REFLECTED_CLASSES 256

/*
================
ClassReflectionTable
================
*/
class ClassReflectionTable
{
public:
	ClassReflectionTable( unsigned int classId, const char* clsName )
		: mClassId( classId )
		, mClassName( clsName )
	{
		TWODGAME_INFO( "Reflection", "ClassReflectionTable '%s' id=%i created .", mClassName, mClassId );
	}

	unsigned int 	GetId() const	{ return mClassId; 	}
	const char* 	GetName() const	{ return mClassName; }

protected:

	// unique class id.
	unsigned int mClassId;

	// human readable name of the class.
	const char* mClassName;
};

/*
================
ReflectionAutoRegister

Internal helper.
================
*/
struct ReflectionAutoRegister
{
	unsigned int mClassId;
	void (*mRegistrationFunction)( unsigned int id );
	void (*mDestructionFunction)();
};

// internal var- array of auto-registered reflection table creation/destruction functions.
extern ReflectionAutoRegister gAutoRegisteredClasses[MAX_REFLECTED_CLASSES];

// internal var - reflectable class count.
extern unsigned int gAutoRegisteredClassCount;

void InitReflectionTables();
void DestroyReflectionTables();

// allows nested expansions stringify (i.e. STRINGIFY(MACRO(cls))).
#define STRINGIFY_EXPAND( str ) #str
#define STRINGIFY( str ) STRINGIFY_EXPAND( str )

// allows nested expansions in concatenations (i.e. APPEND(MACRO(A), B)).
#define APPEND_EXPAND( A, B ) A##B
#define APPEND( A, B ) APPEND_EXPAND( A, B )

// internal reflection helpers
#define REFLECTION_CLASS_NAME( cls ) ReflectionTable_##cls
#define REFLECTION_INST_NAME( cls ) APPEND( _, REFLECTION_CLASS_NAME(cls) )

/*
================
DECLARE_REFLECTION_TABLE

Add this function within the class declaration.
================
*/
#define DECLARE_REFLECTION_TABLE( cls ) 									\
																			\
	static ClassReflectionTable* sReflectionTable;							\
	static void InitReflectionTable( unsigned int id ); 					\
	static void DestroyReflectionTable();									\
																			\
	virtual const ClassReflectionTable*	GetClass();							\
																			\
	struct REFLECTION_CLASS_NAME( cls )										\
	{ 																		\
		REFLECTION_CLASS_NAME( cls )()										\
		{																	\
			ReflectionAutoRegister &rec =									\
				gAutoRegisteredClasses[gAutoRegisteredClassCount]; 			\
			rec.mRegistrationFunction 	= &cls::InitReflectionTable;		\
			rec.mDestructionFunction	= &cls::DestroyReflectionTable; 	\
			rec.mClassId				= gAutoRegisteredClassCount++;		\
		}																	\
	}; 																		\
	static REFLECTION_CLASS_NAME( cls ) REFLECTION_INST_NAME( cls );


/*
================
REFLECTION_TABLE_BEGIN

Put this inside the implementation file (.cpp), this function begins the reflection
table definition and must be followed by a REFLECTION_TABLE_END.
================
*/
#define REFLECTION_TABLE_BEGIN( cls ) 										\
	cls::REFLECTION_CLASS_NAME( cls ) cls::REFLECTION_INST_NAME( cls );		\
	ClassReflectionTable* cls::sReflectionTable = NULL;						\
																			\
	const ClassReflectionTable* cls::GetClass()								\
	{																		\
		return sReflectionTable;											\
	}																		\
																			\
	void cls::DestroyReflectionTable()										\
	{																		\
		delete sReflectionTable;											\
	}																		\
																			\
	void cls::InitReflectionTable( unsigned int id )						\
	{																		\
		sReflectionTable = new ClassReflectionTable(id, STRINGIFY(cls));	\


/*
================
REFLECTION_TABLE_END

Put this after the REFLECTION_TABLE_BEGIN after you have specified all properties.
================
*/
#define REFLECTION_TABLE_END( cls )											\
	}

/*
================
REFLECTION_TABLE_EMPTY

Helper to define the reflection table of a class with no exposed properties.
================
*/
#define REFLECTION_TABLE_EMPTY( cls ) 										\
	REFLECTION_TABLE_BEGIN( cls ) 											\
	REFLECTION_TABLE_END()


class ReflectableClass
{
public:
	virtual const ClassReflectionTable* GetClass() = 0;
	virtual const char* 				GetClassName();
	virtual unsigned int 				GetClassId();
};

#endif /* _CLASS_H */