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

#ifndef _LOGGING_H
#define _LOGGING_H

#include "logog.hpp"

#define PROCYON_DEBUG( group, formatstring, ... ) \
	PROCYON_LEVEL_MESSAGE( LOGOG_LEVEL_DEBUG, group, NULL, formatstring, ##__VA_ARGS__ )

#define PROCYON_INFO( group, formatstring, ... ) \
	PROCYON_LEVEL_MESSAGE( LOGOG_LEVEL_INFO, group, NULL, formatstring, ##__VA_ARGS__ )

#define PROCYON_WARN( group, formatstring, ... ) \
	PROCYON_LEVEL_MESSAGE( LOGOG_LEVEL_WARN, group, NULL, formatstring, ##__VA_ARGS__ )

#define PROCYON_ERROR( group, formatstring, ... ) \
	PROCYON_LEVEL_MESSAGE( LOGOG_LEVEL_ERROR, group, NULL, formatstring, ##__VA_ARGS__ )

#define PROCYON_LEVEL_MESSAGE( level, group, category, formatstring, ... ) \
	LOGOG_LEVEL_GROUP_CATEGORY_MESSAGE( level, group, category, formatstring, ##__VA_ARGS__ )

#endif /* _LOGGING_H */