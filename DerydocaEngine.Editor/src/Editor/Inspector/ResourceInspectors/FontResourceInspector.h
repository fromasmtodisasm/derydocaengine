#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Resources\FontResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class FontResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::FontResource>(object);

			ImGui::Text("Font");
			ImGui::Text(resource->getName().c_str());
		}
	};

}
