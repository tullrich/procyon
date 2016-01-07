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

#ifndef _MAP_DOCUMENT
#define _MAP_DOCUMENT

#include <ProcyonCommon.h>
#include <QObject>
#include <QFileInfo>

namespace Procyon {
    class World;
    class Camera2D;
}

class MapDocument : public QObject
{
    Q_OBJECT

	// Restorable camera state
	struct CameraState
	{
		CameraState()
			: zoom( 1.0f )
		{
		}

		glm::vec2 	center;
		float 		zoom;
	};

public:
	MapDocument();
	virtual ~MapDocument();

	Procyon::World* GetWorld() { return mWorld; }

	QString FileName() const { return mFilePath.fileName(); }
	QString GetFilePath() const { return mFilePath.filePath(); }
	QString GetTabString() const;
	bool IsFresh() const { return !HasSavePath() && !mModified; }

	void SetFilePath( const QFileInfo& filepath );
	bool HasSavePath() const { return !mFilePath.filePath().isEmpty(); }

	bool IsModified() const { return mModified; }
	void SetModified( bool modified = true );
	bool Save( const QString& filename );
	bool Load( const QString& filename );

	void RestoreCameraState( Procyon::Camera2D *camera ) const;
	void SaveCameraState( const Procyon::Camera2D *camera );

signals:
    void 	FilePathChanged( const QFileInfo& filepath );
    void 	DocumentPreparingToSave();
    void 	ModificationStateChanged( bool modified );

protected:

    // World renderable
    Procyon::World*     mWorld;

    // Saved camera state
    CameraState 		mCameraState;

	// Save path of this document
	QFileInfo 			mFilePath;

	// True if this map has been modified since last save or open
	bool 				mModified;
};


#endif /* _MAP_DOCUMENT */