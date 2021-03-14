#include "Core/Core.h"
#include "RHI/RHI.h"
#include <glm/glm.hpp>
#include "RHI/RHIResources.h"
#include "RenderCore/RenderUtils.h"

namespace Lemon
{
    class RHICommandList;
    class SceneRenderer;

    struct ViewUniformParameters
    {
        glm::mat4 WorldToProjection;
        glm::vec3 TestColor;
    };
    
    struct LEMON_API SceneUniformBuffers
    {
     public:
        SceneUniformBuffers();
        ~SceneUniformBuffers() = default;

        // UniformBuffers
        Ref<RHIUniformBuffer<ViewUniformParameters>> ViewUniformBuffer;
    };



    
}    