#include "LightManager.h"
#include "CameraManager.h"
#include "glm\glm.hpp"

void LightManager::bindLightsToShader(Transform* objectTransform, Shader* shader)
{
	assert(objectTransform);
	assert(shader);

	// Get a list of lights that will affect the object being sent in
	std::list<Light*> lights = getLights(objectTransform);

	// Cache some things
	Camera* currentCamera = CameraManager::getInstance().getCurrentCamera();
	glm::mat4 viewMat = currentCamera->getViewMatrix();

	// Loop through each light and bind them to the shader
	int lightIndex = 0;
	for each (Light* light in lights)
	{
		Light::LightType lightType = light->getLightType();

		if (lightType == Light::Directional)
		{
			// Set the light direction
			std::string typeName = "lights[" + std::to_string(lightIndex) + "].Direction";
			glm::vec3 lightDirection = currentCamera->getViewMatrix() * light->getGameObject()->getTransform()->getWorldModel() * glm::vec4(1, 0, 0, 0);
			shader->setVec3(typeName, lightDirection);
		}

		// Set the light type
		std::string typeName = "lights[" + std::to_string(lightIndex) + "].Type";
		shader->setInt(typeName, (int)lightType);

		// Set the intensity
		std::string intensityName = "lights[" + std::to_string(lightIndex) + "].Intensity";
		shader->setColorRGB(intensityName, light->getColor());

		// Set the position
		glm::vec3 lightWorldPos = light->getGameObject()->getTransform()->getWorldPos();
		glm::vec4 lightPositionEyeCoords = viewMat * glm::vec4(lightWorldPos, 1);
		std::string positionName = "lights[" + std::to_string(lightIndex) + "].Position";
		shader->setVec3(positionName, lightPositionEyeCoords);

		// Increase our light index
		lightIndex++;
	}
}

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

std::list<Light*> LightManager::getLights(Transform * objectTransform)
{
	// Create a list to store the lights
	std::list<Light*> lights = std::list<Light*>();

	// Go through each light
	int numLights = 0;
	for each (Light* light in m_lights)
	{
		// TODO: Only include lights that would potentially effect this object

		// Add the light to the list
		lights.push_back(light);

		// If we are at the maximum number of supported lights, lets end it early
		numLights++;
		if (numLights >= MAX_LIGHTS) {
			break;
		}
	}

	// Return the list of lights
	return lights;
}