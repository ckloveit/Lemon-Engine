#include "LemonPCH.h"
#include "RHI/D3D11/D3D11Resources.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11Utils.h"

namespace Lemon
{
	struct D3D11VertexDeclarationKey
	{
		/** Vertex elements in the declaration. */
		D3D11VertexElements VertexElements;

		uint16_t StreamStrides[MaxVertexElementCount];
		/** Initialization constructor. */
		explicit D3D11VertexDeclarationKey(const VertexDeclarationElementList& InElements)
		{
			uint16_t UsedStreamsMask = 0;
			memset(StreamStrides, 0, sizeof(StreamStrides));

			for (int32_t ElementIndex = 0; ElementIndex < InElements.size(); ElementIndex++)
			{
				const RHIVertexElement& Element = InElements[ElementIndex];
				D3D11_INPUT_ELEMENT_DESC D3DElement = { 0 };
				D3DElement.InputSlot = Element.StreamIndex;
				D3DElement.AlignedByteOffset = Element.Offset;
				switch (Element.Type)
				{
				case VET_Float1:		D3DElement.Format = DXGI_FORMAT_R32_FLOAT; break;
				case VET_Float2:		D3DElement.Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case VET_Float3:		D3DElement.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				case VET_Float4:		D3DElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				case VET_PackedNormal:	D3DElement.Format = DXGI_FORMAT_R8G8B8A8_SNORM; break; //TODO: uint32 doesn't work because D3D11 squishes it to 0 in the IA-VS conversion
				case VET_UByte4:		D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UINT; break; //TODO: SINT, blendindices
				case VET_UByte4N:		D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
				case VET_Color:			D3DElement.Format = DXGI_FORMAT_B8G8R8A8_UNORM; break;
				case VET_Short2:		D3DElement.Format = DXGI_FORMAT_R16G16_SINT; break;
				case VET_Short4:		D3DElement.Format = DXGI_FORMAT_R16G16B16A16_SINT; break;
				case VET_Short2N:		D3DElement.Format = DXGI_FORMAT_R16G16_SNORM; break;
				case VET_Half2:			D3DElement.Format = DXGI_FORMAT_R16G16_FLOAT; break;
				case VET_Half4:			D3DElement.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
				case VET_Short4N:		D3DElement.Format = DXGI_FORMAT_R16G16B16A16_SNORM; break;
				case VET_UShort2:		D3DElement.Format = DXGI_FORMAT_R16G16_UINT; break;
				case VET_UShort4:		D3DElement.Format = DXGI_FORMAT_R16G16B16A16_UINT; break;
				case VET_UShort2N:		D3DElement.Format = DXGI_FORMAT_R16G16_UNORM; break;
				case VET_UShort4N:		D3DElement.Format = DXGI_FORMAT_R16G16B16A16_UNORM; break;
				case VET_URGB10A2N:		D3DElement.Format = DXGI_FORMAT_R10G10B10A2_UNORM; break;
				case VET_UInt:			D3DElement.Format = DXGI_FORMAT_R32_UINT; break;
				default: LEMON_CLIENT_ERROR("Unknown RHI vertex element type {0}", (uint8_t)InElements[ElementIndex].Type);
				};
				D3DElement.SemanticName = "ATTRIBUTE";
				D3DElement.SemanticIndex = Element.AttributeIndex;
				D3DElement.InputSlotClass = Element.bUseInstanceIndex ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;

				// This is a divisor to apply to the instance index used to read from this stream.
				D3DElement.InstanceDataStepRate = Element.bUseInstanceIndex ? 1 : 0;

				if ((UsedStreamsMask & 1 << Element.StreamIndex) != 0)
				{
					Check(StreamStrides[Element.StreamIndex] == Element.Stride);
				}
				else
				{
					UsedStreamsMask = UsedStreamsMask | (1 << Element.StreamIndex);
					StreamStrides[Element.StreamIndex] = Element.Stride;
				}

				VertexElements.emplace_back(D3DElement);
			}
		}
	};

	D3D11VertexDeclaration::D3D11VertexDeclaration(ID3D11Device* Direct3DDevice, Ref<RHIVertexShader> vertexShader, const D3D11VertexElements& InElements, const uint16_t* InStrides)
		: m_VertexElements(InElements)
	{
		memcpy(m_StreamStrides, InStrides, sizeof(m_StreamStrides));

		D3D11VertexShader* InVertexShader = D3D11ResourceCast(vertexShader.get());
		// Create an input layout for this combination of vertex declaration and vertex shader.
		D3D11_INPUT_ELEMENT_DESC NullInputElement;
		memset(&NullInputElement, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
		memcpy(m_StreamStrides, m_StreamStrides, MaxVertexElementCount);

		const auto result = Direct3DDevice->CreateInputLayout
		(
			m_VertexElements.data(),
			static_cast<UINT>(m_VertexElements.size()),
			InVertexShader->m_ShaderBlob->GetBufferPointer(),
			InVertexShader->m_ShaderBlob->GetBufferSize(),
			reinterpret_cast<ID3D11InputLayout * *>(&m_InputLayout)
		);
		if (FAILED(result))
		{
			LEMON_CORE_ERROR("Failed to create input layout, {0}", D3D11::DXGIErrorToString(result));
			Check("Failed to create input layout");
		}
	}

	Ref<RHIVertexDeclaration> D3D11DynamicRHI::RHICreateVertexDeclaration(Ref<RHIVertexShader> vertexShader, const VertexDeclarationElementList& Elements)
	{
		// Construct a key from the elements.
		D3D11VertexDeclarationKey Key(Elements);

		// Check for a cached vertex declaration.
		//FVertexDeclarationRHIRef* VertexDeclarationRefPtr = GVertexDeclarationCache.Find(Key);

		// The cached declaration must match the input declaration!
		return CreateRef<D3D11VertexDeclaration>(GetDevice(), vertexShader, Key.VertexElements, Key.StreamStrides);
	}
}