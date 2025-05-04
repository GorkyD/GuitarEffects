QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuitarEffects
TEMPLATE = app

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Effect.cpp \
    EffectManager.cpp \
    DelayEffect.cpp \
    ReverbEffect.cpp \
    OverdriveEffect.cpp

HEADERS += \
    MainWindow.h \
    Effect.h \
    EffectManager.h \
    DelayEffect.h \
    ReverbEffect.h \
    OverdriveEffect.h

# Добавляем RtAudio
LIBS += -lrtaudio

# Настройки компилятора
QMAKE_CXXFLAGS += -std=c++17 