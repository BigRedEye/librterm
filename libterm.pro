TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    term.cpp \
    event.cpp \
    font.cpp \
    key.cpp

HEADERS += \
    term.h \
    event.h \
    font.h \
    key.h

LIBS += -lSDL2 -lSDL2_image
CONFIG += address_sanitizer
