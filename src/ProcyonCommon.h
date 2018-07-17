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

#ifndef _PROCYON_COMMON_H
#define _PROCYON_COMMON_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <set>
#include <map>
#include <type_traits>
#include <chrono>
#include <iomanip>
#include <stack>
#include <cmath>

#include "Logging.h"
#include "Macros.h"
#include "Rect.h"
#include "Aabb.h"

namespace Procyon {

	struct FrameTime
	{
	    float   dt;     // delta time
	    float   tsl;    // time since launch
	};

} /* namespace Procyon */

#endif /* _PROCYON_COMMON_H */
