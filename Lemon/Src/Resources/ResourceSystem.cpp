#include "LemonPCH.h"
#include "ResourceSystem.h"
#include "Importer/ImageImporter.h"

using namespace std;
namespace Lemon
{
	ResourceSystem::ResourceSystem(Engine* engine)
		:ISystem(engine)
	{

	}

	bool ResourceSystem::Initialize()
	{
		string data_dir = FileUtils::GetSolutionDirectory() + "/Editor/Assets/";
		// Add engine standard resource directories
		AddDataDirectory(Asset_Cubemaps, data_dir + "Environment");
		AddDataDirectory(Asset_Fonts, data_dir + "fonts");
		AddDataDirectory(Asset_Icons, data_dir + "icons");
		AddDataDirectory(Asset_Scripts, data_dir + "scripts");
		AddDataDirectory(Asset_ShaderCompiler, data_dir + "shader_compiler");
		AddDataDirectory(Asset_Shaders, data_dir + "shaders");
		AddDataDirectory(Asset_Textures, data_dir + "textures");
		AddDataDirectory(Asset_Model, data_dir + "Models");

		m_ImageImporter = make_shared<ImageImporter>(m_Engine);

		return true;
	}

	void ResourceSystem::AddDataDirectory(const AssetType type, const string& directory)
	{
		m_ResourceDirectoriesMap[type] = directory;
	}

	std::string ResourceSystem::GetAssetDataDirectory(AssetType type)
	{
		for (auto& directory : m_ResourceDirectoriesMap)
		{
			if (directory.first == type)
				return directory.second;
		}

		return "";
	}
}