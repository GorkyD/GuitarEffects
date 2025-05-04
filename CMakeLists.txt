cmake_minimum_required(VERSION 3.10)
project(GuitarEffects VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(RtAudio CONFIG REQUIRED)

# Core sources
set(CORE_SOURCES
    src/core/main.cpp
    src/core/EffectManager.cpp
)

set(CORE_HEADERS
    src/core/EffectManager.h
)

# UI sources
set(UI_SOURCES
    src/ui/MainWindow.cpp
)

set(UI_HEADERS
    src/ui/MainWindow.h
)

# Effect sources
set(EFFECT_SOURCES
    src/effects/Effect.cpp
    src/effects/DelayEffect.cpp
    src/effects/ReverbEffect.cpp
    src/effects/OverdriveEffect.cpp
)

set(EFFECT_HEADERS
    src/effects/Effect.h
    src/effects/DelayEffect.h
    src/effects/ReverbEffect.h
    src/effects/OverdriveEffect.h
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

add_executable(${PROJECT_NAME} 
    ${CORE_SOURCES} ${CORE_HEADERS}
    ${UI_SOURCES} ${UI_HEADERS}
    ${EFFECT_SOURCES} ${EFFECT_HEADERS}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

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