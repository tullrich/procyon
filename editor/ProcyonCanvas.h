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

#include <QGLWidget>
#include <QElapsedTimer>
#include <ProcyonCommon.h>

namespace Procyon {
	class Camera2D;
    class Renderer;
    class Sprite;
    class World;
}
class Editor;
class Grid;
class MapDocument;
class QTimer;

class ProcyonCanvas : public QGLWidget
{
    Q_OBJECT

public:

            ProcyonCanvas( QWidget* Parent, Editor* editor );
    virtual ~ProcyonCanvas();

    // Set the render grid tile size
    float   GetGridSize() const;
    bool    GetShowGrid() const;

    // Reset the camera transform back to the origin.
    void    ResetCamera();

    void    PauseRendering();
    void    StartRendering();

    Procyon::Rect   GetMapBounds() const { return mMapBounds; }
    glm::vec2       GetCameraPosition() const;
    Procyon::Rect   GetCameraViewport() const;

signals:
    void    CameraChanged( const Procyon::Camera2D* camera );

public slots:
    void    OnActiveDocumentChanged( MapDocument *doc);

    // enable/disable the grid
    void    ToggleShowGrid( bool show );

    // toggle the grid
    void    SetGridSize( float gridSize );

    void    SaveCameraState();

    void    MoveCamera( float xMove, float yMove );
    void    SetCamera( float xPos, float yPos );

protected:
    virtual void    initializeGL();
    virtual void    paintGL();
    virtual void    resizeGL(int w, int h);

    // Internal utility
    glm::vec2       WindowToWorld( const QPointF& point );
    glm::vec2       WindowToScreen( const QPointF& point );
    void            Zoom( float amount );

    // Input handlers
    virtual void    mousePressEvent( QMouseEvent* event );
    virtual void    mouseReleaseEvent( QMouseEvent* event );
	virtual void 	keyPressEvent( QKeyEvent* event );
    virtual void    mouseMoveEvent( QMouseEvent* event );
    virtual void    wheelEvent( QWheelEvent* event );

    // Custom implemented event handlers.
    void OnMouseDrag( QMouseEvent* event );
    void OnMouseMove( QMouseEvent* event );

	// The global editor
	Editor				*mEditor;

    // repaint timer
    QTimer              *mTimer;

    // Scene renderer
    Procyon::Renderer   *mRenderer;

    // Viewport camera
    Procyon::Camera2D*  mCamera;

    // World renderable
    MapDocument         *mActiveDocument;

    // Grid renderable
    Grid*				mGrid;

    // Timer used to measure deltatime.
    QElapsedTimer 		mElapsed;

    // Last frame dt information used to compute this frames dt.
    Procyon::FrameTime  mLastDt;

    // Last known window space mouse position used for scene drag.
    glm::vec2           mMouseLast;

    // The point where dragging started
    QPoint              mDragStart;

    Procyon::Sprite*    mGhostTile;

	// The map bounding region
	Procyon::Rect		mMapBounds;

    // Current stroke number
    int                 mStroke;

};

#endif /* _PROCYON_CANVAS_H */
