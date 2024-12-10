cmake_minimum_required(VERSION 3.10)

project(main)

add_executable(main ${CMAKE_SOURCE_DIR}/sources/main.cpp ${CMAKE_SOURCE_DIR}/sources/sprite.cpp ${CMAKE_SOURCE_DIR}/sources/system.cpp
${CMAKE_SOURCE_DIR}/sources/network.cpp ${CMAKE_SOURCE_DIR}/sources/gamestate.cpp ${CMAKE_SOURCE_DIR}/sources/effects.cpp
${CMAKE_SOURCE_DIR}/sources/dungeon.cpp ${CMAKE_SOURCE_DIR}/sources/collision.cpp)

if (LINUX)
target_link_libraries(main sfml-audio sfml-graphics sfml-window sfml-network sfml-system c)
endif()

if (WIN32)
target_link_libraries(main sfml-audio sfml-graphics sfml-window sfml-network sfml-system opengl32 openal32 c)
endif()

set(CMAKE_CXX_FLAGS "-O2")