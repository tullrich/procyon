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
#include "RecentFileList.h"

#include <QMenu>

RecentFileList::RecentFileList( QObject *owner, QMenu *menu, int maxFileCount )
	: QObject( owner )
	, mMenu( menu )
	, mMaxFiles( maxFileCount )
{
    for ( int i = 0; i < mMaxFiles; ++i )
    {
        QAction *action = new QAction( this );
        action->setText( "<empty>" );
        action->setEnabled( false );
        action->setData( -1 );
        connect( action, SIGNAL( triggered() ), this, SLOT( OpenRecentFile() ) );

        mMenu->addAction( action );
    }
}

void RecentFileList::SyncMenuWidget()
{
    QList< QAction* > recentActions = mMenu->actions();
    Q_ASSERT( recentActions.size() == mMaxFiles );

    // Set filled recent actions
    for ( int i = 0; i < mRecentPaths.size(); ++i )
    {
        recentActions[ i ]->setText( mRecentPaths[ i ] );
        recentActions[ i ]->setData( i );
        recentActions[ i ]->setEnabled( true );
    }

    // Clear unused actions
    for ( int i = mRecentPaths.size(); i < mMaxFiles; ++i )
    {
        recentActions[ i ]->setText( "<empty>" );
        recentActions[ i ]->setEnabled( false );
        recentActions[ i ]->setData( -1 );
    }
}

void RecentFileList::AddRecentFile( const QString& filepath )
{
    // Append to recent files list
    mRecentPaths.removeAll( filepath );
    mRecentPaths.prepend( filepath );
    while ( mRecentPaths.size() > mMaxFiles )
    {
        mRecentPaths.removeLast();
    }

    SyncMenuWidget();
}

void RecentFileList::OpenRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());

    int index = action->data().toInt();
    if ( index >= 0 && index < mRecentPaths.size() )
    {
    	emit RecentFileSelected( mRecentPaths[ index ] );
    }
}

void RecentFileList::SetFiles( QStringList files )
{
	mRecentPaths.clear();
	mRecentPaths = files;
	while ( mRecentPaths.size() > mMaxFiles )
    {
        mRecentPaths.removeLast();
    }

	SyncMenuWidget();
}

QStringList RecentFileList::GetFiles() const
{
	return mRecentPaths;
}