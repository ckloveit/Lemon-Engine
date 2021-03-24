#include "Core/Core.h"
#include "RHI/RHI.h"
#include <glm/glm.hpp>
#include "RHI/RHIResources.h"
#include "RenderCore/RenderUtils.h"

namespace Lemon
{
    class RHICommandList;
    class SceneRenderer;
    
    // Low frequency buffer - Updates once per frame
    struct ViewUniformParameters
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;

        glm::vec3 TestColor;
    };

    
    // High frequency - Updates at least as many times as there are objects in the scene
    struct ObjectUniformParameters
    {
        glm::mat4 LocalToWorldMatrix;
        glm::vec4 Color;
        
        bool operator==(const ObjectUniformParameters& rhs)
        {
            return
                LocalToWorldMatrix == rhs.LocalToWorldMatrix &&
                Color == rhs.Color;
        }
    };
    
    struct LEMON_API SceneUniformBuffers
    {
     public:
        SceneUniformBuffers();
        ~SceneUniformBuffers() = default;

        // UniformBuffers
        Ref<RHIUniformBuffer<ViewUniformParameters>> ViewUniformBuffer;
        Ref<RHIUniformBuffer<ObjectUniformParameters>> ObjectUniformBuffer;
        
    };



    
}    