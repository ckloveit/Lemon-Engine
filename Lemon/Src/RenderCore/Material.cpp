#include "LemonPCH.h"
#include "Material.h"

namespace Lemon
{
    Material::Material()
        :Albedo(glm::vec3(1.0f, 1.0f, 1.0f))
        ,Metallic(1.0f)
        ,Roughness(0.0f)
        ,AO(1.0f)
    {

    }


}