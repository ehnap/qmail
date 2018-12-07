QT += core network
QT -= gui

TARGET = QMail
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/smtp/qsmtpresponse.cpp \
    src/smtp/qsmtp.cpp \
    src/mime/qmimeobject.cpp \
    src/mime/qemail.cpp \
    src/mime/qmimetext.cpp \
    src/mime/qmimemultipart.cpp \
    src/mime/qmimeattach.cpp

HEADERS += \
    src/smtp/qsmtpresponse.h \
    src/smtp/qsmtp.h \
    src/mime/qmimeobject.h \
    src/mime/qemail.h \
    src/mime/qmimetext.h \
    src/mime/qmimemultipart.h \
    src/mime/qmimeattach.h

