cmake_minimum_required(VERSION 3.10)

project(main)

add_executable(main ${CMAKE_SOURCE_DIR}/sources/main.cpp ${CMAKE_SOURCE_DIR}/sources/sprite.cpp ${CMAKE_SOURCE_DIR}/sources/system.cpp
${CMAKE_SOURCE_DIR}/sources/network.cpp ${CMAKE_SOURCE_DIR}/sources/gamestate.cpp ${CMAKE_SOURCE_DIR}/sources/effects.cpp
${CMAKE_SOURCE_DIR}/sources/dungeon.cpp ${CMAKE_SOURCE_DIR}/sources/collision.cpp)

if (LINUX)
target_link_libraries(main sfml-audio sfml-graphics sfml-window sfml-network sfml-system openal)
set(CMAKE_CXX_FLAGS "-O2")
endif()

if (WIN32)
target_link_libraries(main sfml-graphics sfml-window sfml-audio sfml-system sfml-network ws2_32 FLAC openal32 vorbisenc vorbisfile vorbis ogg opengl32 freetype winmm gdi32)
set(CMAKE_CXX_FLAGS "-static -O2")
endif()
