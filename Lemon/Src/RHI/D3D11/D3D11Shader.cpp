#include "LemonPCH.h"
#include "RHI/D3D11/D3D11Resources.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11DynamicRHI.h"

using namespace std;

namespace Lemon
{
	D3D11VertexShader::D3D11VertexShader(ID3D11VertexShader* Resource, ID3D10Blob* ShaderBlob)
		: m_Resource(Resource)
		, m_ShaderBlob(ShaderBlob)
	{

	}

	D3D11VertexShader::~D3D11VertexShader()
	{
		D3D11::SafeRelease(m_Resource);
		D3D11::SafeRelease(m_ShaderBlob);
	}

	Ref<RHIVertexShader> D3D11DynamicRHI::RHICreateVertexShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo)
	{
		// Compile flags
		uint32_t compileFlags = 0;
#ifdef LEMON_DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		// Defines
		vector<D3D_SHADER_MACRO> defines =
		{
			D3D_SHADER_MACRO{ "COMPILE_VS", "1" },
			D3D_SHADER_MACRO{ "COMPILE_PS", "0" },
			D3D_SHADER_MACRO{ "COMPILE_CS", "0" }
		};
		for (const auto& define : createInfo.ShaderDefines)
		{
			defines.emplace_back(D3D_SHADER_MACRO{ define.first.c_str(), define.second.c_str() });
		}
		defines.emplace_back(D3D_SHADER_MACRO{ nullptr, nullptr });
		// Compile
		ID3DBlob* blobError = nullptr;
		ID3DBlob* shaderBlob = nullptr;
		auto filePathW = FileUtils::StringToWstring(filePath);
		HRESULT result = D3DCompileFromFile
		(
			filePathW.c_str(),
			defines.data(),
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.c_str(),
			"vs_5_0",
			compileFlags,
			0,
			&shaderBlob,
			&blobError
		);

		if(blobError)
		{
			stringstream ss(static_cast<char*>(blobError->GetBufferPointer()));
			string line;
			while (getline(ss, line, '\n'))
			{
				const auto is_error = line.find("error") != string::npos;
				if (is_error)
				{
					LEMON_CORE_ERROR(line);
				}
				else
				{
					LEMON_CORE_ERROR(line);
				}
			}
			D3D11::SafeRelease(blobError);
			return nullptr;
		}
		// Create shader
		ID3D11VertexShader* vertexShader = nullptr;
		if (FAILED(GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), nullptr, &vertexShader)))
		{
			LEMON_CORE_ERROR("Failed to create vertex shader, {0}", D3D11::DXGIErrorToString(result));
		}

		return CreateRef<D3D11VertexShader>(vertexShader, shaderBlob);
	}

	D3D11PixelShader::D3D11PixelShader(ID3D11PixelShader* Resource)
		:m_Resource(Resource)
	{

	}
	D3D11PixelShader::~D3D11PixelShader()
	{
		D3D11::SafeRelease(m_Resource);
	}

	Ref<RHIPixelShader> D3D11DynamicRHI::RHICreatePixelShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo)
	{
		// Compile flags
		uint32_t compileFlags = 0;
#ifdef LEMON_DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		// Defines
		vector<D3D_SHADER_MACRO> defines =
		{
			D3D_SHADER_MACRO{ "COMPILE_VS", "0" },
			D3D_SHADER_MACRO{ "COMPILE_PS", "1" },
			D3D_SHADER_MACRO{ "COMPILE_CS", "0" }
		};
		for (const auto& define : createInfo.ShaderDefines)
		{
			defines.emplace_back(D3D_SHADER_MACRO{ define.first.c_str(), define.second.c_str() });
		}
		defines.emplace_back(D3D_SHADER_MACRO{ nullptr, nullptr });
		// Compile
		ID3DBlob* blobError = nullptr;
		ID3DBlob* shaderBlob = nullptr;
		auto filePathW = FileUtils::StringToWstring(filePath);
		HRESULT result = D3DCompileFromFile
		(
			filePathW.c_str(),
			defines.data(),
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.c_str(),
			"ps_5_0",
			compileFlags,
			0,
			&shaderBlob,
			&blobError
		);

		if (blobError)
		{
			stringstream ss(static_cast<char*>(blobError->GetBufferPointer()));
			string line;
			while (getline(ss, line, '\n'))
			{
				const auto is_error = line.find("error") != string::npos;
				if (is_error)
				{
					LEMON_CORE_ERROR(line);
				}
				else
				{
					LEMON_CORE_ERROR(line);
				}
			}
			D3D11::SafeRelease(blobError);
			return nullptr;
		}
		// Create shader
		ID3D11PixelShader* pixelShader = nullptr;
		if (FAILED(GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), nullptr, &pixelShader)))
		{
			LEMON_CORE_ERROR("Failed to create vertex shader, {0}", D3D11::DXGIErrorToString(result));
		}
		return CreateRef<D3D11PixelShader>(pixelShader);
	}

	D3D11VertexDeclaration::~D3D11VertexDeclaration()
	{
		D3D11::SafeRelease(m_InputLayout);
	}

}