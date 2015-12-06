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

#include "test_base.h"
#include "reflection.h"

class ReflectionTests : public ProcyonTestBase { };

/*
================
TestBase
================
*/
class TestBase : public ReflectableClass
{
public:
	DECLARE_REFLECTION_TABLE( TestBase )
};

REFLECTION_TABLE_EMPTY( TestBase )

/*
================
TestDerived
================
*/
class TestDerived : public TestBase
{
public:
	DECLARE_REFLECTION_TABLE( TestDerived )
};

REFLECTION_TABLE_BEGIN( TestDerived )
	ADD_PARENT_CLASSES( TestBase )
REFLECTION_TABLE_END()

/*
================
OtherDerived
================
*/
class OtherDerived : public TestDerived
{
public:
	DECLARE_REFLECTION_TABLE( OtherDerived )
};

REFLECTION_TABLE_BEGIN( OtherDerived )
	ADD_PARENT_CLASSES( TestDerived )
REFLECTION_TABLE_END()

/*
================
ReflectionTests::GetClassName
================
*/
TEST_F(ReflectionTests, GetClassName) 
{
	TestBase tb;
	EXPECT_STREQ( "TestBase", tb.GetClassName() );

	TestDerived td;
	EXPECT_STREQ( "TestDerived", td.GetClassName() );
}

/*
================
ReflectionTests::InstanceOf1
================
*/
TEST_F(ReflectionTests, InstanceOf1) 
{
	TestBase base;
	TestDerived derived;

	EXPECT_TRUE( base.InstanceOf( &base ) );
	EXPECT_FALSE( base.InstanceOf( &derived ) );
	EXPECT_TRUE( derived.InstanceOf( &base ) );
	EXPECT_TRUE( derived.InstanceOf( &derived ) );
}

/*
================
ReflectionTests::InstanceOf2
================
*/
TEST_F(ReflectionTests, InstanceOf2) 
{
	TestBase base;
	TestDerived derived;

	EXPECT_TRUE( base.InstanceOf( TestBase::Class() ) );
	EXPECT_FALSE( base.InstanceOf( TestDerived::Class() ) );
	EXPECT_TRUE( derived.InstanceOf( TestBase::Class() ) );
	EXPECT_TRUE( derived.InstanceOf( TestDerived::Class() ) );
}