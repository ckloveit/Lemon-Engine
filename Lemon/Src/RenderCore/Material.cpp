#include "LemonPCH.h"
#include "Material.h"

namespace Lemon
{
    Material::Material()
        :Albedo(glm::vec3(1.0f, 0.0f, 0.0f))
        ,Metallic(0.0f)
        ,Roughness(0.1f)
        ,AO(1.0f)
    {

    }


}