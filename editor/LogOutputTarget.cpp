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
#include <Qt>
#include <QTextBlock>

#include "LogOutputTarget.h"
#include "EditorSettings.h"

using namespace logog;

namespace Procyon {

	static const char* ErrorLevelDescription( int level )
	{
		switch( level )
		{
			case LOGOG_LEVEL_EMERGENCY: return "Emergency";
			case LOGOG_LEVEL_ALERT: 	return "Alert";
			case LOGOG_LEVEL_CRITICAL: 	return "Critical";
			case LOGOG_LEVEL_ERROR: 	return "Error";
			case LOGOG_LEVEL_WARN: 		return "Warn";
			case LOGOG_LEVEL_WARN1: 	return "Warn 1";
			case LOGOG_LEVEL_WARN2: 	return "Warn 2";
			case LOGOG_LEVEL_WARN3: 	return "Warn 3";
			case LOGOG_LEVEL_INFO: 		return "Info";
			case LOGOG_LEVEL_DEBUG: 	return "Debug";
			case LOGOG_LEVEL_ALL: 		return "All";
			case LOGOG_LEVEL_NONE:		// Intentional fall through
			default:  					return "None";
		}
	}

	static QColor ErrorLevelColor( int level )
	{
		switch( level )
		{
			case LOGOG_LEVEL_EMERGENCY: return Qt::red;
			case LOGOG_LEVEL_ALERT: 	return Qt::red;
			case LOGOG_LEVEL_CRITICAL: 	return Qt::red;
			case LOGOG_LEVEL_ERROR: 	return Qt::red;
			case LOGOG_LEVEL_WARN: 		return Qt::darkYellow;
			case LOGOG_LEVEL_WARN1: 	return Qt::darkYellow;
			case LOGOG_LEVEL_WARN2: 	return Qt::darkYellow;
			case LOGOG_LEVEL_WARN3: 	return Qt::darkYellow;
			case LOGOG_LEVEL_INFO: 		return Qt::darkGray;
			case LOGOG_LEVEL_DEBUG: 	return Qt::blue;
			case LOGOG_LEVEL_ALL: 		return Qt::darkGray;
			case LOGOG_LEVEL_NONE:		// Intentional fall through
			default:  					return Qt::darkGray;
		}
	}

	class LogOutputFormatter : public Formatter
	{
	public:
		virtual TOPIC_FLAGS GetTopicFlags( const Topic &topic )
		{
			return 0;
		}

		virtual LOGOG_STRING& Format( const Topic &topic, const Target &target )
		{
			m_sMessageBuffer.clear();

			QColor c = ErrorLevelColor( topic.Level() );

			m_sMessageBuffer.format( "<span class=\"msg\"><span style=\"color:rgb(%i,%i,%i);\">%s:</span> %s</span>"
				, c.red(), c.green(), c.blue()
				, ErrorLevelDescription( topic.Level() )
				, topic.Message().c_str() );

			return m_sMessageBuffer;
		}
	};

	class LogOutputTarget : public logog::Target
	{
	public:
		LogOutputTarget( LogOutputView* textEdit )
			: mTextEdit( textEdit )
			, mFormatter( new LogOutputFormatter() )
		{
			SetFormatter( *mFormatter );
		}

		virtual ~LogOutputTarget()
		{
			delete mFormatter;
		}

		virtual int Output( const LOGOG_STRING &data )
		{
			emit mTextEdit->AppendLog( QString( data ) );
			return 0;
		}

	protected:
		LogOutputView* 			mTextEdit;
		LogOutputFormatter*		mFormatter;
	};


	LogOutputView::LogOutputView( QWidget* parent )
		: QPlainTextEdit( parent )
		, mTarget( new LogOutputTarget( this ) )
		, mFontSize( -1 )
	{

	    setObjectName(QStringLiteral("outputLogger"));
	    setReadOnly(true);
	    setMaximumBlockCount(200);

		connect( this, SIGNAL( AppendLog( const QString& ) )
			,    this, SLOT( appendHtml( const QString& ) )
			, 	 Qt::AutoConnection ); // explicit default

		SetLogFontSize( EditorSettings::Get().GetLogFontSize() );
		connect( &EditorSettings::Get(), SIGNAL( LogFontSizeChanged( int ) )
			, 	 this, SLOT( SetLogFontSize( int ) ) );
	}

	LogOutputView::~LogOutputView()
	{
		delete mTarget;
	}

	void LogOutputView::SetLogFontSize( int size )
	{
		if ( size == mFontSize )
			return;

		document()->setDefaultStyleSheet(
			QString(".msg { font-size:%1px; }").arg( size )
		);
		mFontSize = size;

		QTextDocument* doc = document();
		doc->clear();
	}

} /* namespace Procyon */
