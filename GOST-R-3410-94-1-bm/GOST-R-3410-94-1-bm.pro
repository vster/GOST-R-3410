TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    convhex.cpp \
    G3410-94.cpp \
    GOST-main.cpp

HEADERS += \
    convhex.h \
    G3410-94.h

INCLUDEPATH += \
    ../../include

LIBS += \
    -L../../lib -lntl
