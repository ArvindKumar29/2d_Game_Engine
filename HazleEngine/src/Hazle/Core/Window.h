#pragma once

#include <hzpch.h>
#include "Hazle/Core/core.h"
#include "Hazle/Event/Event.h"
#include "Hazle/Core/Log.h"
//#include "Platform/Windows/WindowsWindow.h"

namespace Hazle {
	struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Hazle Engine",
			uint32_t width = 1440,
			uint32_t height = 920)
			: Title(title), Width(width), Height(height)
		{}
	};

	// intetrface representing a desktop system based Window
	class HAZLE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
		static void Close();
		// public:
		virtual void* GetNativeWindow() const { return m_NativeWindow; }

	private:
		void* m_NativeWindow = nullptr;
	};
}