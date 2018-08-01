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

#ifndef _SPRITE_SHEET_VIEWER_CANVAS_H
#define _SPRITE_SHEET_VIEWER_CANVAS_H

#include <QGLWidget>
#include <QElapsedTimer>
#include <ProcyonCommon.h>

namespace Procyon {
	class Camera2D;
    class Renderer;
    class AnimatedSprite;
    class World;
}
class MapDocument;
class QTimer;

class SpriteSheetViewerCanvas : public QGLWidget
{
    Q_OBJECT

public:

            SpriteSheetViewerCanvas( QWidget* Parent, Procyon::AnimatedSprite* sprite
                , QString texturePath );
    virtual ~SpriteSheetViewerCanvas();

protected:
    virtual void    initializeGL();
    virtual void    paintGL();
    virtual void    resizeGL(int w, int h);

    // repaint timer
    QTimer              *mTimer;

    // Scene renderer
    Procyon::Renderer   *mRenderer;

    // Viewport camera
    Procyon::Camera2D*  mCamera;

    // Timer used to measure deltatime.
    QElapsedTimer 		mElapsed;

    // Last frame dt information used to compute this frames dt.
    Procyon::FrameTime  mLastDt;

    // The sprite under edition
    Procyon::AnimatedSprite*    mSprite;

    // The texture to load
    QString             mTexturePath;
};

#endif /* _SPRITE_SHEET_VIEWER_CANVAS_H */
