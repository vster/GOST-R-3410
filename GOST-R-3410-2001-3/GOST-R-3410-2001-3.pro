TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    G3410-2001.cpp \
    GOST-main.cpp \
    QxyNTL.cpp \
    zzhex.cpp

HEADERS += \
    G3410-2001.h \
    QxyNTL.h \
    zzhex.h

INCLUDEPATH += \
    ../../include

LIBS += \
    -L../../lib -lntl

