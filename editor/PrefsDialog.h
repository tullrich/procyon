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

#ifndef _PREFS_DIALOG_H
#define _PREFS_DIALOG_H

#include <QDialog>
#include "ProcyonCommon.h"

class QAbstractButton;
namespace Ui {
	class PrefsDialog;
}

class PrefsDialog : public QDialog
{
	Q_OBJECT
public:
    PrefsDialog( QWidget *parent = 0 );
    virtual ~PrefsDialog();

public slots:
	void LogFontSizeChanged( int size );
	void Apply();
	void OnDialogButton( QAbstractButton* button );

protected:
	// Designer layout
    Ui::PrefsDialog* 	mUi;
};

#endif /* _PREFS_DIALOG_H */
