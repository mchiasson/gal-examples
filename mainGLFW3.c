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
#include <GLFW/glfw3.h>

extern const char *example_window_title;
extern int example_window_width;
extern int example_window_height;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main( int argc, char* args[] )
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(example_window_width,
                                          example_window_height,
                                          example_window_title,
                                          nullptr,
                                          nullptr);

    if(!window)
    {
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    {
        // tunis::Context can only be instantiated after a context is created
        // and made current
        std::unique_ptr<SampleApp>app = SampleApp::create();

        std::stringstream title;
        title << SampleApp::getSampleName() << " - " << app->ctx.backendName();

        glfwSetWindowTitle(window, title.str().c_str());

        std::string frameName = std::string("Frame(") + app->ctx.backendName() + ")";
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            int winWidth, winHeight, fbWidth , fbHeight;
            glfwGetWindowSize(window, &winWidth, &winHeight);
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

            // Calculate pixel ration for hi-dpi devices.
            float pxRatio = static_cast<float>(fbWidth) / static_cast<float>(winWidth);

            double frameTime = glfwGetTime();
            app->ctx.clearFrame(0, 0, fbWidth, fbHeight);
            app->ctx.beginFrame(winWidth, winHeight, pxRatio);
            app->render(frameTime);
            app->ctx.endFrame();

            glfwSwapBuffers(window);
        }
    }


    glfwTerminate();

    return 0;
}
