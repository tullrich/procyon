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


#include "gmock/gmock.h"
#include "logog.hpp"

#if GTEST_OS_WINDOWS_MOBILE
# include <tchar.h> 

GTEST_API_ int _tmain(int argc, TCHAR** argv) {
#else
GTEST_API_ int main(int argc, char** argv) {
#endif  // GTEST_OS_WINDOWS_MOBILE
  
  	int result = 0;
	LOGOG_INITIALIZE( );
	{
		logog::Cerr err;

		// Since Google Mock depends on Google Test, InitGoogleMock() is
		// also responsible for initializing Google Test.  Therefore there's
		// no need for calling testing::InitGoogleTest() separately.
		testing::InitGoogleMock(&argc, argv);
		result = RUN_ALL_TESTS();
	}


    LOGOG_SHUTDOWN();

    return result;
}
