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
		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void Draw_Component(Entity entity);
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}