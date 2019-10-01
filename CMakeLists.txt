# Copyright (c) 2019 Mathieu-André Chiasson
# All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.10)

include(gal_shader_compile)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project(gal-examples
    DESCRIPTION "Graphic Abstraction Layer Examples - by Mathieu-André Chiasson"
)

############################## Build Deps ######################################

if(NOT TARGET gal AND NOT TARGET gal::gal)
    find_package(gal CONFIG REQUIRED)
endif()

set(GAL_EXAMPLE_WINDOW_SYSTEM "SDL2" CACHE STRING "Window System to use for GAL Examples")
set_property(CACHE GAL_EXAMPLE_WINDOW_SYSTEM PROPERTY STRINGS SDL2 GLFW3)

if (GAL_EXAMPLE_WINDOW_SYSTEM STREQUAL SDL2)
    find_package(SDL2 CONFIG REQUIRED)
elseif (GAL_EXAMPLE_WINDOW_SYSTEM STREQUAL GLFW3)
    find_package(glfw3 CONFIG REQUIRED)
endif()

############################ Example common ####################################

gal_shader_compile(
    OUTPUT
        example_shaders
    SOURCES
        shaders/color.vert
        shaders/color.frag
        shaders/default.vert
        shaders/default.frag
)

add_library(gal-examples-main
    main${GAL_EXAMPLE_WINDOW_SYSTEM}.c
    ${example_shaders}
)

target_include_directories(gal-examples-main PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(gal-examples-main PUBLIC
    $<$<TARGET_EXISTS:gal>:gal>
    $<$<TARGET_EXISTS:gal::gal>:gal::gal>
    $<$<TARGET_EXISTS:glfw>:glfw>
    $<$<TARGET_EXISTS:SDL2::SDL2>:SDL2::SDL2>
    $<$<TARGET_EXISTS:SDL2::SDL2main>:SDL2::SDL2main>
)

############################### Examples #######################################

add_executable(gal-example-01-HelloWorld gal-example-01-HelloWorld.c)
target_link_libraries(gal-example-01-HelloWorld gal-examples-main)
