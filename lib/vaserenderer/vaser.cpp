#ifndef VASER_CPP
#define VASER_CPP

#include "vaser.h"

#ifdef VASER_DEBUG
	#define DEBUG printf
#else
	#define DEBUG ;//
#endif

#include <math.h>
#include <vector>
#include <stdlib.h>

namespace VASEr
{
namespace VASErin
{	//VASEr internal namespace
#include "color.h"
#include "agg_curve4.cpp"
}
#include "opengl.cpp"
#include "polyline.cpp"
#include "gradient.cpp"
#include "curve.cpp"

} //namespace VASEr

#undef DEBUG
#endif
