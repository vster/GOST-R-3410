TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    convhex.cpp \
    GOST-R-3410-94-2.cpp

HEADERS += \
    convhex.h

INCLUDEPATH += \
    ../../include

LIBS += \
    -L../../lib -lntl
