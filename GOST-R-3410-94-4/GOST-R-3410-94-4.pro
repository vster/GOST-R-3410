TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    G3410-94.cpp \
    GOST-main.cpp \
    zzhex.cpp

HEADERS += \
    G3410-94.h \
    zzhex.h

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -L/usr/local/lib -lntl
