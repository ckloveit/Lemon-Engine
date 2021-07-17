#include "Core/Core.h"
#include "RHI/RHI.h"
#include <glm/glm.hpp>
#include "RHI/RHIResources.h"
#include "RenderCore/RenderUtils.h"
#include "Core/TSingleon.h"

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
        glm::vec4 CameraWorldPosition;
        glm::vec4 AmbientColor;
        glm::vec3 TestColor;
    };
    
    // Low frequency buffer - Updates once per frame
    struct LightUniformParameters
    {
        glm::vec4 DirectionalLightDir; // if w = 0 which mean no directionallight
        glm::vec4 DirectionalLightColor; //

        
    };
    
    // High frequency - Updates at least as many times as there are objects in the scene
    struct ObjectUniformParameters
    {
        glm::mat4 LocalToWorldMatrix;
        glm::mat4 WorldToWorldMatrix;
        glm::mat4 WorldToWorldTransposeMatrix;
        glm::vec4 Albedo;
        glm::vec4 PBRParameters;//x : Metallic; y : Roughness; z : AO 
        
        glm::vec4 Color;
        
        bool operator==(const ObjectUniformParameters& rhs)
        {
            return
                LocalToWorldMatrix == rhs.LocalToWorldMatrix &&
                Color == rhs.Color &&
                Albedo == rhs.Albedo &&
                PBRParameters == rhs.PBRParameters;
        }
    };
	// High frequency - Updates at least as many times as there are many fullscreenquad drawcall in one frame
	struct FullScreenUniformParameters
	{
		glm::mat4 LocalToWorldMatrix;

	};

    // High frequency - Updates at least as many times as there are many fullscreenquad drawcall in one frame
    struct CustomDataFloat4UniformParameters
    {
        glm::vec4 CustomData0;
    };
    struct CustomDataFloat8UniformParameters
    {
        glm::vec4 CustomData0;
        glm::vec4 CustomData1;
    };
    struct CustomDataFloat16UniformParameters
    {
        glm::vec4 CustomData0;
        glm::vec4 CustomData1;
        glm::vec4 CustomData2;
        glm::vec4 CustomData3;
    };

    
    struct LEMON_API SceneUniformBuffers : public TSingleton<SceneUniformBuffers>
    {
     public:
        SceneUniformBuffers();
        ~SceneUniformBuffers() = default;

        void Allocate();

        // UniformBuffers
        Ref<RHIUniformBuffer<ViewUniformParameters>> ViewUniformBuffer;
        Ref<RHIUniformBuffer<ObjectUniformParameters>> ObjectUniformBuffer;
        Ref<RHIUniformBuffer<LightUniformParameters>> LightUniformBuffer;
		Ref<RHIUniformBuffer<FullScreenUniformParameters>> FullScreenUniformBuffer;
        // CustomUniformBuffers
        Ref<RHIUniformBuffer<CustomDataFloat4UniformParameters>> CustomDataFloat4UniformBuffer;
        Ref<RHIUniformBuffer<CustomDataFloat8UniformParameters>> CustomDataFloat8UniformBuffer;
        Ref<RHIUniformBuffer<CustomDataFloat16UniformParameters>> CustomDataFloat16UniformBuffer;
        
    };



    
}    