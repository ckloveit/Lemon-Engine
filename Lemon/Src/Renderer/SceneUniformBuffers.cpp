#include "LemonPCH.h"
#include "SceneUniformBuffers.h"

namespace Lemon
{
    
    SceneUniformBuffers::SceneUniformBuffers()
    {
        ViewUniformBuffer = RHIUniformBuffer<ViewUniformParameters>::CreateUniformBufferImmediate("ViewUniform");

        ViewUniformParameters parameters;
        parameters.WorldToProjection = glm::mat4();
        parameters.TestColor = glm::vec3(1,0,0);
        ViewUniformBuffer->UpdateUniformBufferImmediate(parameters);

    }
    

    
}