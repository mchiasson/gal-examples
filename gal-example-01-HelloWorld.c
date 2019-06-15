#include <gal/gal.h>
#include <stdbool.h>
#include <stdio.h>

const char *example_window_title = "gal-example-01-HelloWorld";
const int example_window_width = 800;
const int example_window_height = 600;

static gal_context *ctx = NULL;

bool example_init(void *native_window_handle)
{
    ctx = gal_create_default_context(native_window_handle);
    if (!ctx) {
        fprintf(stderr, "gal_create_default_context failed: %s\n", gal_get_error());
        return false;
    }
    return true;
}

void example_frame() {}
