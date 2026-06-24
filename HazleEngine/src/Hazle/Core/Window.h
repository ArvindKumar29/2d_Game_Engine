#pragma once

#include "hzpch.h"
#include "Hazle/Core/core.h"
#include "Hazle/Event/Event.h"
#include "Hazle/Core/Log.h"

namespace Hazle {
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazle Engine",
			unsigned int width = 1440,
			unsigned int height = 920)
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
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
		static void Close();
		// public:
		virtual void* GetNativeWindow() const { return m_NativeWindow; }

	private:
		void* m_NativeWindow = nullptr;
	};
}