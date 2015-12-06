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
#ifndef _LOG_OUTPUT_TARGET_H
#define _LOG_OUTPUT_TARGET_H

#include <QPlainTextEdit>

#include "ProcyonCommon.h"

namespace Procyon {
	
	class LogOutputTarget;

	class LogOutputView : public QPlainTextEdit
	{
    	Q_OBJECT

	public:
				LogOutputView( QWidget* parent );
		virtual ~LogOutputView();

	signals:
		void AppendLog( const QString &html );

	protected:
		LogOutputTarget* mTarget;
	};
	
} /* namespace Procyon */

#endif /* _LOG_OUTPUT_TARGET_H */
