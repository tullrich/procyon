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

#ifndef _EDITOR_H
#define _EDITOR_H

#include <QMainWindow>

class QLabel;

namespace Ui {
	class Editor;
}
namespace Procyon {
	class Camera2D;
}
class ProcyonCanvas;


class EditorForm : public QMainWindow
{
    Q_OBJECT

public:
    EditorForm(QWidget *parent = 0);
    ~EditorForm();

protected slots:
	void GridSizeChanged( QAction* action );
	void ResetViewport();

    // Update the status bar camera text to match the provided camera.
	void CanvasCameraChanged( const Procyon::Camera2D* cam );

protected:
    void SetupOutputLog();

	// Designer layout
    Ui::Editor* 	mUi;

    // Main viewport canvas
    ProcyonCanvas* 	mCanvas;

    // Status bar camera position text
    QLabel* 		mCameraLabel;
};

#endif /* _EDITOR_H */