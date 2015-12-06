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

#ifndef _CLASS_H
#define _CLASS_H

#include "ProcyonCommon.h"

class ReflectableClass;

/*

===================

Reflection macros and utilities.

Supports reflection of classes, member functions, and member variables.
===================

If you just want to make a new reflectable class you only need to do three things:

1. Make sure that your new class derives from ReflectableClass (does not need to be immediate).

2. Add the DECLARE_REFLECTION_TABLE() macro inside your class definition. It must be made public!

	class TestBase : public ReflectableClass
	{
	public:

		DECLARE_REFLECTION_TABLE( TestBase )
	}

3. Define the reflection table inside of a single implementation file (not a header!).

	REFLECTION_TABLE_EMPTY( TestBase )
or

	REFLECTION_TABLE_BEGIN( TestBase )
		...
	REFLECTION_TABLE_END()

Inside the reflection table you may list parents using the ADD_PARENT_CLASSES macro.

*/

// very little overhead so feel free to raise this.
#define MAX_REFLECTED_CLASSES 256

// max base classes, also very little overhead.
#define MAX_REFLECTED_BASES 16

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
		, mParentCount( 0 )
	{
		PROCYON_INFO( "Reflection", "ClassReflectionTable '%s' id=%i created .", mClassName, mClassId );
	}

	unsigned int 				GetId() 	const { return mClassId;   }
	const char* 				GetName() 	const { return mClassName; }

	void 						AddParent 			( const ClassReflectionTable* parent );
	bool						HasImmediateParent 	( const ClassReflectionTable* search ) const;
	bool						HasParent 			( const ClassReflectionTable* search ) const;

protected:

	// unique class id.
	unsigned int mClassId;

	// human readable name of the class.
	const char* mClassName;

	// array of parent classes
	const ClassReflectionTable* mParents[ MAX_REFLECTED_BASES ];

	// current number of parents, may be zero.
	short mParentCount;
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
	static const ClassReflectionTable* Class() { return sReflectionTable; }	\
																			\
	virtual const ClassReflectionTable*	GetClass() const;					\
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
	const ClassReflectionTable* cls::GetClass()	const						\
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
		sReflectionTable 													\
			= new ClassReflectionTable( id, STRINGIFY( cls ) );				\


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

// FOREACH below is adapted from 
// http://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros
#define FE_1(WHAT, X) WHAT(X) 
#define FE_2(WHAT, X, ...) WHAT(X)FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X)FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X)FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X)FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X)FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X)FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X)FE_7(WHAT, __VA_ARGS__)
#define FE_9(WHAT, X, ...) WHAT(X)FE_8(WHAT, __VA_ARGS__)
#define FE_10(WHAT, X, ...) WHAT(X)FE_9(WHAT, __VA_ARGS__)
#define FE_11(WHAT, X, ...) WHAT(X)FE_10(WHAT, __VA_ARGS__)
#define FE_12(WHAT, X, ...) WHAT(X)FE_11(WHAT, __VA_ARGS__)
#define FE_13(WHAT, X, ...) WHAT(X)FE_12(WHAT, __VA_ARGS__)
#define FE_14(WHAT, X, ...) WHAT(X)FE_13(WHAT, __VA_ARGS__)
#define FE_15(WHAT, X, ...) WHAT(X)FE_14(WHAT, __VA_ARGS__)
#define FE_16(WHAT, X, ...) WHAT(X)FE_15(WHAT, __VA_ARGS__)

#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,NAME,...) NAME 
#define FOR_EACH(action,...) \
	GET_MACRO(__VA_ARGS__,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1)(action,__VA_ARGS__)

#define ADD_PARENT_INTERNAL( parent )											\
	sReflectionTable->AddParent( parent::sReflectionTable );

#define ADD_PARENT_CLASSES( ... )												\
	FOR_EACH( ADD_PARENT_INTERNAL, __VA_ARGS__ )


class ReflectableClass
{
public:
	virtual const ClassReflectionTable* GetClass() const = 0;
	virtual const char* 				GetClassName() const;
	virtual unsigned int 				GetClassId() const;
	virtual bool 						InstanceOf( const ReflectableClass *ls ) const;
	virtual bool 						InstanceOf( const ClassReflectionTable *ls ) const;
};

#endif /* _CLASS_H */