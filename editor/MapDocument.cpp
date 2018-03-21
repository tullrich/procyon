#include "MapDocument.h"
#include "SceneObject.h"
#include "Collision/World.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "ProcyonQtUtil.h"
#include "EditorAssets.h"
#include "MapDocumentCommands.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>

#define MAP_DOCUMENT_UNDO_LIMIT 256

MapDocument::MapDocument()
	: mTileSet( new Procyon::TileSet() )
	, mWorld( new Procyon::World )
	, mRoot( new SceneObject( "Root", this ) )
	, mModified( false )
    , mUndoStack( new QUndoStack( this ) )
    , mListModel( new QSortFilterProxyModel( this ) )
{
    mListModel->setDynamicSortFilter( false );
    mListModel->setSourceModel( new SceneObjectListModel( this, mRoot ) );
    mSelectionModel = new QItemSelectionModel( mListModel, this );

    mUndoStack->setUndoLimit( MAP_DOCUMENT_UNDO_LIMIT );
}

MapDocument::~MapDocument()
{
	delete mWorld;
	delete mTileSet;
}

QString MapDocument::GetTabString() const
{
	QString text = ( HasSavePath() ) ? GetFileName() : tr( "untitled" );
	if ( mModified )
		text += "*";
	return text;
}

QString MapDocument::GetTitleString() const
{
    if ( HasSavePath() )
    {
        return QString( "%1 - %2 - Procyon" ).arg( GetTabString(), GetFilePath() );
    }
    else
    {
        return QString( "%1 - Procyon" ).arg( GetTabString() );
    }
}

void MapDocument::SetFilePath( const QFileInfo& filepath )
{
	 mFilePath = filepath;
	 emit FilePathChanged( mFilePath );
}

void MapDocument::SetModified( bool modified /* = true */)
{
	if ( mModified != modified )
	{
		mModified = modified;
        if ( !mModified )
        {
            mUndoStack->setClean();
        }
		emit ModificationStateChanged( mModified );
	}
}

bool MapDocument::Save( const QString& filename )
{
	emit DocumentPreparingToSave();

	QFile file( filename );

	if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
        PROCYON_WARN( "MapDocument", "Unable to open file '%s' for saving.", filename.toUtf8().data() );
        return false;
	}

	QXmlStreamWriter writer( &file );
    writer.setAutoFormatting( true );
    writer.writeStartDocument();

    writer.writeStartElement( "Map" );

		writer.writeStartElement( "TileSet" );
		for ( int i = 1; i < mTileSet->Size(); i++ )
		{
			writer.writeStartElement( "TileDef" );
			writer.writeAttribute( "filepath", mTileSet->GetTileDef( i ).filepath.c_str() );
			writer.writeAttribute( "type", Procyon::TileDef::TileTypeToString( mTileSet->GetTileDef( i ).type ) );
			writer.writeEndElement(); // TileDef
		}
		writer.writeEndElement(); // TileSet

	    writer.writeStartElement("Camera");
	    	writer.writeAttribute( "x", QString::number( mCameraState.center.x ) );
	    	writer.writeAttribute( "y", QString::number( mCameraState.center.y ) );
	    	writer.writeAttribute( "zoom", QString::number( mCameraState.zoom ) );
	    writer.writeEndElement(); // camera

	    writer.writeStartElement("Tiles");
        writer.writeAttribute( "width", QString::number( mWorld->GetSize().x ) );
        writer.writeAttribute( "height", QString::number( mWorld->GetSize().y ) );
	    for ( int w = 0; w < mWorld->GetSize().x; ++w )
	    {
	    	writer.writeStartElement( "TileRow" );
	    	writer.writeAttribute( "index", QString::number( w ) );
	    	for ( int h = 0; h < mWorld->GetSize().y; ++h )
	    	{
		    	writer.writeStartElement( "Tile" );
		    	writer.writeAttribute( "index", QString::number( h ) );
		    	writer.writeCharacters( QString::number( int( mWorld->GetTile( glm::ivec2( w, h ) ) ) ) );
	    		writer.writeEndElement(); // Tile
		    }
	    	writer.writeEndElement(); // TileRow
	    }
	    writer.writeEndElement(); // tiles

        if ( mRoot && mRoot->GetChildCount() > 0 )
        {
            writer.writeStartElement( "Objects" );
            for ( int oi = 0; oi < mRoot->GetChildCount(); oi++ )
            {
                const SceneObject* obj = mRoot->GetChild( oi );
                writer.writeStartElement( "Object" );
                writer.writeAttribute( "name", obj->GetName() );
                writer.writeStartElement( "Transform" );
                writer.writeStartElement( "Position" );
                writer.writeAttribute( "x", QString::number( obj->GetPosition().x() ) );
                writer.writeAttribute( "y", QString::number( obj->GetPosition().y() ) );
                writer.writeAttribute( "z", QString::number( 0 ) );
                writer.writeEndElement(); // Position
                writer.writeStartElement( "Dimensions" );
                writer.writeAttribute( "width", QString::number( obj->GetDimensions().x() ) );
                writer.writeAttribute( "height", QString::number( obj->GetDimensions().y() ) );
                writer.writeEndElement(); // Dimensions
                writer.writeStartElement( "Rotation" );
                writer.writeCharacters( QString::number( obj->GetRotation() ) );
                writer.writeEndElement(); // Rotation
                writer.writeEndElement(); // Transform
                writer.writeEndElement(); // Object
            }
            writer.writeEndElement(); // Objects
        }
	writer.writeEndElement(); // map

    writer.writeEndDocument();

    if ( writer.hasError() )
	{
    	PROCYON_WARN( "MapDocument", "Saving map '%s' failed with QXmlStreamWriter error"
    		, filename.toUtf8().data() );
    	return false;
    }
	else if ( file.error() != QFile::NoError )
	{
    	PROCYON_WARN( "MapDocument", "Saving map '%s' failed with QFile error: %s"
    		, filename.toUtf8().data(), file.errorString().toUtf8().data() );
    	return false;
	}

	SetModified( false );
	return true;
}

bool MapDocument::Load( const QString& filename )
{
	QFile file( filename );

	if ( !file.open( QIODevice::ReadOnly ) )
	{
        PROCYON_WARN( "MapDocument", "Unable to open file '%s' for loading.", filename.toUtf8().data() );
        return false;
	}

    QDomDocument doc;

    QString errMsg;
    int errLine, errColumn;
    if ( !doc.setContent( &file, &errMsg, &errLine, &errColumn ) )
    {
        PROCYON_WARN( "MapDocument", "Error parsing XML file '%s' Line %i Column %i: %s"
            , filename.toUtf8().data()
            , errLine, errColumn
            , errMsg.toUtf8().data() );
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if ( !n.isNull() )
    {
        // Parse TileSet
        QDomElement domTileSet = docElem.firstChildElement( "TileSet" );
        if ( !domTileSet.isNull() )
        {
            QDomElement domtTileDef = domTileSet.firstChildElement( "TileDef" );
            while ( !domtTileDef.isNull() )
            {
                QString filepath = domtTileDef.attribute( "filepath" );

                Procyon::TileDef def;
                if ( !filepath.isEmpty() )
                {
                    def.filepath = filepath.toUtf8();
					def.collidable = true;
                }

	            QString type = domtTileDef.attribute( "type" );
                if ( !type.isEmpty() )
				{
					def.type = Procyon::TileDef::StringToTileType( type.toUtf8().data() );
				}

				mTileSet->AddTileDef( def );
                domtTileDef = domtTileDef.nextSiblingElement( "TileDef" );
            }
        }

        // Parse Camera
        QDomElement domCamera = docElem.firstChildElement( "Camera" );
        if ( !domCamera.isNull() )
        {
            // parse camera
            mCameraState.center.x = domCamera.attribute( "x" ).toFloat();
            mCameraState.center.y = domCamera.attribute( "y" ).toFloat();
            mCameraState.zoom = domCamera.attribute( "zoom" ).toFloat();
        }

        // Parse Tiles
        QDomElement domTiles = docElem.firstChildElement( "Tiles" );
        if ( !domTiles.isNull() )
        {
            int sizeX = domTiles.attribute( "width" ).toUInt();
            int sizeY = domTiles.attribute( "height" ).toUInt();
            mWorld->NewWorld( glm::ivec2( sizeX, sizeY ), mTileSet );

            // parse tiles
            QDomElement domTileRow = domTiles.firstChildElement( "TileRow" );
            while ( !domTileRow.isNull() )
            {
                int w = domTileRow.attribute( "index" ).toInt();
                QDomElement domTile = domTileRow.firstChildElement( "Tile" );
                while ( !domTile.isNull() )
                {
                    int h = domTile.attribute( "index" ).toInt();
                    int tileType = domTile.text().toInt();
                    mWorld->SetTile( glm::ivec2( w, h ), ( Procyon::TileId )tileType );
                    domTile = domTile.nextSiblingElement( "Tile" );
                }
                domTileRow = domTileRow.nextSiblingElement( "TileRow" );
            }
        }

        // Parse Objects
        QDomElement domObjects = docElem.firstChildElement( "Objects" );
        if ( !domObjects.isNull() )
        {
            QDomElement domObject = domObjects.firstChildElement( "Object" );
            QString name;
            while ( !domObject.isNull() && !( name = domObject.attribute( "name" ) ).isEmpty() )
            {
                SceneObject* obj = new SceneObject( name, nullptr );

                QDomElement domTransform = domObject.firstChildElement( "Transform" );
                if ( !domTransform.isNull() )
                {
                    QDomElement domPosition = domTransform.firstChildElement( "Position" );
                    if ( !domPosition.isNull() )
                    {
                        QPoint pos;
                        pos.setX( domPosition.attribute( "x" ).toFloat() );
                        pos.setY( domPosition.attribute( "y" ).toFloat() );
                        obj->SetPosition( pos );
                    }

                    QDomElement domDimensions = domTransform.firstChildElement( "Dimensions" );
                    if ( !domDimensions.isNull() )
                    {
                        QPoint dims;
                        dims.setX( domDimensions.attribute( "width" ).toFloat() );
                        dims.setY( domDimensions.attribute( "height" ).toFloat() );
                        obj->SetDimensions( dims );
                    }

                    QDomElement domRotation = domTransform.firstChildElement( "Rotation" );
                    if ( !domRotation.isNull() )
                    {
                        obj->SetRotation( domRotation.text().toFloat() );
                    }
                }

                mRoot->AddChild( obj );
                domObject = domObject.nextSiblingElement( "Object" );
            }
        }
    }

	mFilePath = filename;
	SetModified( false );
	return true;
}

void MapDocument::NewWorld( int sizeX, int sizeY )
{
	mTileSet->Clear();

	Procyon::TileDef td;
	td.filepath = "sprites/tile.png";
	td.texture = EditorAssets::sTileTexture;
	td.type = Procyon::TILETYPE_SOLID;
	td.collidable = true;
	mTileSet->AddTileDef( td );

	td.filepath = "sprites/uv_map.png";
	td.texture = Procyon::Texture::Allocate( "sprites/uv_map.png" );
	td.type = Procyon::TILETYPE_ONE_WAY;
	mTileSet->AddTileDef( td );

    mWorld->NewWorld( glm::ivec2( sizeX, sizeY ), mTileSet );

	SetModified( true );
}

void MapDocument::RestoreCameraState( Procyon::Camera2D *camera ) const
{
    camera->SetPosition( mCameraState.center );
    camera->SetZoom( mCameraState.zoom );
}

void MapDocument::SaveCameraState( const Procyon::Camera2D *camera )
{
    mCameraState.center = camera->GetPosition();
    mCameraState.zoom = camera->GetZoom();
}

void MapDocument::Render( Procyon::Renderer* r )
{
    mWorld->Render( r );
}

void MapDocument::AddCommand( QUndoCommand* cmd )
{
	mUndoStack->push( cmd );
}

void MapDocument::SetTile( const glm::ivec2& t, Procyon::TileId type, int stroke /*= -1*/ )
{
	if ( mWorld->InBounds( t ) && GetTile( t ) != type )
	{
		mUndoStack->push( new SetTileCommand( this, t, type, stroke ) );
	}
}

Procyon::TileId MapDocument::GetTile( const glm::ivec2& t )
{
	return mWorld->GetTile( t );
}

const Procyon::TileDef& MapDocument::GetTileDef( int idx ) const
{
	return mTileSet->GetTileDef( (Procyon::TileId)idx );
}

glm::ivec2 MapDocument::GetSize() const
{
	return mWorld->GetSize();
}
