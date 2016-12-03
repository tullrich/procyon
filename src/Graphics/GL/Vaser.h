#include "ProcyonGL.h"

struct VaseVec2 { double x,y;};
struct VaseColor { float r,g,b,a;};

inline VaseVec2 GlmToVaseVec2( const glm::vec2& v )
{
	VaseVec2 ret;
	ret.x = (double)v.x;
	ret.y = (double)v.y;
	return ret;
}

inline VaseColor GlmToVaseColor( const glm::vec4& v )
{
	VaseColor ret;
	ret.r = v.r;
	ret.g = v.g;
	ret.b = v.b;
	ret.a = v.a;
	return ret;
}

typedef VaseVec2 Vec2;
typedef VaseColor Color;

#include "vaser.h"
