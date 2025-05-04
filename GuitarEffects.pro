QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuitarEffects
TEMPLATE = app

# Core sources
SOURCES += \
    src/core/main.cpp \
    src/core/EffectManager.cpp

HEADERS += \
    src/core/EffectManager.h

# UI sources
SOURCES += \
    src/ui/MainWindow.cpp

HEADERS += \
    src/ui/MainWindow.h

# Effect sources
SOURCES += \
    src/effects/Effect.cpp \
    src/effects/DelayEffect.cpp \
    src/effects/ReverbEffect.cpp \
    src/effects/OverdriveEffect.cpp

HEADERS += \
    src/effects/Effect.h \
    src/effects/DelayEffect.h \
    src/effects/ReverbEffect.h \
    src/effects/OverdriveEffect.h

# Resources
RESOURCES += \
    resources/resources.qrc

LIBS += -lrtaudio

QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH += src 