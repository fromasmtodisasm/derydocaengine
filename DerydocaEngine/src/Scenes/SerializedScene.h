#pragma once
#include "Scenes\Scene.h"
#include "Scenes\SceneObject.h"

namespace DerydocaEngine::Scenes
{

	class SerializedScene : public Scene
	{
	public:
		SerializedScene();
		~SerializedScene();

		virtual void setUp();
		virtual void tearDown();

		void LoadFromFile(const std::string& filePath);
		void SaveToFile(const std::string& filePath);
	private:
		std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;

		std::shared_ptr<SceneObject> findNode(const boost::uuids::uuid& id);
	};

}
