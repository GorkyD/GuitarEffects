cmake_minimum_required(VERSION 3.10)
project(GuitarEffects)

add_executable(Delay Delay.cpp)
add_executable(Reverb Reverb.cpp)
add_executable(Overdrive Overdrive.cpp)

find_package(RtAudio CONFIG REQUIRED)
target_link_libraries(Delay PRIVATE RtAudio::rtaudio)
target_link_libraries(Reverb PRIVATE RtAudio::rtaudio)
target_link_libraries(Overdrive PRIVATE RtAudio::rtaudio)