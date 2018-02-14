#include "SerializedScene.h"
#include <iostream>
#include <fstream>
#include "ObjectLibrary.h"
#include "YamlTools.h"
#include "GameComponentFactory.h"

SerializedScene::SerializedScene()
{
}

SerializedScene::~SerializedScene()
{
}

void SerializedScene::setUp(GameObject * root)
{
	// Initialize the components
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		SceneObject* sceneObject = m_sceneObjects[i];

		// If the object is already created, move onto the next object
		if (sceneObject->isObjectCreated())
		{
			continue;
		}

		if (sceneObject->getType() == "GameObject")
		{
			YAML::Node properties = sceneObject->getProperties();

			std::string name = properties["Name"].as<std::string>();
			GameObject* go = new GameObject(name);

			// Set the transform component
			Transform* trans = go->getTransform();
			YAML::Node transformNode = properties["Transform"];
			YAML::Node positionNode = transformNode["Position"];
			glm::vec3 Position = glm::vec3(positionNode[0].as<float>(), positionNode[1].as<float>(), positionNode[2].as<float>());
			trans->setPos(Position);
			YAML::Node rotationNode = transformNode["Rotation"];
			glm::vec3 rotationEuler = glm::vec3(rotationNode[0].as<float>(), rotationNode[1].as<float>(), rotationNode[2].as<float>());
			trans->setEulerAngles(rotationEuler);
			YAML::Node scaleNode = transformNode["Scale"];
			glm::vec3 scale = glm::vec3(scaleNode[0].as<float>(), scaleNode[1].as<float>(), scaleNode[2].as<float>());
			trans->setScale(scale);

			sceneObject->setObjectReference(go);
		}
	}

	// Resolve dependencies
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		SceneObject* sceneObject = m_sceneObjects[i];

		// Do not resolve the object if it is not created for some reason
		if (!sceneObject->isObjectCreated())
		{
			continue;
		}

		if (sceneObject->getType() == "GameObject")
		{
			YAML::Node properties = sceneObject->getProperties();
			GameObject* go = (GameObject*)sceneObject->getObjectReference();

			YAML::Node parentObjectIdNode = properties["Parent"];
			if (parentObjectIdNode)
			{
				uuid parentId = parentObjectIdNode.as<uuid>();
				SceneObject* parentSceneObject = findNode(parentId);
				GameObject* parentGo = (GameObject*)parentSceneObject->getObjectReference();
				parentGo->addChild(go);
			}
			else
			{
				root->addChild(go);
			}

			YAML::Node componentNodes = properties["Components"];
			for (int componentIndex = 0; componentIndex < componentNodes.size(); componentIndex++)
			{
				YAML::Node compNode = componentNodes[componentIndex];
				std::string compType = compNode["Type"].as<std::string>();

				GameComponent* component = GameComponentFactory::getInstance().CreateGameComponent(compType);

				if (component == nullptr)
				{
					continue;
				}

				if (component->deserialize(compNode))
				{
					go->addComponent(component);
				}

			}
		}
	}
}

void SerializedScene::tearDown(GameObject * root)
{
}

void SerializedScene::LoadFromFile(std::string filePath)
{
	printf("Loading scene from file: %s\n", filePath.c_str());
	YAML::Node file = YAML::LoadFile(filePath);
	YAML::Node scene = file["Scene"];
	for (unsigned i = 0; i < scene.size(); i++)
	{
		YAML::Node sceneNode = scene[i];
		YAML::Node typeNode = sceneNode["Type"];
		YAML::Node idNode = sceneNode["ID"];
		YAML::Node propertiesNode = sceneNode["Properties"];

		if (!typeNode)
		{
			printf("Skipping scene node %i because type is not defined.\n", i);
			continue;
		}

		if (!typeNode.IsScalar())
		{
			printf("Skipping scene node %i because type is not a scalar.\n", i);
			continue;
		}

		if (!typeNode.IsScalar())
		{
			printf("Skipping scene node %i because type is not a scalar.\n", i);
			continue;
		}

		if (!idNode || !idNode.IsScalar())
		{
			printf("Skipping scene node %i because it does not contain a valid ID.\n", i);
			continue;
		}

		if (!propertiesNode)
		{
			printf("Skipping scene node %i because it contains no properties.\n", i);
			continue;
		}

		boost::uuids::uuid id = idNode.as<boost::uuids::uuid>();
		std::string typeName = typeNode.as<std::string>();

		SceneObject* obj = new SceneObject(id, typeName, propertiesNode);

		// Store a tuple type object with object ID and generic pointer to object in memory then resolve all references
		m_sceneObjects.push_back(obj);
	}
}

void SerializedScene::SaveToFile(std::string filePath)
{
	YAML::Node root;
	
	YAML::Node materialNode;
	materialNode["Type"] = "Material";
	materialNode["ID"] = 100;
	YAML::Node materialProperties;
	materialProperties["Shader"] = "../res/diffuseFrag";
	YAML::Node materialTexture;
	materialTexture["Type"] = "Texture";
	materialTexture["Slot"] = 0;
	materialTexture["Path"] = "../res/grass.png";
	materialProperties["MaterialParameters"].push_back(materialTexture);
	materialNode["Properties"] = materialProperties;
	root["Scene"].push_back(materialNode);

	YAML::Node meshNode;
	meshNode["Type"] = "Mesh";
	meshNode["ID"] = 200;
	YAML::Node meshProperties;
	meshProperties["Path"] = "../res/sphere.obj";
	meshNode["Properties"] = meshProperties;
	root["Scene"].push_back(meshNode);

	YAML::Node gameObjectNode;
	gameObjectNode["Type"] = "GameObject";
	gameObjectNode["ID"] = 300;
	YAML::Node gameObjectProperties;
	gameObjectProperties["Name"] = "Diffuse Shaded Model";
	YAML::Node gameObjectTransform;
	gameObjectTransform["Position"].push_back<float>(0);
	gameObjectTransform["Position"].push_back<float>(0);
	gameObjectTransform["Position"].push_back<float>(-1);
	gameObjectTransform["Rotation"].push_back<float>(0);
	gameObjectTransform["Rotation"].push_back<float>(0);
	gameObjectTransform["Rotation"].push_back<float>(0);
	gameObjectTransform["Scale"].push_back<float>(1);
	gameObjectTransform["Scale"].push_back<float>(1);
	gameObjectTransform["Scale"].push_back<float>(1);
	gameObjectProperties["Transform"] = gameObjectTransform;
	YAML::Node meshRendererComponent;
	meshRendererComponent["Type"] = "MeshRenderer";
	meshRendererComponent["ID"] = 310;
	meshRendererComponent["Material"] = 100;
	meshRendererComponent["Mesh"] = 200;
	gameObjectProperties["Components"].push_back(meshRendererComponent);
	gameObjectNode["Properties"] = gameObjectProperties;
	root["Scene"].push_back(gameObjectNode);

	YAML::Emitter out;
	out.SetIndent(2);
	out.SetMapFormat(YAML::Block);
	out << root;
	//out.c_str()
	std::ofstream file;
	file.open(filePath);
	file << out.c_str();
	file.close();
}

SceneObject * SerializedScene::findNode(boost::uuids::uuid id)
{
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		if (m_sceneObjects[i]->getId() == id)
		{
			return m_sceneObjects[i];
		}
	}
	return nullptr;
}
