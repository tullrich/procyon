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
#include "ioc.h"

using namespace Procyon;

class IOCTests : public ProcyonTestBase { };

/*
================
TestBase
================
*/
class ITestInterface : public ReflectableClass
{
public:
	DECLARE_REFLECTION_TABLE( ITestInterface )
};

REFLECTION_TABLE_EMPTY( ITestInterface )

/*
================
TestDerived
================
*/
class TestImplementation : public ITestInterface
{
public:
    TestImplementation() 
    {
    	Constructed();
    }
    
  	MOCK_METHOD0(Constructed, void());

	DECLARE_REFLECTION_TABLE( TestImplementation )
};

REFLECTION_TABLE_BEGIN( TestImplementation )
	ADD_PARENT_CLASSES( ITestInterface )
REFLECTION_TABLE_END()

/*
================
TestDerived
================
*/
class TestImplementation2 : public ITestInterface
{
public:
	DECLARE_REFLECTION_TABLE( TestImplementation2 )
};

REFLECTION_TABLE_BEGIN( TestImplementation2 )
	ADD_PARENT_CLASSES( ITestInterface )
REFLECTION_TABLE_END()

/*
================
IOCTests::Resolve1
================
*/
TEST_F(IOCTests, Resolve1)
{
	TypeCatalog catalog( {
    	Factory< TestImplementation
    		, Implements< ITestInterface > >(),
    	Factory< TestImplementation2 >()
    } );

    ITestInterface* imp = catalog.Resolve< ITestInterface >();

    ASSERT_TRUE( imp != NULL );
	EXPECT_EQ( TestImplementation::Class(), imp->GetClass() );
	EXPECT_STREQ( "TestImplementation", imp->GetClassName() );

    ITestInterface* imp2 = catalog.Resolve< TestImplementation2 >();

    ASSERT_TRUE( imp2 != NULL );
	EXPECT_EQ( TestImplementation2::Class(), imp2->GetClass() );
	EXPECT_STREQ( "TestImplementation2", imp2->GetClassName() );
}

/*
================
IOCTests::Resolve2
================
*/
TEST_F(IOCTests, Resolve2)
{
	TypeCatalog catalog( {
    	Factory< TestImplementation2
    		, Implements< TestImplementation, ITestInterface > >()
    } );

	// create implementation via interface
    TestImplementation* imp = catalog.Resolve< TestImplementation >();

    ASSERT_TRUE( imp != NULL );
	EXPECT_EQ( TestImplementation2::Class(), imp->GetClass() );
	EXPECT_STREQ( "TestImplementation2", imp->GetClassName() );


	// create implementation via itself
    TestImplementation2* imp2 = catalog.Resolve< TestImplementation2 >();

    ASSERT_TRUE( imp2 != NULL );
	EXPECT_EQ( TestImplementation2::Class(), imp2->GetClass() );
	EXPECT_STREQ( "TestImplementation2", imp2->GetClassName() );
}

/*
================
IOCTests::Resolve_DoesNotExist
================
*/
TEST_F(IOCTests, Resolve_DoesNotExist)
{
	TypeCatalog catalog( {
    	Factory< TestImplementation2
    		, Implements< ITestInterface > >()
    } );

	// create implementation via interface
    TestImplementation* imp = catalog.Resolve< TestImplementation >();

    EXPECT_TRUE( imp == NULL );
}

/*
================
IOCTests::MalformedCatalogs
================
*/
TEST_F(IOCTests, MalformedCatalogs)
{
	// Type is an implementor
	ASSERT_THROW( TypeCatalog catalog( {
    	Factory< TestImplementation2
    		, Implements< ITestInterface > >(),
    	Factory< TestImplementation
    		, Implements< TestImplementation2 > >()
    } ), std::invalid_argument );

	// Type is an implementor (reversed)
	ASSERT_THROW( TypeCatalog catalog( {
    	Factory< TestImplementation2
    		, Implements< ITestInterface > >(),
    	Factory< ITestInterface
    		, Implements< TestImplementation > >()
    } ), std::invalid_argument );

	// Type already wired to implementor.
	ASSERT_THROW( TypeCatalog catalog( {
    	Factory< TestImplementation2
    		, Implements< ITestInterface > >(),
    	Factory< TestImplementation
    		, Implements< ITestInterface > >()
    } ), std::invalid_argument );

	// Type already wired to implementor.
	ASSERT_THROW( TypeCatalog catalog( {
    	Factory< TestImplementation2
    		, Implements< ITestInterface > >(),
    	Factory< TestImplementation
    		, Implements< ITestInterface > >()
    } ), std::invalid_argument );

	// Type wired to itself.
	ASSERT_THROW( TypeCatalog catalog( {
    	Factory< TestImplementation
    		, Implements< TestImplementation > >()
    } ), std::invalid_argument );

	// Multiple entries of the same type
	ASSERT_THROW( TypeCatalog catalog( {
    	Factory< TestImplementation
    		, Implements< TestImplementation2 > >(),
    	Factory< TestImplementation
    		, Implements< ITestInterface > >()
    } ), std::invalid_argument );
}

/*
================
IOCTests::Singleton
================
*/
TEST_F(IOCTests, Singleton)
{
	TypeCatalog catalog( {
    	Singleton< TestImplementation
    		, Implements< ITestInterface > >()
    } );

	// resolve it once
    ITestInterface* imp = catalog.Resolve< ITestInterface >();
    EXPECT_TRUE( imp != NULL );

	// resolve it again
    ITestInterface* imp2 = catalog.Resolve< ITestInterface >();
    EXPECT_TRUE( imp2 != NULL );

    // should be the same instance
    EXPECT_EQ(imp, imp2);
}

/*
================
IOCTests::Factory
================
*/
TEST_F(IOCTests, Factory)
{
	TypeCatalog catalog( {
    	Factory< TestImplementation
    		, Implements< ITestInterface > >()
    } );

	// resolve it once
    ITestInterface* imp = catalog.Resolve< ITestInterface >();
    EXPECT_TRUE( imp != NULL );

	// resolve it again
    ITestInterface* imp2 = catalog.Resolve< ITestInterface >();
    EXPECT_TRUE( imp2 != NULL );

    // should be different instances
    EXPECT_NE(imp, imp2);
}