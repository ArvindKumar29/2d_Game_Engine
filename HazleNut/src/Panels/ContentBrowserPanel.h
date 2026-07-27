#pragma once
#include <filesystem>
#include "Hazle/Renderer/Texture.h"

namespace Hazle
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_DirectoryIcon, m_FileIcon;
	};
}