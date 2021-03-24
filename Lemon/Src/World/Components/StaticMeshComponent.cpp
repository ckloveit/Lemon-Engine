#include "LemonPCH.h"
#include "StaticMeshComponent.h"

namespace Lemon
{
    void StaticMeshComponent::SetMesh(Ref<Mesh> renderMesh)
    {
        m_RenderMesh = renderMesh;
    }

}