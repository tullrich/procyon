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

#ifndef _LOGGING_H
#define _LOGGING_H

#include "logog.hpp"

#define TWODGAME_DEBUG( group, formatstring, ... ) \
	TWODGAME_LEVEL_MESSAGE( LOGOG_LEVEL_DEBUG, group, NULL, formatstring, ##__VA_ARGS__ )

#define TWODGAME_INFO( group, formatstring, ... ) \
	TWODGAME_LEVEL_MESSAGE( LOGOG_LEVEL_INFO, group, NULL, formatstring, ##__VA_ARGS__ )

#define TWODGAME_WARN( group, formatstring, ... ) \
	TWODGAME_LEVEL_MESSAGE( LOGOG_LEVEL_WARN, group, NULL, formatstring, ##__VA_ARGS__ )

#define TWODGAME_ERROR( group, formatstring, ... ) \
	TWODGAME_LEVEL_MESSAGE( LOGOG_LEVEL_ERROR, group, NULL, formatstring, ##__VA_ARGS__ )

#define TWODGAME_LEVEL_MESSAGE( level, group, category, formatstring, ... ) \
	LOGOG_LEVEL_GROUP_CATEGORY_MESSAGE( level, group, category, formatstring, ##__VA_ARGS__ )

#endif /* _LOGGING_H */