QT += core network
QT -= gui

TARGET = qmail
CONFIG += c++11

TEMPLATE = lib

DEFINES += QT_DLL QMAIL_LIB

MOC_DIR += ./moc
PRECOMPILED_HEADER = stdafx.h
PRECOMPILED_SOURCE = stdafx.cpp

SOURCES += \
    src/smtp/qsmtpresponse.cpp \
    src/smtp/qsmtp.cpp \
    src/mime/qmimeobject.cpp \
    src/mime/qemail.cpp \
    src/mime/qmimetext.cpp \
    src/mime/qmimemultipart.cpp \
    src/mime/qmimeattach.cpp

HEADERS += \
    src/qmail_global.h \
    src/smtp/qsmtpresponse.h \
    src/smtp/qsmtp.h \
    src/mime/qmimeobject.h \
    src/mime/qemail.h \
    src/mime/qmimetext.h \
    src/mime/qmimemultipart.h \
    src/mime/qmimeattach.h

