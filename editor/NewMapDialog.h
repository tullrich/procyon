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

#ifndef _NEW_MAP_DIALOG_H
#define _NEW_MAP_DIALOG_H

#include <QDialog>
#include "ProcyonCommon.h"

class QAbstractButton;
namespace Ui {
	class NewMapDialog;
}

class NewMapDialog : public QDialog
{
	Q_OBJECT
public:
	NewMapDialog( QWidget *parent = 0 );
	virtual ~NewMapDialog();

	glm::ivec2 GetMapSize() const;
public slots:
	void UpdatePixelDimensionsLabel();

protected:
	// Designer layout
    Ui::NewMapDialog* 	mUi;
};

#endif /* _NEW_MAP_DIALOG_H */
