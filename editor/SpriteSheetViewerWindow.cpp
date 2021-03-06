#include "SpriteSheetViewerWindow.h"
#include "SpriteSheetViewerCanvas.h"
#include "Graphics/AnimatedSprite.h"

#include <QCloseEvent>

#include "ui_sprite_sheet_viewer_window.h"

SpriteSheetViewerWindow::SpriteSheetViewerWindow( QString spritePath, QWidget *parent /*= 0*/ )
	: QMainWindow( parent )
	, mUi( new Ui::SpriteSheetViewerWindow )
    , mSprite( new Procyon::AnimatedSprite() )
{
	mUi->setupUi( this );

    // Setup canvas
    QVBoxLayout* canvasLayout = new QVBoxLayout( mUi->canvasHolder );
    canvasLayout->addWidget( new SpriteSheetViewerCanvas( this, mSprite, spritePath ) );

    mSprite->Play();

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
