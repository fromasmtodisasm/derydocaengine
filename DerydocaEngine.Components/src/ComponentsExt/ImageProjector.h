#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Texture.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class ImageProjector : public Components::GameComponent, Components::SelfRegister<ImageProjector>
	{
	public:
		GENINSTANCE(ImageProjector);

		ImageProjector();
		~ImageProjector();

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void update(float const& deltaTime);

		void inline setFocalPoint(glm::vec3 const& focalPoint) { m_focalPoint = focalPoint; m_dirty = true; }
		void inline setUpVector(glm::vec3 const& upVector) { m_upVector = upVector; m_dirty = true; }
	private:
		bool m_dirty = true;
		glm::vec3 m_focalPoint;
		glm::vec3 m_upVector;
		glm::mat4 m_projectorMatrix;
		float m_fov;
		float m_zNear;
		float m_zFar;
		Rendering::Texture* m_projectorTexture;
		std::vector<Components::MeshRenderer*> m_meshRenderers;

		void updateProjectionMatrix();
		void setProjectionGraphic();
	};

}