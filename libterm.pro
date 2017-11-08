TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/term.cpp \
    src/event.cpp \
    src/font.cpp \
    src/key.cpp \
    src/color.cpp

HEADERS += \
    src/term.h \
    src/event.h \
    src/font.h \
    src/key.h \
    src/color.h \
    src/char.h

LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf
CONFIG += address_sanitizer
