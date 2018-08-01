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

#ifndef _SPRITE_SHEET_VIEWER_WINDOW_H
#define _SPRITE_SHEET_VIEWER_WINDOW_H

#include "ProcyonCommon.h"

#include <QMainWindow>

namespace Ui {
	class SpriteSheetViewerWindow;
}

namespace Procyon {
    class AnimatedSprite;
}

class SpriteSheetViewerWindow : public QMainWindow
{
	Q_OBJECT
public:
	SpriteSheetViewerWindow( QString spritePath, QWidget *parent = 0 );
	virtual ~SpriteSheetViewerWindow();

signals:
    void Close();

protected:
    virtual void closeEvent( QCloseEvent *event );

	// Designer layout
    Ui::SpriteSheetViewerWindow* 	mUi;

    // The sprite under edition
    Procyon::AnimatedSprite* mSprite;
};

#endif /* _SPRITE_SHEET_VIEWER_WINDOW_H */
