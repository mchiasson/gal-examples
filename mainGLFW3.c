/*******************************************************************************
 * Copyright (c) 2019 Mathieu-André Chiasson
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(__APPLE__)
#    define GLFW_EXPOSE_NATIVE_COCOA 1
#elif defined(_WIN32)
#    define GLFW_EXPOSE_NATIVE_WIN32 1
#elif defined(__linux__)
#    define GLFW_EXPOSE_NATIVE_X11 1
#endif
#include <GLFW/glfw3native.h>

extern const char *example_window_title;
extern const int example_window_width;
extern const int example_window_height;
extern bool example_init(void *native_window_handle);
extern void example_frame();

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

void *get_native_window(GLFWwindow *window)
{
#if defined(GLFW_EXPOSE_NATIVE_COCOA)
    return glfwGetCocoaWindow(window);
#elif GLFW_EXPOSE_NATIVE_WIN32
    return glfwGetWin32Window(window);
#elif defined(GLFW_EXPOSE_NATIVE_X11)
    return (void *) (uintptr_t) glfwGetX11Window(window);
#endif
}

int main( int argc, char* args[] )
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return -1;
    }

    char title[256];
    snprintf(title, sizeof(title), "%s - GLFW3", example_window_title);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(example_window_width,
                                          example_window_height,
                                          title,
                                          NULL,
                                          NULL);

    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    if (!example_init(get_native_window(window))) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    while (!glfwWindowShouldClose(window)) {
        example_frame();
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
