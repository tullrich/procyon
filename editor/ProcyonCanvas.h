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

#ifndef _PROCYON_CANVAS_H
#define _PROCYON_CANVAS_H

#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <ProcyonCommon.h>

namespace Procyon {
	class Camera2D;
    class Renderer;
    class Sprite;
    class World;
}
class Grid;

class ProcyonCanvas : public QOpenGLWidget
{
    Q_OBJECT

public:

                                ProcyonCanvas( QWidget* Parent );
    virtual                     ~ProcyonCanvas();

    // Set the render grid tile size
    void                        SetShowGrid( bool render );

    // Set the grid 
    void                        SetGridSize( float gridSize );

    // Reset the camera transform back to the origin.
    void                        ResetCamera();

    // Returns the current camera used by this camera.
    const Procyon::Camera2D*    GetCamera() { return mCamera; }

signals:
    void    CameraChanged( const Procyon::Camera2D* camera );

protected:
    virtual void    initializeGL();
    virtual void    paintGL();

    // Internal utility
    glm::vec2       WindowToWorld( const QPointF& point );
    glm::vec2       WindowToScreen( const QPointF& point );

    // Input handlers
    virtual void    mousePressEvent( QMouseEvent* event );
    virtual void    mouseReleaseEvent( QMouseEvent* event );
	virtual void 	keyPressEvent( QKeyEvent* event );
    virtual void    mouseMoveEvent( QMouseEvent* event );
    virtual void    wheelEvent( QWheelEvent* event );

    // Custom implemented event handlers.
    void OnMouseDrag( QMouseEvent* event );
    void OnMouseMove( QMouseEvent* event );

    // Viewport camera
	Procyon::Camera2D* 	mCamera;

    // Scene renderer
    Procyon::Renderer* 	mRenderer;

    // World renderable
    Procyon::World*     mWorld;

    // Grid renderable
    Grid*				mGrid;

    // Timer used to measure deltatime.
    QElapsedTimer 		mElapsed;

    // Last frame dt information used to compute this frames dt.
    FrameTime           mLastDt;

    // Last known window space mouse position used for scene drag.
    glm::vec2           mMouseLast;

    // The point where dragging started
    QPoint              mDragStart;

    Procyon::Sprite*    mGhostTile;

};

#endif /* _PROCYON_CANVAS_H */
