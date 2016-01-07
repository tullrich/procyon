#include "MapDocument.h"
#include "World.h"
#include "Camera.h"
#include "ProcyonQtUtil.h"

#include <QFile>
#include <QXmlStreamWriter>

MapDocument::MapDocument()
	: mWorld( new Procyon::World() )
	, mModified( false )
{
}

MapDocument::~MapDocument()
{
	delete mWorld;
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

	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
	{
        PROCYON_WARN( "MapDocument", "Unable to open file '%s' for saving.", filename.toUtf8() );
        return false;
	}

	QXmlStreamWriter writer( &file );
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    writer.writeStartElement("Map");

	    writer.writeStartElement("Camera");
	    	writer.writeAttribute( "x", QString::number( mCameraState.center.x ) );
	    	writer.writeAttribute( "y", QString::number( mCameraState.center.y ) );
	    	writer.writeAttribute( "zoom", QString::number( mCameraState.zoom ) );
	    writer.writeEndElement(); // camera

	    writer.writeStartElement("Tiles");
	    for ( int w = 0; w < WORLD_WIDTH; ++w )
	    {
	    	writer.writeStartElement( "TileRow" );
	    	writer.writeAttribute( "index", QString::number( w ) );
	    	for ( int h = 0; h < WORLD_HEIGHT; ++h )
	    	{
		    	writer.writeStartElement( "Tile" );
		    	writer.writeAttribute( "index", QString::number( h ) );
		    	writer.writeCharacters( QString::number( mWorld->GetTileType( glm::ivec2( w, h ) ) ) );
	    		writer.writeEndElement(); // Tile
		    }
	    	writer.writeEndElement(); // TileRow
	    }
	    writer.writeEndElement(); // tiles

	writer.writeEndElement(); // map

    writer.writeEndDocument();

    if (writer.hasError())
	{
    	PROCYON_WARN( "MapDocument", "Saving map '%s' failed with QXmlStreamWriter error"
    		, filename.toUtf8() );
    	return false;
    }
	else if (file.error() != QFile::NoError)
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
			if ( reader.name() == "Camera" )
			{
				// parse camera
				mCameraState.center.x = reader.attributes().value("x").toFloat();
				mCameraState.center.y = reader.attributes().value("y").toFloat();
				mCameraState.zoom = reader.attributes().value("zoom").toFloat();
			}
			else if ( reader.name() == "Tiles" )
			{
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

						mWorld->SetTileType( glm::ivec2( w, h ), (Procyon::TileType)tileType );

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