#include "example_shaders.h"
#include <assert.h> // assert
#include <gal/gal.h>
#include <stddef.h> // offsetof
#include <string.h> // memcpy

const char *exampleWindowTitle = __FILE__;

static mtx_t mutex;
static cnd_t cond;
static GalAdapter adapter = {0};
static GalDevice device = {0};
static GalRenderPipeline renderPipeline = {0};
static GalSwapChain swapChain = {0};

typedef struct Vertex
{
    float x, y;
    GalColor color;
} Vertex;

static GalBuffer vertexBuffer;
static Vertex vertexData[] = {{0.0f, 0.8f, GalColorCyan},
                              {-0.8f, -0.8f, GalColorYellow},
                              {0.8f, -0.8f, GalColorMagenta}};

void write_vertex(GalBufferMapAsyncStatus status, uint8_t *dest, void *userdata)
{
    mtx_lock(&mutex);

    assert(status == GalBufferMapAsyncStatusSuccess);
    memcpy(dest, vertexData, sizeof(vertexData));

    cnd_broadcast(&cond);
    mtx_unlock(&mutex);
}

bool exampleInit(void *nativeWindowHandle, int fbWidth, int fbHeight)
{
    mtx_init(&mutex, mtx_plain);
    cnd_init(&cond);

    /*** Device Setup ***/

    adapter = galRequestAdapter(
        &(GalRequestAdapterOptions){.powerPreference = GalPowerPreferenceHighPerformance});

    device = galAdapterRequestDevice(adapter,
                                     &(GalDeviceDescriptor){
                                         .extensions = {.anisotropicFiltering = false},
                                     });

    /*** Shader Setup ***/

    GalShaderModuleDescriptor vertexDescriptor = {.code = shaders_color_vert_460_glsl,
                                                  .length = sizeof(shaders_color_vert_460_glsl)};

    GalShaderModuleDescriptor fragmentDescriptor = {.code = shaders_color_frag_460_glsl,
                                                    .length = sizeof(shaders_color_frag_460_glsl)};

    GalShaderModule vertexShader = galDeviceCreateShaderModule(device, &vertexDescriptor);
    GalShaderModule fragmentShader = galDeviceCreateShaderModule(device, &fragmentDescriptor);

    const GalProgrammableStageDescriptor vertexStageDescriptor = {.module = vertexShader,
                                                                  .entryPoint = "main"};
    const GalProgrammableStageDescriptor fragmentStageDescriptor = {.module = fragmentShader,
                                                                    .entryPoint = "main"};

    /*** Vertex Buffer Setup ***/

    GalBufferDescriptor vertexDataBufferDescriptor = {.size = sizeof(vertexData),
                                                      .usage = GalBufferUsageMapWrite
                                                               | GalBufferUsageVertex};

    vertexBuffer = galDeviceCreateBuffer(device, &vertexDataBufferDescriptor);

    /*** Write Data To GPU ***/

    mtx_lock(&mutex);
    galBufferMapWriteAsync(vertexBuffer, write_vertex, NULL);
    cnd_wait(&cond, &mutex);
    mtx_unlock(&mutex);
    galBufferUnmap(vertexBuffer);

    /*** Describe Vertex Data For Pipeline ***/

    GalVertexAttributeDescriptor attributes[2] = {// position attribute
                                                  {.shaderLocation = 0,
                                                   .offset = 0,
                                                   .format = GalVertexFormatFloat2},
                                                  // color attribute
                                                  {.shaderLocation = 1,
                                                   .offset = offsetof(Vertex, color),
                                                   .format = GalVertexFormatUchar4Norm}};

    GalVertexBufferDescriptor vertexBufferDescriptor = {.stride = sizeof(Vertex),
                                                        .attributeSet = attributes,
                                                        .attributeSetLength = sizeof(attributes)};

    GalVertexInputDescriptor vertexInputDescriptor = {.vertexBuffers = &vertexBufferDescriptor,
                                                      .vertexBuffersLength = 1};

    /*** Finish Pipeline State ***/

    GalBlendDescriptor alphaBlendDescriptor = {.srcFactor = GalBlendFactorOne,
                                               .dstFactor = GalBlendFactorZero,
                                               .operation = GalBlendOperationAdd};

    GalBlendDescriptor colorBlendDescriptor = {.srcFactor = GalBlendFactorOne,
                                               .dstFactor = GalBlendFactorZero,
                                               .operation = GalBlendOperationAdd};

    GalColorStateDescriptor colorStateDescriptor = {.format = GalTextureFormatRgba8Unorm,
                                                    .alphaBlend = alphaBlendDescriptor,
                                                    .colorBlend = colorBlendDescriptor,
                                                    .writeMask = GalColorWriteAll};

    GalRenderPipelineDescriptor renderPipelineDescriptor = {
        .vertexStage = vertexStageDescriptor,
        .fragmentStage = &fragmentStageDescriptor,
        .primitiveTopology = GalPrimitiveTopologyTriangleList,
        .colorStates = &colorStateDescriptor,
        .colorStatesLength = 1,
        .vertexInput = vertexInputDescriptor,
        .sampleCount = 1,
        .sampleMask = 0xFFFFFFFF,
    };

    renderPipeline = galDeviceCreateRenderPipeline(device, &renderPipelineDescriptor);

    /*** Swap Chain Setup ***/

    GalSwapChainDescriptor swapchainDesc = {.nativeWindowHandle = nativeWindowHandle,
                                            .width = fbWidth,
                                            .height = fbHeight,
                                            .format = galDeviceGetSwapChainPreferredFormat(device),
                                            .usage = GalTextureUsageOutputAttachment};

    swapChain = galDeviceConfigureSwapChain(device, &swapchainDesc);

    return true;
}

void exampleFrame()
{
    /*** Render Pass Setup ***/

    /* Acquire Texture To Render To */

    GalTexture swapChainTexture = galSwapChainGetCurrentTexture(swapChain);
    GalTextureView renderAttachment = galTextureCreateDefaultView(swapChainTexture);

    GalRenderPassColorAttachmentDescriptor colorAttachmentDescriptor
        = {.attachment = renderAttachment,
           .loadOp = GalLoadOpClear,
           .storeOp = GalStoreOpStore,
           .clearColor = GalColorMidnightBlue};

    GalRenderPassDescriptor renderPassDescriptor = {.colorAttachments = &colorAttachmentDescriptor,
                                                    .colorAttachmentsSize = 1};

    /*** Rendering ***/

    GalCommandEncoder commandEncoder = galDeviceCreateCommandEncoder(device, NULL);
    GalRenderPassEncoder renderPassEncoder = galCommandEncoderBeginRenderPass(commandEncoder,
                                                                              &renderPassDescriptor);

    galRenderPassEncoderSetPipeline(renderPassEncoder, renderPipeline);

    uint32_t vertexBufferSlot = 0;
    GalBufferSize vertexBufferOffset = 0;
    galRenderPassEncoderSetVertexBuffers(renderPassEncoder,
                                         vertexBufferSlot,
                                         &vertexBuffer,
                                         &vertexBufferOffset,
                                         1);
    galRenderPassEncoderDraw(renderPassEncoder,
                             3,  // 3 vertices
                             1,  // 1 instance
                             0,  // 0th vertex
                             0); // 0th instance.
    galRenderPassEncoderEndPass(renderPassEncoder);

    GalCommandBuffer commandBuffer = galCommandEncoderFinish(commandEncoder, NULL);

    GalQueue queue = galDeviceGetQueue(device);
    galQueueSubmit(queue, &commandBuffer, 1);
}

void exampleShutdown()
{
}
