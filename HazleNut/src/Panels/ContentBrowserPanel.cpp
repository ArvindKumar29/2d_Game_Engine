#include <hzpch.h>
#include "ContentBrowserPanel.h"
#include "imgui.h"
#include "Hazle/Core/Log.h"

namespace Hazle
{
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_FileIcon		= Texture2D::Create("assets/Textures/document.png");
		m_DirectoryIcon = Texture2D::Create("assets/Textures/folder.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbNailSize = 128.0f;
		float cellSize = thumbNailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		int i = 0;
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			ImGui::PushID(i++);
			const auto& path = directoryEntry.path();
			auto& relativePath = std::filesystem::relative(directoryEntry.path(), g_AssetPath);
			std::string fileNameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { thumbNailSize, thumbNailSize }, {0, 1}, {1, 0} );
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}
			
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(fileNameString.c_str());
			ImGui::NextColumn();

			ImGui::PopID();
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size", &thumbNailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);
		ImGui::End();
	}
}