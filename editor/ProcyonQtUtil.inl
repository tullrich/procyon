#ifndef PROCYON_QT_UTIL_INL
#define PROCYON_QT_UTIL_INL

inline std::string ProcyonQtUtil::QStringToUtf8( const QString& str )
{
	return std::string( str.toUtf8().data() );
}

inline std::string ProcyonQtUtil::QStringToUtf8( const QStringRef& str )
{
	return std::string( str.toUtf8().data() );
}

#endif /* PROCYON_QT_UTIL_INL */