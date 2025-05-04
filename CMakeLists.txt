cmake_minimum_required(VERSION 3.10)
project(GuitarEffects VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

find_package(RtAudio CONFIG REQUIRED)

set(SOURCES
    main.cpp
    MainWindow.cpp
    Effect.cpp
    EffectManager.cpp
    DelayEffect.cpp
    ReverbEffect.cpp
    OverdriveEffect.cpp
)

set(HEADERS
    MainWindow.h
    Effect.h
    EffectManager.h
    DelayEffect.h
    ReverbEffect.h
    OverdriveEffect.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    RtAudio::rtaudio
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::Core>
            $<TARGET_FILE:Qt5::Gui>
            $<TARGET_FILE:Qt5::Widgets>
            $<TARGET_FILE:RtAudio::rtaudio>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()