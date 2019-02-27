#include "EditorComponentsPch.h"
#include "AnimationViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Dgui\ResourcePicker.h"
#include "Rendering\Mesh.h"
#include <limits>
#include "Components\Light.h"

DerydocaEngine::Components::AnimationViewerWindow::AnimationViewerWindow() :
	SceneViewerWindow(),
	m_animationTime(0.0f),
	m_playbackSpeed(1.0f),
	m_modelScale(0.001f),
	m_playing(false),
	m_looping(false),
	m_meshRenderer(std::make_shared<Components::SkinnedMeshRenderer>()),
	m_scene(std::make_shared<Scenes::HardCodedScene>())
{
	auto meshRendererGameObject = std::make_shared<GameObject>("__skinnedMeshRenderer");
	meshRendererGameObject->addComponent(m_meshRenderer);
	meshRendererGameObject->init();
	meshRendererGameObject->postInit();
	meshRendererGameObject->getTransform()->setScale(glm::vec3(0.001f));

	m_scene->getRoot()->addChild(meshRendererGameObject);

	auto go = std::make_shared<GameObject>("");
	go->addComponent(Components::Light::generateInstance());
	go->init();
	go->postInit();
	go->getTransform()->setPos({ 0.0f, 0.5f, 1.0f });
	m_scene->getRoot()->addChild(go);
	auto go2 = std::make_shared<GameObject>("");
	go2->init();
	go2->postInit();
	go2->addComponent(Components::Light::generateInstance());
	go2->getTransform()->setPos({ 0.0f, 0.5f, -1.0f });
	m_scene->getRoot()->addChild(go2);

	getCamera();
}

DerydocaEngine::Components::AnimationViewerWindow::~AnimationViewerWindow()
{
}

void DerydocaEngine::Components::AnimationViewerWindow::update(const float deltaTime)
{
	SceneViewerWindow::update(deltaTime);

	if (m_playing && m_meshRenderer->isFullyConfigured())
	{
		m_animationTime += deltaTime * m_playbackSpeed;

		float animDuration = static_cast<float>(m_meshRenderer->getAnimation()->getDuration());
		if (m_animationTime > animDuration)
		{
			if (m_looping)
			{
				m_animationTime = fmod(m_animationTime, animDuration) * animDuration;
			}
			else
			{
				// TODO: Fix this. It is currently invalid to end on the final frame as
				//  the engine will crash looking for the next frame to blend
				m_animationTime = animDuration - (animDuration / 1000.0f);
				m_playing = false;
			}
		}
		if (m_animationTime < 0.0f)
		{
			if (m_looping)
			{
				m_animationTime = animDuration - (fmod(m_animationTime, animDuration) * animDuration * -1);
			}
			else
			{
				m_animationTime = 0.0f;
				m_playing = false;
			}
		}
	}

	m_meshRenderer->setAnimationTime(m_animationTime);
}

void DerydocaEngine::Components::AnimationViewerWindow::renderWindow()
{
	SceneViewerWindow::updateDisplayProperties();

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Animation", m_meshRenderer->getAnimationResource(), Resources::AnimationResourceType, modifiedResource))
		{
			m_meshRenderer->setAnimationResource(std::static_pointer_cast<Resources::AnimationResource>(modifiedResource));
		}
	}

	ImGui::SameLine();

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Mesh", m_meshRenderer->getMeshResource(), Resources::MeshResourceType, modifiedResource))
		{
			m_meshRenderer->setMeshResource(std::static_pointer_cast<Resources::MeshResource>(modifiedResource));
		}
	}

	ImGui::SameLine();

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Material", m_meshRenderer->getMaterialResource(), Resources::MaterialResourceType, modifiedResource))
		{
			m_meshRenderer->setMaterialResource(std::static_pointer_cast<Resources::MaterialResource>(modifiedResource));
		}
	}

	ImGui::SameLine();
	if (ImGui::InputFloat("Model Scale", &m_modelScale, 0.01f))
	{
		updateModelScale();
	}

	SceneViewerWindow::renderViewToWindow();

	renderTimelineControl();
}

glm::vec2 DerydocaEngine::Components::AnimationViewerWindow::getViewPadding()
{
	return glm::vec2(0.0f, 120.0f);
}

void DerydocaEngine::Components::AnimationViewerWindow::renderToActiveBuffer()
{
	Editor::EditorRenderer::GetInstance().renderEditorCameraToActiveBuffer(
		getCamera(),
		{m_scene},
		static_cast<int>(getDisplayWidth()),
		static_cast<int>(getDisplayHeight())
	);
}

void DerydocaEngine::Components::AnimationViewerWindow::setModelScale(float modelScale)
{
	m_modelScale = modelScale;
	updateModelScale();
}

void DerydocaEngine::Components::AnimationViewerWindow::renderTimelineControl()
{
	float animationDuration = 0.0f;
	auto anim = m_meshRenderer->getAnimation();
	if (anim)
	{
		animationDuration = static_cast<float>(anim->getDuration());
	}

	ImGui::PushItemWidth(-1);
	ImGui::SliderFloat("Time", &m_animationTime, 0.0f, animationDuration, "%.2f sec");
	ImGui::PopItemWidth();

	if (ImGui::Button(m_playing ? "Pause" : "Play"))
	{
		m_playing = !m_playing;
	}

	ImGui::SameLine();
	ImGui::Checkbox("Loop", &m_looping);
	ImGui::SameLine();
	ImGui::InputFloat("Playback Speed", &m_playbackSpeed);
}

void DerydocaEngine::Components::AnimationViewerWindow::updateModelScale()
{
	m_meshRenderer->getGameObject()->getTransform()->setScale(glm::vec3(m_modelScale));
}
