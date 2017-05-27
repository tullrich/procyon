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
#include <QApplication>
#include <QSurfaceFormat>
#include <QGLFormat>

#include "Editor.h"
#include "ProcyonCommon.h"

int main(int argc, char *argv[])
{
	int ret;
    LOGOG_INITIALIZE();
    {
        logog::GetFilterDefault().Group( "RawInput" );
        logog::Cout err;

		QGLFormat glFormat;
		glFormat.setVersion( 3, 3 );
		glFormat.setProfile( QGLFormat::CompatibilityProfile ); // Requires >=Qt-4.8.0
		glFormat.setSampleBuffers( true );
		glFormat.setSamples( 4 );
		//QGLFormat::setDefaultFormat( glFormat );

		QApplication app( argc, argv  );

		Editor *ef = new Editor();
	    ef->show();

	    ret = app.exec();
    }
    LOGOG_SHUTDOWN();
    return ret;
}
