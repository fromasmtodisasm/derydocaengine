#include "ResourceSerializerLibrary.h"
#include "MaterialResourceSerializer.h"
#include "MeshResourceSerializer.h"
#include "TextureResourceSerializer.h"
#include "CubemapResourceSerializer.h"
#include "ShaderResourceSerializer.h"

ResourceSerializer * ResourceSerializerLibrary::getSerializer(ResourceType type)
{
	// Return the associated serializer for the resource type specified
	auto search = m_serializers.find(type);
	if (search != m_serializers.end())
	{
		return search->second;
	}
	else
	{
		return nullptr;
	}
}

ResourceSerializerLibrary::ResourceSerializerLibrary()
{
	registerSerializer(new MaterialResourceSerializer());
	registerSerializer(new MeshResourceSerializer());
	registerSerializer(new TextureResourceSerializer());
	registerSerializer(new CubemapResourceSerializer());
	registerSerializer(new ShaderResourceSerializer());
}

ResourceSerializerLibrary::~ResourceSerializerLibrary()
{
	// Delete all type serializers
	for (std::map<ResourceType, ResourceSerializer*>::iterator it = m_serializers.begin(); it != m_serializers.end(); ++it)
	{
		delete(it->second);
	}
}

void ResourceSerializerLibrary::registerSerializer(ResourceSerializer * serializer)
{
	// Add this serializer to the list of serializers
	m_serializers.insert(std::pair<ResourceType, ResourceSerializer*>(serializer->getResourceType(), serializer));
}
