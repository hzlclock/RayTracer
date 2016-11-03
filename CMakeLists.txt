cmake_minimum_required(VERSION 3.6)
project(project2_cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        boolean.cpp
        boolean.h
        camera.cpp
        camera.h
        cylinder.cpp
        cylinder.h
        driver.cpp
        driver.h
        dump_png.cpp
        film.cpp
        film.h
        flat_shader.cpp
        flat_shader.h
        grid.h
        hit.h
        light.h
        main.cpp
        object.h
        parse.cpp
        phong_shader.cpp
        phong_shader.h
        plane.cpp
        plane.h
        point_light.h
        ray.h
        reflective_shader.cpp
        reflective_shader.h
        render_world.cpp
        render_world.h
        shader.h
        sphere.cpp
        sphere.h
        vec.h)

add_executable(project2_cmake ${SOURCE_FILES})

IF (APPLE)
    INCLUDE_DIRECTORIES(/System/Library/Frameworks)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(GLUT_LIBRARY GLUT)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
    MARK_AS_ADVANCED(COCOA_LIBRARY
            GLUT_LIBRARY
            OpenGL_LIBRARY)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY} -lpng)
ENDIF (APPLE)

target_link_libraries(project2_cmake ${EXTRA_LIBS})