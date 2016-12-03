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

#include "ProcyonCommon.h"
#include "SceneObject.h"
#include "World.h"

#include <QObject>
#include <QFileInfo>
#include <QList>
#include <QAbstractItemModel>
#include <QColor>
#include <QList>
#include <QIcon>

namespace Procyon {
    class Camera2D;
}
class SceneObject;

class SceneObjectListModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	SceneObjectListModel( QObject *parent, SceneObject* root )
		: QAbstractItemModel( parent )
		, mRoot( root )
	{
		//setupModelData
		RegisterSignals( mRoot );
	}

	void RegisterSignals( SceneObject* object )
	{
    	connect( object, SIGNAL( ChildAdded(SceneObject*, int) ), this, SLOT( OnObjectAdded(SceneObject*, int) ) );
	    connect( object, SIGNAL( ChildRemovedStart(SceneObject*, int) ), this, SLOT( OnObjectRemovedStart(SceneObject*, int) ) );
	    connect( object, SIGNAL( ChildRemovedEnd(SceneObject*, int) ), this, SLOT( OnObjectRemovedEnd(SceneObject*, int) ) );
	    connect( object, SIGNAL( NameChanged(const QString&) ), this, SLOT( OnObjectNameChanged(const QString&) ) );
		for( int i = 0; i < object->GetChildCount(); i++ )
		{
			RegisterSignals( object->GetChild( i ) );
		}
	}

	void UnregisterSignals( SceneObject* object )
	{
    	object->disconnect( this );
		for( int i = 0; i < object->GetChildCount(); i++ )
		{
			UnregisterSignals( object->GetChild( i ) );
		}
	}

	const SceneObject* Resolve( const QModelIndex& idx ) const
	{
    	const SceneObject* parentItem = nullptr;
	    if ( idx.isValid() )
		{
	        parentItem = static_cast< SceneObject* >( idx.internalPointer() );
		}
		return parentItem;
	}

	QModelIndex GetIndexFor( SceneObject* s ) const
	{
		QModelIndex index;
		if ( s != mRoot )
		{
			assert( s->GetParent() );
			index = createIndex( s->GetIndexInParent(), 0, (void*)s );
		}
		return index;
	}

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const
	{
		return 1;
	}

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
	{
    	const SceneObject* parentItem = Resolve( parent );
		if ( !parentItem )
		{
			parentItem = mRoot;
		}
	    return parentItem->GetChildCount();
	}

	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
	{
	    if ( !hasIndex(row, column, parent) )
		{
	        return QModelIndex();
		}

		const SceneObject* parentItem = Resolve( parent );
		if ( !parentItem )
		{
			parentItem = mRoot;
		}
		if ( row < parentItem->GetChildCount() )
		{
			QModelIndex index = createIndex( row, column, (void*)parentItem->GetChild( row ) );
			//PROCYON_DEBUG( "Editor", "Undex Of [%i, %i - %i ]"
			//	, index.row(), index.column(), index.internalPointer() );
			return index;
		}
		else
		{
	        return QModelIndex();
		}
	}

	virtual QModelIndex parent( const QModelIndex &index ) const
	{
		if ( !index.isValid() )
			return QModelIndex();

		//PROCYON_DEBUG( "Editor", "Parent Of [%i, %i - %i ]"
		//	, index.row(), index.column(), index.internalPointer() );
		const SceneObject* childItem = Resolve( index );
		const SceneObject* parentItem = childItem->GetParent();
	    if ( parentItem == mRoot )
	        return QModelIndex();

	    return createIndex( parentItem->GetIndexInParent(), 0, (void*)parentItem );
	}

	virtual QVariant data( const QModelIndex& index, int role ) const
	{
		if ( !index.isValid() )
			return QVariant();

		const SceneObject* item = Resolve( index );
		switch( role )
		{
			case Qt::DisplayRole: return QVariant( item->GetName() );
			case Qt::DecorationRole: return QVariant( QIcon(":resources/document.png") );
			default: return QVariant();
		}
	}

protected slots:
	void OnObjectAdded( SceneObject* child, int idx )
	{
		QModelIndex index;
		if ( child->GetParent() != mRoot )
		{
			index = createIndex( child->GetParent()->GetIndexInParent(), 0, (void*)child->GetParent() );
		}

		RegisterSignals( child );
		beginInsertRows( index, idx, idx);
		endInsertRows();
	}

	void OnObjectRemovedStart( SceneObject* child, int idx )
	{
		QModelIndex index;
		if ( child->GetParent() != mRoot )
		{
			index = createIndex( child->GetParent()->GetIndexInParent(), 0, (void*)child->GetParent() );
		}

		//PROCYON_DEBUG( "Editor", "Deleting row [ %i, 0, %i ] : Parent [%i, %i - %i ]"
		//	, idx,(void*)child, index.row(), index.column(), index.internalPointer() );
		beginRemoveRows( index, idx, idx);
	}

	void OnObjectRemovedEnd( SceneObject* child, int idx )
	{
		endRemoveRows();
		UnregisterSignals( child );
	}

	void OnObjectNameChanged( const QString& newName )
	{
		PROCYON_DEBUG( "Editor", "OnObjectNameChanged" );
		SceneObject* obj = qobject_cast< SceneObject* >( sender() );
		QModelIndex index = GetIndexFor( obj );
		emit dataChanged( index, index, { Qt::DisplayRole } );
	}

protected:
	SceneObject* mRoot;
};

class MapDocument : public QObject
{
    Q_OBJECT

	// Restorable camera state
	struct CameraState
	{
		CameraState()
			: center( glm::vec2( (float)TILE_PIXEL_SIZE * 5.0f ) )
			, zoom( 1.0f )
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

	int GetTileDefCount() const { return mTileSet->Size(); }
	const Procyon::TileDef& GetTileDef( int idx ) const;

	SceneObject* GetRootSceneObject() { return mRoot; }
signals:
    void 	FilePathChanged( const QFileInfo& filepath );
    void 	DocumentPreparingToSave();
    void 	ModificationStateChanged( bool modified );

protected:
	// This maps tileset
	Procyon::TileSet*	mTileSet;

    // World renderable
    Procyon::World*     mWorld;

	// Scene object root
	SceneObject* mRoot;

    // Saved camera state
    CameraState 		mCameraState;

	// Save path of this document
	QFileInfo 			mFilePath;

	// True if this map has been modified since last save or open
	bool 				mModified;
};


#endif /* _MAP_DOCUMENT */
