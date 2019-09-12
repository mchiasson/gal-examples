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
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <stdbool.h>
#include <stdlib.h>

extern const char *exampleWindowTitle;
extern const int exampleWindowWidth;
extern const int exampleFramebufferHeight;
extern bool exampleInit(void *nativeWindowHandle, int fbWidth, int fbHeight);
extern void exampleFrame();
extern void exampleShutdown();

void *getNativeWindow(SDL_Window *window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        fprintf(stderr, "SDL_GetWindowWMInfo failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }

#if defined(SDL_VIDEO_DRIVER_COCOA)
    return (void *) wmi.info.cocoa.window;
#elif defined(SDL_VIDEO_DRIVER_WINDOWS)
    return (void *) wmi.info.win.window;
#elif defined(SDL_VIDEO_DRIVER_X11)
    return (void *) wmi.info.x11.window;
#else
#    error "Unsupported SDL Video Driver. Please submit a feature request at " \
           "https://github.com/mchiasson/gal/issues"
#endif
}

int main( int argc, char* args[] )
{
    if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    char title[256];
    snprintf(title, sizeof(title), "%s - SDL2", exampleWindowTitle);

    SDL_Window *window = SDL_CreateWindow(title,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1280,
                                          720,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int fbWidth, fbHeight;
    SDL_GL_GetDrawableSize(window, &fbWidth, &fbHeight);

    if (!exampleInit(getNativeWindow(window), fbWidth, fbHeight)) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Event event = {0};
    bool quit = false;
    while (!quit) {
        exampleFrame();

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    break;

                case SDL_WINDOWEVENT_CLOSE:
                    quit = true;
                    break;
                }
                break;
            }
        }
    }

    exampleShutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
