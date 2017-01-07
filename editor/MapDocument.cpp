#include "MapDocument.h"
#include "SceneObject.h"
#include "World.h"
#include "GLTexture.h"
#include "Camera.h"
#include "ProcyonQtUtil.h"
#include "EditorAssets.h"

#include <QFile>
#include <QXmlStreamWriter>

MapDocument::MapDocument()
	: mTileSet( new Procyon::TileSet() )
	, mWorld( new Procyon::World )
	, mRoot( new SceneObject( "Root", this ) )
	, mModified( false )
{
	SceneObject* obj1 = new SceneObject( "Object1", mRoot );
	mRoot->AddChild( obj1  );
	mRoot->AddChild( new SceneObject( "Object2", mRoot ) );
	mRoot->AddChild( new SceneObject( "Object3", mRoot ) );
	obj1->AddChild( new SceneObject( "Object4", obj1 ) );
	obj1->AddChild( new SceneObject( "Object5", obj1 ) );

	Procyon::TileDef td;
	td.filepath = "sprites/tile.png";
	td.texture = EditorAssets::sTileTexture;
	td.collidable = true;
	mTileSet->AddTileDef( td );

	td.filepath = "sprites/uv_map.png";
	td.texture = new Procyon::GL::GLTexture( GL_TEXTURE_2D, "sprites/uv_map.png" );
	mTileSet->AddTileDef( td );
}

MapDocument::~MapDocument()
{
	delete mWorld;
	delete mTileSet;
}

QString MapDocument::GetTabString() const
{
	QString text = ( HasSavePath() ) ? FileName() : tr( "untitled" );
	if ( mModified )
		text += "*";
	return text;
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
		emit ModificationStateChanged( mModified );
	}
}

bool MapDocument::Save( const QString& filename )
{
	emit DocumentPreparingToSave();

	QFile file( filename );

	if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
        PROCYON_WARN( "MapDocument", "Unable to open file '%s' for saving.", filename.toUtf8() );
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

	writer.writeEndElement(); // map

    writer.writeEndDocument();

    if ( writer.hasError() )
	{
    	PROCYON_WARN( "MapDocument", "Saving map '%s' failed with QXmlStreamWriter error"
    		, filename.toUtf8() );
    	return false;
    }
	else if ( file.error() != QFile::NoError )
	{
    	PROCYON_WARN( "MapDocument", "Saving map '%s' failed with QFile error: %s"
    		, filename.toUtf8(), file.errorString().toUtf8() );
    	return false;
	}

	SetModified( false );
	return true;
}

bool MapDocument::Load( const QString& filename )
{
	QFile file( filename );

	if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
        PROCYON_WARN( "MapDocument", "Unable to open file '%s' for loading.", filename.toUtf8().data() );
        return false;
	}

	QXmlStreamReader reader( &file );

	reader.readNext();
	if ( !reader.isStartDocument() )
	{
        PROCYON_WARN( "MapDocument", "Error reading map '%s': malformed xml", filename.toUtf8().data() );
        return false;
	}

	reader.readNext();
	if ( !reader.isStartElement() || reader.name() != "Map" )
	{
        PROCYON_WARN( "MapDocument", "Error reading map '%s': malformed xml", filename.toUtf8().data() );
        return false;
	}

	while( !reader.atEnd())
	{
		reader.readNextStartElement();

		if( reader.isStartElement() )
		{
			if ( reader.name() == "TileSet")
			{
				reader.readNextStartElement();
				// parse tiles
				while ( reader.name() == "TileDef" )
				{
					QString w = reader.attributes().value("filepath").toString();
					if ( !w.isEmpty() )
					{
						//mTileSets.push_back( w );
					}

					reader.readNextStartElement();
				}
			}
			else if ( reader.name() == "Camera" )
			{
				// parse camera
				mCameraState.center.x = reader.attributes().value("x").toFloat();
				mCameraState.center.y = reader.attributes().value("y").toFloat();
				mCameraState.zoom = reader.attributes().value("zoom").toFloat();
			}
			else if ( reader.name() == "Tiles" )
			{
                int sizeX = reader.attributes().value( "width" ).toUInt();
                int sizeY = reader.attributes().value( "height" ).toUInt();
                mWorld->NewWorld( glm::ivec2( sizeX, sizeY ), mTileSet );
				reader.readNextStartElement();
				// parse tiles
				while ( reader.name() == "TileRow" )
				{
					PROCYON_DEBUG( "Editor", "TileRow" );
					int w = reader.attributes().value("index").toInt();

					reader.readNextStartElement();
					while( reader.name() == "Tile" )
					{
						int h = reader.attributes().value("index").toInt();
						int tileType = reader.readElementText().toInt();
						PROCYON_DEBUG( "Editor", "\tTile [%i %i] = %i", w, h, tileType );

						mWorld->SetTile( glm::ivec2( w, h ), (Procyon::TileId)tileType );

						//reader.skipCurrentElement();
						reader.readNextStartElement();
					}
				}
			}
		}
	}

    if (reader.hasError())
	{
    	PROCYON_WARN( "MapDocument", "Loading map '%s' failed with QXmlStreamWriter error"
    		, filename.toUtf8().data(), reader.errorString().toUtf8().data() );
    	//return false;
    }
	else if (file.error() != QFile::NoError)
	{
    	PROCYON_WARN( "MapDocument", "Loading map '%s' failed with QFile error: %s"
    		, filename.toUtf8().data(), file.errorString().toUtf8().data() );
    	return false;
	}

	mFilePath = filename;
	SetModified( false );
	return true;
}

void MapDocument::NewWorld( int sizeX, int sizeY )
{
    mWorld->NewWorld( glm::ivec2( sizeX, sizeY ), mTileSet );
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

const Procyon::TileDef& MapDocument::GetTileDef( int idx ) const
{
	return mTileSet->GetTileDef( (Procyon::TileId)idx );
}

glm::ivec2 MapDocument::GetSize() const
{
	return mWorld->GetSize();
}
