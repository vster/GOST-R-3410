TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    convhex.cpp \
    G3410-2001.cpp \
    GOST-main.cpp \
    QxyNTL.cpp

HEADERS += \
    convhex.h \
    G3410-2001.h \
    QxyNTL.h

INCLUDEPATH += \
    ../../include

LIBS += \
    -L../../lib -lntl
