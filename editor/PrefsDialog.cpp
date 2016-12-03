#include "PrefsDialog.h"

#include <QPushButton>

#include "ui_prefs_dialog.h"
#include "EditorSettings.h"

PrefsDialog::PrefsDialog( QWidget *parent /*= 0*/ )
	: QDialog( parent )
    , mUi( new Ui::PrefsDialog )
{
    mUi->setupUi( this );

	mUi->logFontSize->setValue( EditorSettings::Get().GetLogFontSize() );

	connect( mUi->buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( OnDialogButton( QAbstractButton* ) ) );//
	connect( this, SIGNAL( accepted() ), this, SLOT( Apply() ) );
}

PrefsDialog::~PrefsDialog()
{
    delete mUi;
}

void PrefsDialog::LogFontSizeChanged( int size )
{
	EditorSettings::Get().SetLogFontSize( size );
}

void PrefsDialog::OnDialogButton( QAbstractButton* button )
{
	if ( mUi->buttonBox->buttonRole( button ) == QDialogButtonBox::ApplyRole )
	{
		emit Apply();
	}
}

void PrefsDialog::Apply()
{
	EditorSettings::Get().SetLogFontSize( mUi->logFontSize->value() );
}
