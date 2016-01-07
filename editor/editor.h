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
#include <QList>

class QLabel;

namespace Ui {
	class Editor;
}
namespace Procyon {
	class Camera2D;
}
class ProcyonCanvas;
class MapDocument;
class EditorSettings;
class RecentFileList;

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);
    ~Editor();
    
    RecentFileList* GetRecent() { return mRecentFiles; }

signals:
    void ActiveDocumentChanged(MapDocument *doc);

public slots:
    void SetActiveDocument( int idx );

protected slots:
	void GridSizeChanged( QAction* action );
	void ResetViewport();
    void TabMoved( int from, int to );

    void NewDocument();
    void OpenDocument( QString filePath = QString() );
    bool SaveDocument( MapDocument *doc = NULL );
    bool SaveDocumentAs( MapDocument *doc = NULL );
    bool CloseDocument( MapDocument *doc = NULL, bool forceDiscard = false );

    // Update the status bar camera text to match the provided camera.
	void CanvasCameraChanged( const Procyon::Camera2D* cam );

protected:
    void ShowStatusMessage( const QString& msg, bool permanent = false );
    void AddDocument( MapDocument *doc, bool makeActive = true );

    void SetupOutputLog();

    int GetDocumentIndex( MapDocument* doc ) const;
    int GetDocumentIndex( const QString& filePath ) const;

    virtual void closeEvent( QCloseEvent *event );

	// Designer layout
    Ui::Editor* 	mUi;

    // The shared global settings
    EditorSettings* mSettings;

    // Main viewport canvas
    ProcyonCanvas* 	mCanvas;

    // Status bar camera position text
    QLabel* 		mCameraLabel;

    // The document tab bar
    QTabBar*        mTabBar;

    // The active document (the one in the currently viewable tab)
    MapDocument*    mActiveDocument;;

    // The working set of map documents
    QList< MapDocument* > mDocuments;    

    // Recent file action items (appear in the Recent menu)
    RecentFileList* mRecentFiles;
};

#endif /* _EDITOR_H */