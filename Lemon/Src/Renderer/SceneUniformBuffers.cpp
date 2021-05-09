#include "LemonPCH.h"
#include "SceneUniformBuffers.h"

namespace Lemon
{
    
    SceneUniformBuffers::SceneUniformBuffers()
    {
        ViewUniformBuffer = RHIUniformBuffer<ViewUniformParameters>::CreateUniformBufferImmediate(0, "ViewUniform");
        ObjectUniformBuffer = RHIUniformBuffer<ObjectUniformParameters>::CreateUniformBufferImmediate(1, "ObjectUniform");
        LightUniformBuffer = RHIUniformBuffer<LightUniformParameters>::CreateUniformBufferImmediate(2, "LightUniform");
		FullScreenUniformBuffer = RHIUniformBuffer<FullScreenUniformParameters>::CreateUniformBufferImmediate(3, "FullScreenUniform");

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

        LightUniformParameters lightParameters;
        lightParameters.DirectionalLightDir = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        lightParameters.DirectionalLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        LightUniformBuffer->UpdateUniformBufferImmediate(lightParameters);
        
		FullScreenUniformParameters fullScreenParameters;
		fullScreenParameters.LocalToWorldMatrix = glm::mat4();
		FullScreenUniformBuffer->UpdateUniformBufferImmediate(fullScreenParameters);


    }
    

    
}