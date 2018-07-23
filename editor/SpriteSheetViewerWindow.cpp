#include "SpriteSheetViewerWindow.h"
#include <QCloseEvent>

#include "ui_sprite_sheet_viewer_window.h"

SpriteSheetViewerWindow::SpriteSheetViewerWindow( QWidget *parent /*= 0*/ )
	: QMainWindow( parent )
	, mUi( new Ui::SpriteSheetViewerWindow )
{
	mUi->setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
}

SpriteSheetViewerWindow::~SpriteSheetViewerWindow()
{
	delete mUi;
}

void SpriteSheetViewerWindow::closeEvent( QCloseEvent *event )
{
    QMainWindow::closeEvent(event);
    emit Close();
}
