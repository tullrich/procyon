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
#include <QMenu>
#include <QListWidget>

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
class QtVariantPropertyManager;
class QtProperty;

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);
    virtual ~Editor();

    RecentFileList* GetRecent() { return mRecentFiles; }

signals:
    void ActiveDocumentChanged(MapDocument *doc);

public slots:
    void SetActiveDocument( int idx );

protected slots:
	void GridSizeChanged( QAction* action );
	void ResetViewport();
    void TabMoved( int from, int to );
	void OpenPreferences();

    void NewDocument();
    void OpenDocument( QString filePath = QString() );
    bool SaveDocument( MapDocument *doc = NULL );
    bool SaveDocumentAs( MapDocument *doc = NULL );
    bool CloseDocument( MapDocument *doc = NULL, bool forceDiscard = false );

    // Update the status bar camera text to match the provided camera.
	void CanvasCameraChanged( const Procyon::Camera2D* cam );
	void OnSceneTreeContextMenuRequested( const QPoint& point );

	void OnObjectActivated( const QModelIndex &index );
	void OnPropertyValueChanged(QtProperty* property, const QVariant& value );

protected:
    void ShowStatusMessage( const QString& msg, bool permanent = false );
	void AddClassProperties(const QMetaObject *metaObject);
	void SetPropertySheetObject( QObject *object );
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

	// The scene tree context menu
	QAction* mSceneCreate, *mSceneDelete;
	QMenu* mSceneTreeItemMenu;

    // The active document (the one in the currently viewable tab)
    MapDocument*    mActiveDocument;

    // The working set of map documents
    QList< MapDocument* > mDocuments;

    // Recent file action items (appear in the Recent menu)
    RecentFileList* mRecentFiles;

	// Current object being shown on the property sheet
	QObject* mPropertyObject;
	QtVariantPropertyManager* mPropertyManager;
    QList<QtProperty *> mTopLevelProperties;
    QMap<const QMetaObject *, QtProperty *> mClassToProperty;

	// Tile sets list widget
	QListWidget* mTileSetsView;
};

#endif /* _EDITOR_H */
