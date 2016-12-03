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
#ifndef _EDITOR_SETTINGS_H
#define _EDITOR_SETTINGS_H

#include <QObject>

class Editor;

class EditorSettings : public QObject
{
	 Q_OBJECT

public:
	EditorSettings( Editor *parent );

	static EditorSettings& Get();

	void Load();
	void Save();

    Q_PROPERTY(bool GridVisible READ GetGridVisible WRITE SetGridVisible MEMBER mGridVisible NOTIFY GridVisibilityChanged)
    Q_PROPERTY(float GridSize READ GetGridSize WRITE SetGridSize MEMBER mGridSize NOTIFY GridSizeChanged)
    Q_PROPERTY(int LogFontSize READ GetLogFontSize WRITE SetLogFontSize MEMBER mLogFontSize NOTIFY LogFontSizeChanged)

    bool GetGridVisible() const { return mGridVisible; }
    void SetGridVisible( bool visible )
    {
    	mGridVisible = visible;
    	emit GridVisibilityChanged( mGridVisible );
    }

    float GetGridSize() const { return mGridSize; }
    void SetGridSize( float size )
    {
    	mGridSize = size;
    	emit GridSizeChanged( mGridSize );
    }

	int GetLogFontSize() const { return mLogFontSize; }
	void SetLogFontSize( int size )
	{
		mLogFontSize = size;
    	emit LogFontSizeChanged( mLogFontSize );
	}

signals:
	void GridVisibilityChanged( bool visibility );
	void GridSizeChanged( float gridSize );
	void LogFontSizeChanged( int gridSize );


protected:
    Editor *mEditor;

	bool mGridVisible;
	float mGridSize;

	int mLogFontSize;
};

#endif /* _EDITOR_SETTINGS_H */
