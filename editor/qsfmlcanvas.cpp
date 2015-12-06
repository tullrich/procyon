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

#include "qsfmlcanvas.h"
#include <QPalette>
#include <QResizeEvent>
#include <iostream> 
#include <X11/Xlib.h>
#include <QX11Info>
#include <GL/gl.h>

QSFMLCanvas::QSFMLCanvas(QWidget* Parent)
	: QWidget       (Parent)
	, myInitialized (false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
	setFocusPolicy(Qt::StrongFocus);

    setSizePolicy( QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored );

    // Setup the timer
    myTimer.setInterval(0);

    shape = new sf::CircleShape(100.f);
    shape->setFillColor(sf::Color::Green);
}

QSFMLCanvas::~QSFMLCanvas()
{
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
	std::cout << "showevent" << std::endl;
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows`
        XFlush(QX11Info::display());

        // Create the SFML window with the widget handle
        sf::RenderWindow::create(winId());

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

bool QSFMLCanvas::event( QEvent* event )
{
    std::cout << "qt::Event" << std::endl;
	return QWidget::event(event);
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{

    sf::RenderWindow::clear(sf::Color(128, 0, 0));

    shape->move(1, 0);

	sf::RenderWindow::draw(*shape);

    // Display on screen
    sf::RenderWindow::display();
}

void QSFMLCanvas::resizeEvent ( QResizeEvent * event )
{
    unsigned width = event->size().width();
    unsigned height = event->size().height();

    std::cout << "qt::resizeEvent <" << width << ", " << height << ">" << std::endl;

    //sf::RenderWindow::setSize( sf::Vector2u( width, height ) );
    std::cout << "sf::getSize <" << sf::RenderWindow::getSize().x << ", " << sf::RenderWindow::getSize().y << ">" << std::endl;


    //float a = height/(300*2);
    //view.setViewport(sf::FloatRect(0,(-a+1),width/300,height/300));
    //sf::RenderWindow::create(winId());

    sf::View view( sf::FloatRect(0, 0, width, height ) );
    view.setViewport( sf::FloatRect(0, 0, 1, 1 ) );
    //sf::RenderWindow::setView( view );

    std::cout << "sf::getView <" 
        << sf::RenderWindow::getView().getCenter().x << ", " 
        << sf::RenderWindow::getView().getCenter().y << ", " 
        << sf::RenderWindow::getView().getSize().x << ", " 
        << sf::RenderWindow::getView().getSize().y << "> viewport: " 
        << sf::RenderWindow::getView().getViewport().top << ", " 
        << sf::RenderWindow::getView().getViewport().left << ", " 
        << sf::RenderWindow::getView().getViewport().width << ", " 
        << sf::RenderWindow::getView().getViewport().height << ">" 
        << std::endl;

    QWidget::resizeEvent( event );
}

void QSFMLCanvas::OnUpdate()
{
    sf::Event event;
    while ( pollEvent(event) )
    {
        if ( event.type == sf::Event::Resized ) {
            std::cout << "sf::Resized" << std::endl;
            //glViewport(0, 0, event.size.Width, event.size.Height);
            sf::RenderWindow::setView( sf::View( sf::FloatRect(0, 0 ,event.size.width, event.size.height) ) );
        }

        // ... check mouse and keys etc
    }
}

void QSFMLCanvas::mouseReleaseEvent ( QMouseEvent * event )
{
     std::cout << "sf::click1" << std::endl;
}