#include "LemonPCH.h"
#include "RHI/D3D11/D3D11Resources.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11Utils.h"
#include "Containers/ResourceArray.h"
#include <d3dcommon.h>

namespace Lemon
{
	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		D3D11::SafeRelease(m_Buffer);
	}

	Ref<RHIVertexBuffer> D3D11DynamicRHI::RHICreateVertexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo)
	{
		const void* resourceData = createInfo.ResourceArray->GetResourceData();

		// fill in a buffer description.
		D3D11_BUFFER_DESC bufDesc;
		bufDesc.ByteWidth = size;
		bufDesc.Usage = IsStaticUsage(usage) && resourceData ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = IsDynamicUsage(usage) ? D3D11_CPU_ACCESS_WRITE : 0;
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.StructureByteStride = 0;
		bufDesc.MiscFlags = 0;

		// fill in the subresource data.
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = resourceData;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		
		ID3D11Buffer* buffer = nullptr;
		const auto result = GetDevice()->CreateBuffer(&bufDesc, IsDynamicUsage(usage) ? nullptr : &initData, &buffer);
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to create vertex buffer");
			return nullptr;
		}
		// set debug name
		if (createInfo.DebugName.size())
		{
			buffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)createInfo.DebugName.size(), createInfo.DebugName.c_str());
		}
		return CreateRef<D3D11VertexBuffer>(this, buffer, size, usage);
	}

	void* D3D11VertexBuffer::Lock() const
	{
		if (!m_D3DRHI || !m_Buffer)
		{
			LEMON_CORE_ERROR("Invalid Vertex Buffer.");
			return nullptr;
		}

		// Disable GPU access to the vertex buffer data.
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		const auto result = m_D3DRHI->GetDeviceContext()->Map(static_cast<ID3D11Resource*>(m_Buffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to map vertex buffer");
			return nullptr;
		}
		return mappedResource.pData;
	}

	// close the Update Buffer address
	bool D3D11VertexBuffer::UnLock() const
	{
		if (!m_D3DRHI || !m_Buffer)
		{
			LEMON_CORE_ERROR("Failed to unmap vertex buffer");
			return false;
		}

		// Re-enable GPU access to the vertex buffer data.
		m_D3DRHI->GetDeviceContext()->Unmap(static_cast<ID3D11Resource*>(m_Buffer), 0);
		return true;
	}
}