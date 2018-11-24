#include "Files\Serializers\RasterFontFileSerializer.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node RasterFontFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	Files::FileType RasterFontFileSerializer::getFileType()
	{
		return Files::FileType::RasterFontType;
	}

	Resources::Resource * RasterFontFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::RasterFontResourceType);
		return r;
	}

}