#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Hazle/Scene/Component.h"
#include "Hazle/Core/core.h"
#include <glm/gtc/type_ptr.hpp>

namespace Hazle
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		auto view = m_Context->m_Registry.view<CTag>();
		for (auto& entityID : view)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);	
		}
		 
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext.m_Scene != nullptr)
		{
			DrawComponent(m_SelectionContext);
		}

		ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<CTag>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		
		if (opened)
		{
			ImGui::TreePop();
		}

	}

	void SceneHierarchyPanel::DrawComponent(Entity entity)
	{
		if (entity.hasComponent<CTag>())
		{
			auto& tag = entity.getComponent<CTag>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		if (entity.hasComponent<CTransform>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CTransform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponent<CTransform>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<CCamera>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CCamera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.getComponent<CCamera>();
				auto& camera = cameraComponent.camera;

				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}

					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{

					float persFOV = glm::degrees(camera.GetPerspectiveVerticleFOV());
					if(ImGui::DragFloat("FOV", &persFOV))
						camera.SetPerspectiveVerticleFOV(glm::radians(persFOV));
					
					float persNear = camera.GetPerspectiveNearClip();
					if(ImGui::DragFloat("Near", &persNear))
						camera.SetPerspectiveNearClip(persNear);
					
					float persFar = camera.GetPerspectiveFarClip();
					if(ImGui::DragFloat("Far", &persFar))
						camera.SetPerspectiveFarClip(persFar);
				}
				
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if(ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);
					
					float orthoNear = camera.GetOrthographicNearClip();
					if(ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);
					
					float orthoFar = camera.GetOrthographicFarClip();
					if(ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);
				}


				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<CSpriteRenderer>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CSpriteRenderer).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& sprite = entity.getComponent<CSpriteRenderer>();
				ImGui::ColorEdit4("Color", glm::value_ptr(sprite.Color));
				ImGui::TreePop();
			}
		}
	}

}