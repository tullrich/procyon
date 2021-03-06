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

#ifndef _SCENE_OBJECT
#define _SCENE_OBJECT

#include "ProcyonCommon.h"

#include <QObject>
#include <QVector>
#include <QPoint>

namespace Procyon
{
	class Renderer;
}

class SceneObject : public QObject
{
    Q_OBJECT

public:
	SceneObject( QString name, QObject* owner );

	Q_PROPERTY( QString Name MEMBER mName NOTIFY NameChanged READ GetName WRITE SetName )
	Q_PROPERTY( QPoint Position MEMBER mPosition READ GetPosition WRITE SetPosition )
    Q_PROPERTY( QPoint Dimensions MEMBER mDimensions READ GetDimensions WRITE SetDimensions )
	Q_PROPERTY( float Rotation MEMBER mRotation READ GetRotation WRITE SetRotation )

	int GetChildCount() const { return mChildren.size(); }
	const SceneObject* GetChild( int idx ) const;
	SceneObject* GetChild( int idx );

	const SceneObject* GetParent() const { return mParent; }
	SceneObject* GetParent() { return mParent; }
	void AddChild( SceneObject* child );
	void RemoveChild( SceneObject* child );

	int GetChildIndex( const SceneObject* child ) const;
	int GetIndexInParent() const;

	QString GetName() const { return mName; }
    void SetName( QString name ) { mName = name; emit NameChanged( mName ); }

    QPoint GetPosition() const { return mPosition; }
    void SetPosition( QPoint pos ) { mPosition = pos; }

    QPoint GetDimensions() const { return mDimensions; }
    void SetDimensions( QPoint dims ) { mDimensions = dims; }

    float GetRotation() const { return mRotation; }
    void SetRotation( float rot ) { mRotation = rot; }

	void Render( Procyon::Renderer* renderer );

    void SetSelected( bool selected ) { mSelected = selected; }
    bool IsSelected() const { return mSelected; }

signals:
	void ChildRemovedStart( SceneObject* child, int idx );
	void ChildRemovedEnd( SceneObject* child, int idx );
	void ChildAdded( SceneObject* child, int idx );
	void NameChanged( const QString &newName );

protected:
	void SetParent( SceneObject* parent ) { mParent = parent; };
	void OnRender( Procyon::Renderer* renderer );

	QPoint mDimensions;
	QPoint mPosition;
    float mRotation;

	QString mName;

	SceneObject* mParent;
	QVector< SceneObject* > mChildren;

    bool mSelected;
};

#endif /* _SCENE_OBJECT */
