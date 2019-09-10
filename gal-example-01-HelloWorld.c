#include <gal/gal.h>
#include <stdbool.h>
#include <stdio.h>

const char *example_window_title = "gal-example-01-HelloWorld";
const int example_window_width = 800;
const int example_window_height = 600;

static GalContext *ctx = NULL;

bool example_init(void *native_window_handle)
{
    ctx = galCreate(native_window_handle);
    if (!ctx) {
        fprintf(stderr, "gal_context_create failed: %s\n", galGetError());
        return false;
    }

    galSetLayerViewport(ctx, 0, 0, 0, example_window_width, example_window_height);
    galSetLayerClear(ctx, 0, GAL_CLEAR_ALL, GalColorPreset.Red, 1.0f, 0);

    return true;
}

void example_frame()
{
    galPresent(ctx) && galRender(ctx) && galSwapBuffers(ctx);
}

void example_shutdown()
{
    galDestroy(ctx);
    ctx = NULL;
}
