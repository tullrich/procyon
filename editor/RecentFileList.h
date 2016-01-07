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
#ifndef _RECENT_FILE_LIST_H
#define _RECENT_FILE_LIST_H

#include <QObject>
#include <QStringList>

class QMenu;

class RecentFileList : public QObject
{
	Q_OBJECT
public:
	RecentFileList( QObject *owner, QMenu *menu, int maxFileCount );

	void AddRecentFile( const QString& filepath );

	void SetFiles( QStringList files );
	QStringList GetFiles() const;

signals:
	void RecentFileSelected( QString filepath );

protected slots:
	void OpenRecentFile();

protected:
	void SyncMenuWidget();

	QMenu *mMenu;

	// The maximum number of file to display
	int mMaxFiles;

    // Recent file action items (appear in the Recent menu)
    QStringList mRecentPaths;
};

#endif /* _RECENT_FILE_LIST_H */
