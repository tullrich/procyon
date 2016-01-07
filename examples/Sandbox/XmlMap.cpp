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

#include <tinyxml.h>

namespace Procyon {

	static int sindent( char *buf, int indent, char c = ' ' )
	{
		for ( int i = 0; i < indent; ++i )
		{
			buf += sprintf( buf, "%c", c );
		}
		return indent;
	}

	static void print_attributes( TiXmlElement *element, int indent )
	{
		char buf[1024];
		char *msg = buf;

		msg += sindent( msg, indent );

		TiXmlAttribute* attrib = element->FirstAttribute();

		while ( attrib )
		{
			sprintf( msg, "%s: value=[%s]", attrib->Name(), attrib->Value() );
			PROCYON_DEBUG( "XmlMap", "%s", buf );
			attrib = attrib->Next();
		}
	}

	static void print_dom( TiXmlNode *node, int indent = 0 )
	{
		char buf[1024];
		char *msg = buf;

		msg += sindent( msg, indent );

		int t = node->Type();
		switch ( t )
		{
		case TiXmlNode::TINYXML_DOCUMENT:
			msg += sprintf( msg, "Document" );
			break;
		case TiXmlNode::TINYXML_ELEMENT:
			msg += sprintf( msg, "Element [%s]", node->Value() );
			break;
		case TiXmlNode::TINYXML_COMMENT:
			msg += sprintf( msg, "Comment: [%s]", node->Value());
			break;
		case TiXmlNode::TINYXML_UNKNOWN:
			msg += sprintf( msg, "Unknown" );
			break;
		case TiXmlNode::TINYXML_TEXT:
			msg += sprintf( msg, "Text: [%s]", node->ToText()->Value() );
			break;
		case TiXmlNode::TINYXML_DECLARATION:
			msg += sprintf( msg, "Declaration" );
			break;
		default:
			msg += sprintf( msg, "Really Unknown" );
			break;
		}

		PROCYON_DEBUG( "XmlMap", "%s", buf );

		if ( t == TiXmlNode::TINYXML_ELEMENT )
		{
			print_attributes( node->ToElement(), indent + 1 );
		}

		for ( TiXmlNode *child = node->FirstChild(); child != 0; child = child->NextSibling()) 
		{
			print_dom( child, indent + 1 );
		}
	}

	XmlMap::XmlMap( const std::string& filePath )
		: mFilePath( filePath )
	{
		for ( int x = 0; x < WORLD_WIDTH; x++ )
			for ( int y = 0; y < WORLD_HEIGHT; y++ )
				mTiles[ x ][ y ] = TILE_EMPTY;
	}

	bool XmlMap::Load()
	{
		// Parse dom
		TiXmlDocument doc( mFilePath );
		if ( !doc.LoadFile() )
		{
			return false;
		}

		// Walk dom
		if ( doc.Type() == TiXmlNode::TINYXML_DOCUMENT )
		{
			// <Map>
			TiXmlNode *map = NULL;
			if ( map = doc.FirstChildElement( "Map" ) )
			{
				// <Tile>
				TiXmlNode *tiles = NULL;
				if ( tiles = map->FirstChildElement( "Tiles" ) )
				{
					// ForEach <TileRow>
					TiXmlNode *tileRow = 0;
					while( tileRow = tiles->IterateChildren( "TileRow", tileRow ) )
					{
						int x = 0;
						if ( !tileRow->ToElement() ||
							 tileRow->ToElement()->QueryIntAttribute( "index", &x ) != TIXML_SUCCESS )
						{
							goto error;
						}

						// ForEach <Tile>
						TiXmlNode *tile = 0;
						while( tile = tileRow->IterateChildren( "Tile", tile ) )
						{
							int y = 0;
							if ( !tile->ToElement() ||
								 tile->ToElement()->QueryIntAttribute( "index", &y) != TIXML_SUCCESS )
							{
								goto error;
							}

							TileType t = (TileType)atoi( tile->ToElement()->GetText() );

							if ( x >= 0 && x < WORLD_WIDTH &&
								 y >= 0 && y < WORLD_HEIGHT &&
								 t >= 0 && t < TileType::TILE_TYPE_COUNT )
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
		}
		else
		{
			goto error;
		}

		return true; // success

error:
		PROCYON_WARN( "XmlMap", "Malformed map xml" );
		return false; // error
	}

	TileType XmlMap::GetTileType( int x, int y ) const
	{
		return mTiles[ x ][ y ];
	}

} /* namespace Procyon */
