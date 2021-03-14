#include "LemonPCH.h"
#include "RHI/D3D11/D3D11Resources.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11Utils.h"
#include "Containers/ResourceArray.h"
#include <d3dcommon.h>

namespace Lemon
{
	D3D11UniformBuffer ::~D3D11UniformBuffer()
	{
		D3D11::SafeRelease(m_Buffer);
	}

	Ref<RHIUniformBufferBase> D3D11DynamicRHI::RHICreateUniformBuffer(uint32_t size, const std::string& uniformBufferName)
	{
		// fill in a buffer description.
		D3D11_BUFFER_DESC bufDesc;
		bufDesc.ByteWidth = size;
		bufDesc.Usage =  D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.StructureByteStride = 0;
		bufDesc.MiscFlags = 0;

		ID3D11Buffer* buffer = nullptr;
		const auto result = GetDevice()->CreateBuffer(&bufDesc, nullptr, &buffer);
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to create Uniform buffer");
			return nullptr;
		}
		// set debug name
		if (uniformBufferName.size())
		{
			buffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)uniformBufferName.size(), uniformBufferName.c_str());
		}
		return CreateRef<D3D11UniformBuffer>(this, buffer, size);
	}

	void* D3D11UniformBuffer::Lock() const
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
			LEMON_CORE_ERROR("Failed to map uniform buffer");
			return nullptr;
		}
		return mappedResource.pData;
	}

	// close the Update Buffer address
	bool D3D11UniformBuffer::UnLock() const
	{
		if (!m_D3DRHI || !m_Buffer)
		{
			LEMON_CORE_ERROR("Failed to unmap uniform buffer");
			return false;
		}

		// Re-enable GPU access to the vertex buffer data.
		m_D3DRHI->GetDeviceContext()->Unmap(static_cast<ID3D11Resource*>(m_Buffer), 0);
		return true;
	}
}