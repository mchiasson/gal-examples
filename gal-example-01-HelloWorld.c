#include <gal/gal.h>
#include <string.h> // memcpy
#include "example_shaders.h"

const char *exampleWindowTitle = __FILE__;

static GalDevice *device = NULL;
static GalSwapChain swapchain = {0};
static GalShaderModule vertexShader = {0};
static GalShaderModule fragmentShader = {0};

typedef struct Vertex {
    float x, y;
    GalColor color;
} Vertex;

bool exampleInit(void *nativeWindowHandle, int fbWidth, int fbHeight)
{
    /*** Device Setup ***/

    GalDeviceDescriptor deviceDesc;
    deviceDesc.type = GAL_DEVICE_TYPE_AUTO;
    deviceDesc.msaa = GAL_MSAA_OFF;
    deviceDesc.vsync = true;

    device = galDeviceCreate(&deviceDesc);

    /*** Shader Setup ***/

    vertexShader = galDeviceCreateShaderModule(device,
                                               GAL_SHADER_MODULE_VERTEX,
                                               example_shaders,
                                               "shaders/color.vert");

    fragmentShader = galDeviceCreateShaderModule(device,
                                                 GAL_SHADER_MODULE_FRAGMENT,
                                                 example_shaders,
                                                 "shaders/color.frag");

    /*** Vertex Buffer Setup ***/

    Vertex vertexData[] = {
        { 0.0f,  0.8f, {  0, 255, 255, 255}},
        {-0.8f, -0.8f, {255, 255,   0, 255}},
        { 0.8f, -0.8f, {255,   0, 255, 255}}
    };

    const int colorOffset = offsetof(Vertex, color);

    GalBufferDescriptor vertexDataBufferDescriptor;
    vertexDataBufferDescriptor.bufferSize = sizeof(vertexData);
    vertexDataBufferDescriptor.usage = GAL_BUFFER_USAGE_MAP_WRITE | GAL_BUFFER_USAGE_VERTEX;

    GalBuffer vertexBuffer = galDeviceCreateBuffer(device, &vertexDataBufferDescriptor);

    /*** Write Data To GPU ***/

    galBufferSetSubData(vertexBuffer, 0, sizeof(vertexData), vertexData);

    /*** Describe Vertex Data For Pipeline ***/

    GalVertexAttributeDescriptor positionAttribute;
    positionAttribute.shaderLocation = 0;
    positionAttribute.offset = 0;
    positionAttribute.format = GAL_VERTEX_FORMAT_FLOAT2;

    GalVertexAttributeDescriptor colorAttribute;
    positionAttribute.shaderLocation = 1;
    positionAttribute.offset = offsetof(Vertex, color);
    positionAttribute.format = GAL_VERTEX_FORMAT_UCHAR4NORM;

    GalVertexBufferDescriptor vertexBufferDescriptor;
    vertexBufferDescriptor.stride = sizeof(Vertex);
    vertexBufferDescriptor.attributeCount = 2;
    vertexBufferDescriptor.attributes[0] = positionAttribute;
    vertexBufferDescriptor.attributes[1] = colorAttribute;

    GalVertexInputDescriptor vertexInputDescriptor = {
        vertexBuffers: [vertexBufferDescriptor]
    };

    /*** Finish Pipeline State ***/

//    GPUBlendDescriptors alphaBlendDescriptor = { srcFactor: "one", dstFactor: "zero", operation: "add" };
//    GPUBlendDescriptors colorBlendDescriptor = { srcFactor: "one", dstFactor: "zero", operation: "add" };

//    GPUColorStateDescriptor colorStateDescriptor = {
//        format: "bgra8unorm",
//        alphaBlend: alphaBlendDescriptor,
//        colorBlend: colorBlendDescriptor,
//        writeMask: GPUColorWriteBits.ALL
//    };

//    GPURenderPipelineDescriptor renderPipelineDescriptor = {
//        vertexStage: vertexStageDescriptor,
//        fragmentStage: fragmentStageDescriptor,
//        primitiveTopology: "triangle-list",
//        colorStates: [colorStateDescriptor],
//        vertexInput: vertexInputDescriptor
//    };

//    GPURenderPipeline renderPipeline = device.createRenderPipeline(renderPipelineDescriptor);

    /*** Swap Chain Setup ***/

    GalSwapChainDescriptor swapchainDesc;
    swapchainDesc.nativeWindowHandle = nativeWindowHandle;
    swapchainDesc.width = fbWidth;
    swapchainDesc.height = fbHeight;
    swapchainDesc.format = galDeviceGetSwapChainPreferredFormat(device);
    swapchainDesc.usage = GAL_TEXTURE_USAGE_OUTPUT_ATTACHMENT;
    swapchain = galDeviceConfigureSwapChain(device, &swapchainDesc);
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
