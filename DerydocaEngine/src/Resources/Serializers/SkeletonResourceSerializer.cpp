#include "EnginePch.h"
#include "Resources\Serializers\SkeletonResourceSerializer.h"

#include "assimp\Importer.hpp"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "Helpers\AssimpUtils.h"
#include "Animation\Skeleton.h"
#include "Resources\SkeletonResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> SkeletonResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		auto sr = std::static_pointer_cast<SkeletonResource>(resource);
		Assimp::Importer importer;
		auto scene = importer.ReadFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		return Helpers::AssimpUtils::getSkeleton(scene, 0);
	}

}
