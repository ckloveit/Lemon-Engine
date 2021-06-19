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

        //CustomData
        CustomDataFloat4UniformBuffer = RHIUniformBuffer<CustomDataFloat4UniformParameters>::CreateUniformBufferImmediate(4, "CustomDataFloat4");
        CustomDataFloat8UniformBuffer = RHIUniformBuffer<CustomDataFloat8UniformParameters>::CreateUniformBufferImmediate(5, "CustomDataFloat4");
        CustomDataFloat16UniformBuffer = RHIUniformBuffer<CustomDataFloat16UniformParameters>::CreateUniformBufferImmediate(6, "CustomDataFloat4");

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

        CustomDataFloat4UniformParameters customDataFloat4Parameters;
        customDataFloat4Parameters.CustomData0 = glm::vec4(0, 0, 0, 0);
        CustomDataFloat4UniformBuffer->UpdateUniformBufferImmediate(customDataFloat4Parameters);


    }
    

    
}