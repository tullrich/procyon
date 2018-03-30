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
#ifndef _SANDBOX_H
#define _SANDBOX_H

#include "ProcyonCommon.h"
#include "MainLoop.h"
#include "Platform/Joystick.h"
#include "Graphics/Camera.h"
#include "Graphics/Text.h"
#include "Collision/World.h"
#include "Player.h"
#include "PolyLine.h"

namespace Procyon
{
    class Map;
	class Sprite;
}

using namespace Procyon::GL;
using namespace Procyon;

#define SANDBOX_RESOLUTION_X 480
#define SANDBOX_RESOLUTION_Y 270
#define SANDBOX_WINDOW_WIDTH 960
#define SANDBOX_WINDOW_HEIGHT 540
#define CAMERA_VERTICAL_OFFSET 80.0f
#define CAMERA_LERP_RATE 0.25f

class Sandbox : public MainLoop
{
public:
                    Sandbox();

    virtual void    Initialize( int argc, char *argv[] );
    virtual void    Cleanup();

    virtual void    Process( FrameTime t );
    virtual void    Render();

    virtual void    OnMouseMoved( const InputEvent& ev );

    virtual void    OnWindowChanged( const InputEvent& ev );

protected:
    Map*            LoadMap( std::string filePath );
	std::string 	BuildFPSString() const;

    IJoystick*      mJoyStick;
    Player*         mPlayer;
    Camera2D*       mCamera;
    Map*            mCustomMap;
	Text*           mFpsText;
	PolyLine		mPolyLine;
	World*          mWorld;
	std::vector< Procyon::Sprite* > mStaticSprites;

};

#endif /* _SANDBOX_H */
