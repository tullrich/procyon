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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "reflection.h"

// Base test fixture for all Procyon tests.
class ProcyonTestBase : public ::testing::Test {
protected:

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:
	virtual void SetUp() 
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
		InitReflectionTables();
	}

	virtual void TearDown() 
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
        DestroyReflectionTables();
	}
};