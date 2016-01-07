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
#include "EditorSettings.h"
#include "editor.h"
#include "RecentFileList.h"

#include "Logging.h"

#include <QSettings>

EditorSettings *gSettings;

EditorSettings& EditorSettings::Get()
{
	Q_ASSERT( gSettings );
	return *gSettings;
}

EditorSettings::EditorSettings( Editor *parent )
	: QObject( parent )
	, mEditor( parent )
	, mGridVisible( true )
	, mGridSize( 5.0f )
{
	Q_ASSERT( !gSettings );
	gSettings = this;
}

void EditorSettings::Load()
{
    QSettings settings( "tullrich", "Procyon Editor" );

    // Window settings
    settings.beginGroup( "Window" );
    mEditor->restoreGeometry( settings.value( "geometry" ).toByteArray() );
    mEditor->restoreState( settings.value( "window_state" ).toByteArray() );
    mEditor->GetRecent()->SetFiles( settings.value( "recent_files" ).toStringList() );

    settings.endGroup(); // Window

    // Canvas settings
    settings.beginGroup( "Canvas" );
    SetGridVisible( settings.value( "grid_visible", mGridVisible).toBool() );
    SetGridSize( settings.value( "grid_size", mGridSize ).toFloat() );
    settings.endGroup(); // Camera

    PROCYON_DEBUG( "EditorSettings", "EditorSettings loaded: %s, Grid Size %f"
    	, (mGridVisible) ? "Grid Visibile" : "Grid Hidden"
    	, mGridSize );
}

void EditorSettings::Save()
{
    QSettings settings( "tullrich", "Procyon Editor" );

    // Window settings
    settings.beginGroup( "Window" );
    settings.setValue( "geometry", mEditor->saveGeometry() );
    settings.setValue( "window_state", mEditor->saveState() );
    settings.setValue( "recent_files", mEditor->GetRecent()->GetFiles() );
    settings.endGroup(); // Window

    // Canvas settings
    settings.beginGroup( "Canvas" );
    settings.setValue( "grid_visible", mGridVisible );
    settings.setValue( "grid_size", mGridSize );
    settings.endGroup(); // Camera

    PROCYON_DEBUG( "EditorSettings", "EditorSettings saved: %s, Grid Size %f"
    	, (mGridVisible) ? "Grid Visibile" : "Grid Hidden"
    	, mGridSize );
}