#include "EngineComponentsPch.h"
#include "TessellatedMeshRenderer.h"

#include <GL\glew.h>
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	TessellatedMeshRenderer::TessellatedMeshRenderer() :
		m_vao(0),
		m_vbo(0),
		m_useDynamicTessellation(false),
		m_tessellationLevel(4),
		m_minDynamicTessLevel(2),
		m_maxDynamicTessLevel(8),
		m_minDynamicTessDistance(1.0f),
		m_maxDynamicTessDistance(10.0f),
		m_mesh(),
		m_material(std::make_shared<Rendering::Material>())
	{
	}

	TessellatedMeshRenderer::~TessellatedMeshRenderer()
	{
	}

	void TessellatedMeshRenderer::init()
	{
		glGenBuffers(1, &m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH * sizeof(float), m_mesh->getPatchData(), GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glPatchParameteri(GL_PATCH_VERTICES, 16);

		updateMaterial();
	}

	void TessellatedMeshRenderer::deserialize(const YAML::Node& compNode)
	{
		YAML::Node tessellationLevelNode = compNode["tessellationLevel"];
		if (tessellationLevelNode)
		{
			m_tessellationLevel = tessellationLevelNode.as<int>();
		}

		YAML::Node useDynamicTessellationNode = compNode["useDynamicTessellation"];
		if (useDynamicTessellationNode)
		{
			m_useDynamicTessellation = useDynamicTessellationNode.as<bool>();
		}

		YAML::Node minDynamicTessDistanceNode = compNode["minDynamicTessDistance"];
		if (minDynamicTessDistanceNode)
		{
			m_minDynamicTessDistance = minDynamicTessDistanceNode.as<float>();
		}

		YAML::Node maxDynamicTessDistanceNode = compNode["maxDynamicTessDistance"];
		if (maxDynamicTessDistanceNode)
		{
			m_maxDynamicTessDistance = maxDynamicTessDistanceNode.as<float>();
		}

		YAML::Node minDynamicTessLevelNode = compNode["minDynamicTessLevel"];
		if (minDynamicTessLevelNode)
		{
			m_minDynamicTessLevel = minDynamicTessLevelNode.as<int>();
		}

		YAML::Node maxDynamicTessLevelNode = compNode["maxDynamicTessLevel"];
		if (maxDynamicTessLevelNode)
		{
			m_maxDynamicTessLevel = maxDynamicTessLevelNode.as<int>();
		}

		auto sourceMaterial = getResourcePointer<Rendering::Material>(compNode, "material");
		m_material->copyFrom(sourceMaterial);
		m_mesh = getResourcePointer<BezierPatchMesh>(compNode, "bezierPatchMesh");
	}

	void TessellatedMeshRenderer::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		glBindVertexArray(m_vao);
		glPatchParameteri(GL_PATCH_VERTICES, 16);
		glDrawArrays(GL_PATCHES, 0, m_mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH);

		glFinish();
	}

	void TessellatedMeshRenderer::renderMesh(
		const std::shared_ptr<Rendering::MatrixStack> matrixStack,
		std::shared_ptr<Rendering::Material> material,
		const Rendering::Projection& projection,
		const std::shared_ptr<Components::Transform> projectionTransform
	)
	{
		// Consider an alternate to using the same material as what is being used to render to screen
		//  The calculations may be excessive for a shadow calc
		m_material->bind();
		m_material->getShader()->update(matrixStack, projection, projectionTransform);
		glBindVertexArray(m_vao);
		glPatchParameteri(GL_PATCH_VERTICES, 16);
		glDrawArrays(GL_PATCHES, 0, m_mesh->getNumPatches() * BezierPatchMesh::FLOATS_PER_PATCH);

		glFinish();
	}

	void TessellatedMeshRenderer::update(const float deltaTime)
	{
		updateMaterial();
	}

	void TessellatedMeshRenderer::updateMaterial()
	{
		m_material->setInt("UseDynamicTessellation", m_useDynamicTessellation ? 1 : 0);
		m_material->setInt("TessLevel", m_tessellationLevel);
		m_material->setFloat("MinTessLevel", (float)m_minDynamicTessLevel);
		m_material->setFloat("MaxTessLevel", (float)m_maxDynamicTessLevel);
		m_material->setFloat("MinDistance", m_minDynamicTessDistance);
		m_material->setFloat("MaxDistance", m_maxDynamicTessDistance);
	}

}
