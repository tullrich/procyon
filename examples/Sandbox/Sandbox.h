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
#include "Player.h"
#include "Joystick.h"
#include "AudioDevice.h"
#include "Camera.h"

namespace Procyon 
{
    class Renderer;
    class Map;
}
class Grid;

using namespace Procyon::GL;
using namespace Procyon;

class Sandbox : public MainLoop
{
public:
                    Sandbox();

    virtual void    Initialize( int argc, char *argv[] );
    virtual void    Cleanup();

    virtual void    Process( FrameTime t );
    virtual void    Render();

    virtual void    OnKeyDown( const InputEvent& ev );
    virtual void    OnKeyUp( const InputEvent& ev );

    virtual void    OnMouseDown( const InputEvent& ev );
    virtual void    OnMouseUp( const InputEvent& ev );
    virtual void    OnMouseMoved( const InputEvent& ev );

    virtual void    OnWindowChanged( const InputEvent& ev );

protected:
    Map*            LoadMap( std::string filePath );

    IJoystick*      mJoyStick;
    Player*         mPlayer;
    Renderer*       mRenderer;
    AudioDevice*    mAudioDev;
    Camera2D*       mCamera;
    Grid*           mGrid;
    Map*            mCustomMap;

};
	
#endif /* _SANDBOX_H */
