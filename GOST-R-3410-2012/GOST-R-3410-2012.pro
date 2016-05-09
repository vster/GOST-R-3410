TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    convhex.cpp \
    G3410-2012.cpp \
    GOST-main.cpp \
    QxyNTL.cpp

HEADERS += \
    convhex.h \
    G3410-2012.h \
    QxyNTL.h

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -L/usr/local/lib -lntl
