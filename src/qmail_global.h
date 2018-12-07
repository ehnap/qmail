#pragma once

#include <QtCore/QtGlobal>

#ifndef QMAIL_STATIC
# if defined(QMAIL_LIB)
#  define QMAIL_EXPORT Q_DECL_EXPORT
# else
#  define QMAIL_EXPORT Q_DECL_IMPORT
# endif
#else
# define QMAIL_EXPORT
#endif

