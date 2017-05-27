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

#ifndef _MAP_DOCUMENT_COMMANDS
#define _MAP_DOCUMENT_COMMANDS

#include "ProcyonCommon.h"
#include "MapDocument.h"

#include <QUndoCommand>
#include <QLinkedList>

class SetTileCommand : public QUndoCommand
{
public:
	SetTileCommand( MapDocument* document, const glm::ivec2& t, Procyon::TileId type, int stroke  )
		: mDocument( document )
		, mType( type )
        , mStroke( stroke )
	{
        mRecords.append( { t,document->GetTile( t ) } );
		setText( "apply brush" );
	}

	virtual void undo()
	{
        for ( auto& iter : mRecords )
        {
            mDocument->mWorld->SetTile( iter.mIndex, iter.mPrevType );
			mDocument->SetModified();
        }
	}

	virtual void redo()
	{
        for ( auto& iter : mRecords )
        {
            mDocument->mWorld->SetTile( iter.mIndex, mType );
			mDocument->SetModified();
        }
	}

    virtual int	id() const
    {
        return 1;
    }

    virtual bool mergeWith( const QUndoCommand *other )
    {
        if ( other->id() != id() ) // make sure other is also an AppendText command
            return false;

        const SetTileCommand* stc = static_cast< const SetTileCommand* >( other );
        if ( mStroke == -1 || stc->mStroke != mStroke || stc->mType != mType )
            return false;

        mRecords += stc->mRecords;
        return true;
    }

private:
    struct SetTileRecord
    {
        glm::ivec2      mIndex;
        Procyon::TileId mPrevType;
    };

	MapDocument* 	                mDocument;
    QLinkedList< SetTileRecord >    mRecords;
	Procyon::TileId                 mType;
    int                             mStroke;
};

#endif /* _MAP_DOCUMENT_COMMANDS */
