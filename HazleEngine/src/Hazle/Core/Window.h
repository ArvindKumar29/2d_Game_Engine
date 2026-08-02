#pragma once

#include <hzpch.h>
#include "Hazle/Core/core.h"
#include "Hazle/Event/Event.h"
#include "Hazle/Core/Log.h"

namespace Hazle {
	// WindowProps is a struct that holds the properties of a window, such as its title, width, and height.
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

	 //intetrface representing a desktop system based Window
	// This is an abstract class that defines the interface for a window in the Hazle Engine.
	// It has pure virtual functions that must be implemented by derived classes such as WindowsWindow or MacWindow or LinuxWindow etc.
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0; // This function is called every frame to update the window's state and render its contents.
		
		virtual uint32_t GetWidth() const = 0; // This function returns the width of the window in pixels.
		virtual uint32_t GetHeight() const = 0; // This function returns the height of the window in pixels.

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0; // This function sets the callback function that will be called when an event occurs in the window.
		virtual void SetVSync(bool enabled) = 0; // This function enables or disables vertical synchronization.
		virtual bool IsVSync() const = 0; // This function returns whether vertical synchronization is enabled.

		static Scope<Window> Create(const WindowProps& props = WindowProps()); // This function creates a new window with the specified properties and returns a unique pointer to it.
		static void Close(); // This function closes the window and releases its resources.

		// public:
		// This function returns a pointer to the native window object, which can be used for platform-specific operations.
		// A native window object is a platform-specific representation of a window, such as an HWND on Windows or an NSWindow on macOS.
		virtual void* GetNativeWindow() const { return m_NativeWindow; } 
	private:
		void* m_NativeWindow = nullptr; // Native window object pointer, which is initialized to nullptr. It will be set to the actual native window object when the window is created.
	};
}