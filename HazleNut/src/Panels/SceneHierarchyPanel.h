#pragma once

#include "Hazle/Scene/Scene.h"
#include "Hazle/Scene/Entity.h"

namespace Hazle
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}