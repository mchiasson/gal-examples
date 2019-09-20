#include <gal/gal.h>
#include <stdbool.h>
#include <stdio.h>

const char *exampleWindowTitle = __FILE__;

static GalDevice *device = NULL;
static GalSwapChain swapchain = GAL_HANDLE_INIT();

bool exampleInit(void *nativeWindowHandle, int fbWidth, int fbHeight)
{
    GalDeviceDescriptor deviceDesc;
    deviceDesc.type = GAL_DEVICE_TYPE_AUTO;
    deviceDesc.msaa = GAL_MSAA_OFF;
    deviceDesc.vsync = true;

    device = galDeviceCreate(deviceDesc);


    GalSwapChainDescriptor swapchainDesc;
    swapchainDesc.nativeWindowHandle = nativeWindowHandle;
    swapchainDesc.width = fbWidth;
    swapchainDesc.height = fbHeight;
    swapchainDesc.format = galDeviceGetSwapChainPreferredFormat(device);
    swapchainDesc.usage = GAL_TEXTURE_USAGE_OUTPUT_ATTACHMENT;
    swapchain = galDeviceConfigureSwapChain(device, swapchainDesc);
    return true;
}

void exampleFrame()
{
}

void exampleShutdown()
{
    galDeviceDestroy(device);
    device = NULL;
}
