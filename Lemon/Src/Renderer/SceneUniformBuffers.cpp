#include "LemonPCH.h"
#include "SceneUniformBuffers.h"

namespace Lemon
{
    
    SceneUniformBuffers::SceneUniformBuffers()
    {
        ViewUniformBuffer = RHIUniformBuffer<ViewUniformParameters>::CreateUniformBufferImmediate("ViewUniform");
        ObjectUniformBuffer = RHIUniformBuffer<ObjectUniformParameters>::CreateUniformBufferImmediate("ObjectUniform");
        
        // Upload
        ViewUniformParameters viewParameters;
        viewParameters.ViewMatrix = glm::mat4();
        viewParameters.ProjectionMatrix = glm::mat4();
        viewParameters.ViewProjectionMatrix = glm::mat4();
        viewParameters.TestColor = glm::vec3(1,0,0);
        ViewUniformBuffer->UpdateUniformBufferImmediate(viewParameters);

        //
        ObjectUniformParameters objectParameters;
        objectParameters.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        objectParameters.LocalToWorldMatrix = glm::mat4();
        ObjectUniformBuffer->UpdateUniformBufferImmediate(objectParameters);
        
    }
    

    
}