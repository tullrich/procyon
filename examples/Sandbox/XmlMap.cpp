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
#include "XmlMap.h"

#include <tinyxml2.h>

using namespace tinyxml2;

namespace Procyon {

	static int sindent( char *buf, int indent, char c = ' ' )
	{
		for ( int i = 0; i < indent; ++i )
		{
			buf += sprintf( buf, "%c", c );
		}
		return indent;
	}

	static void print_attributes( XMLElement *element, int indent )
	{
		char buf[1024];
		char *msg = buf;

		msg += sindent( msg, indent );

		const XMLAttribute* attrib = element->FirstAttribute();

		while ( attrib )
		{
			sprintf( msg, "%s: value=[%s]", attrib->Name(), attrib->Value() );
			PROCYON_DEBUG( "XmlMap", "%s", buf );
			attrib = attrib->Next();
		}
	}

	static void print_dom( XMLNode *node, int indent = 0 )
	{
		char buf[1024];
		char *msg = buf;

		msg += sindent( msg, indent );

        if ( node->ToDocument() )
        {
			msg += sprintf( msg, "Document" );
	    }
        else if ( node->ToElement() )
        {
			msg += sprintf( msg, "Element [%s]", node->Value() );
        }
        else if ( node->ToComment() )
        {
			msg += sprintf( msg, "Comment: [%s]", node->Value());
        }
        else if ( node->ToUnknown() )
        {
			msg += sprintf( msg, "Unknown" );
        }
        else if ( node->ToText() )
        {
			msg += sprintf( msg, "Text: [%s]", node->ToText()->Value() );
        }
        else if ( node->ToDeclaration() )
        {
			msg += sprintf( msg, "Declaration" );
        }
        else
        {
			msg += sprintf( msg, "Really Unknown" );
		}

		PROCYON_DEBUG( "XmlMap", "%s", buf );

		if ( node->ToElement() )
		{
			print_attributes( node->ToElement(), indent + 1 );
		}
		for ( XMLNode *child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			print_dom( child, indent + 1 );
		}
	}

	XmlMap::XmlMap( const std::string& filePath )
		: mFilePath( filePath )
	{
		for ( int x = 0; x < WORLD_WIDTH; x++ )
			for ( int y = 0; y < WORLD_HEIGHT; y++ )
				mTiles[ x ][ y ] = 0;
	}

	bool XmlMap::Load()
	{
		// Parse dom
		tinyxml2::XMLDocument doc;
		if ( doc.LoadFile( mFilePath.c_str() ) != XML_SUCCESS )
		{
			return false;
		}

		mTileSet.Clear();

		// Walk dom
		// <Map>
        XMLNode *map = NULL;
		if ( map = doc.FirstChildElement( "Map" ) )
		{
				// <TileSet>
				TiXmlNode *tileSet = NULL;
				if ( tileSet = map->FirstChildElement( "TileSet" ) )
				{
					TiXmlNode *tileDef = 0;
					while( tileDef = tileSet->IterateChildren( "TileDef", tileDef ) )
					{
						TileDef def;
						if ( !tileDef->ToElement() )
						{
							goto error;
						}

						const char* filepath = tileDef->ToElement()->Attribute("filepath");
					}
				}

			// <Tile>
            XMLNode *tiles = NULL;
			if ( tiles = map->FirstChildElement( "Tiles" ) )
			{
				// ForEach <TileRow>
                const XMLElement *tileRow;
				for ( tileRow = tiles->FirstChildElement( "TileRow" ); tileRow; tileRow = tileRow->NextSiblingElement( "TileRow" ) )
				{
					int x = 0;
					if ( !tileRow->ToElement() ||
							tileRow->ToElement()->QueryIntAttribute( "index", &x ) != XML_SUCCESS )
					{
						goto error;
					}

					// ForEach <Tile>
                    const XMLElement *tile;
                    for ( tile = tileRow->FirstChildElement( "Tile" ); tile; tile = tile->NextSiblingElement( "Tile" ) )
					{
						int y = 0;
						if ( !tile->ToElement() ||
								tile->ToElement()->QueryIntAttribute( "index", &y) != XML_SUCCESS )
						{
							goto error;
						}

							TileId t = (TileId)atoi( tile->ToElement()->GetText() );

						if ( x >= 0 && x < WORLD_WIDTH &&
								 y >= 0 && y < WORLD_HEIGHT )
						{
							mTiles[ x ][ y ] = t;
						}
						else
						{
							goto error;
						}
					}
				}
			}
			else
			{
				goto error;
			}
		}
		else
		{
			goto error;
		}

		return true; // success

error:
		mTileSet.Clear();
		PROCYON_WARN( "XmlMap", "Malformed map xml" );
		return false; // error
	}

	TileId XmlMap::GetTile( int x, int y ) const
	{
		return mTiles[ x ][ y ];
	}

} /* namespace Procyon */
