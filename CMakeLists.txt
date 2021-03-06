cmake_minimum_required(VERSION 3.8)
project(game)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall -Wextra -Werror")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")


set(SOURCE_FILES Source/Main.cpp Source/Application.h Source/Application.cpp Source/QuadBoard.h Source/QuadBoard.cpp Source/Config.h)

set(RESOURCE_FILES font/arial.ttf)


find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    link_libraries(${OPENGL_LIBRARIES})
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIRS})
    link_libraries(${SFML_LIBRARIES})
endif()

find_package(Threads REQUIRED)
if (THREADS_FOUND)
    link_libraries(Threads::Threads)
endif()

add_executable(game ${SOURCE_FILES}) 
