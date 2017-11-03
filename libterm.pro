TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    term.cpp \
    event.cpp \
    font.cpp \
    key.cpp \
    color.cpp

HEADERS += \
    term.h \
    event.h \
    font.h \
    key.h \
    color.h

LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf
CONFIG += address_sanitizer
