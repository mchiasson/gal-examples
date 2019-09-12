#include <gal/gal.h>
#include <stdbool.h>
#include <stdio.h>

const char *exampleWindowTitle = "gal-example-01-HelloWorld";

static GalContext *ctx = NULL;
static GalVertexLayout layout = {0};
static GalStaticVertexBufferHandle vbo = {0};
static GalViewId viewId = 0;

bool exampleInit(void *nativeWindowHandle, int fbWidth, int fbHeight)
{
    ctx = galCreate(nativeWindowHandle, fbWidth, fbHeight);
    if (!ctx) {
        fprintf(stderr, "galCreate failed: %s\n", galGetError());
        return false;
    }

    galSetViewClear(ctx, viewId, GAL_CLEAR_ALL, GalColorPreset.DimGray, 1.0f, 0);

    return true;
}

void exampleFrame()
{
    galSubmit(ctx, viewId, (GalProgramHandle){0});
    galPresent(ctx) && galRender(ctx) && galSwapBuffers(ctx);
}

void exampleShutdown()
{
    galDestroy(ctx);
    ctx = NULL;
}
