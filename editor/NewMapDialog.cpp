#include "NewMapDialog.h"
#include "Collision/World.h"

#include <QPushButton>

#include "ui_new_map_dialog.h"

NewMapDialog::NewMapDialog( QWidget *parent /*= 0*/ )
	: QDialog( parent )
	, mUi( new Ui::NewMapDialog )
{
	mUi->setupUi( this );
	UpdatePixelDimensionsLabel();
	connect( mUi->widthSpinner, SIGNAL( valueChanged( int ) ), this, SLOT( UpdatePixelDimensionsLabel() ) );//
	connect( mUi->heightSpinner, SIGNAL( valueChanged( int ) ), this, SLOT( UpdatePixelDimensionsLabel() ) );//
}

NewMapDialog::~NewMapDialog()
{
	delete mUi;
}

void NewMapDialog::UpdatePixelDimensionsLabel()
{
	mUi->pixelDimensionsLabel->setText( QString::number( TILE_PIXEL_SIZE * mUi->widthSpinner->value() )
		+ " x " + QString::number( TILE_PIXEL_SIZE * mUi->heightSpinner->value() ) + " pixels" );
}

glm::ivec2 NewMapDialog::GetMapSize() const
{
	return glm::ivec2( mUi->widthSpinner->value(), mUi->heightSpinner->value() );
}
