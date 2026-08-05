#pragma once

#include "Hazle/Event/Event.h"

namespace Hazle {

	// WindowResizeEvent class represents an event that occurs when the window is resized.
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override							// Override the ToString method to provide a string representation of the event
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)									// Define the event type as WindowResize
			EVENT_CLASS_CATEGORY(EventCategoryApplication)				// Define the event category as Application
	private:
		unsigned int m_Width, m_Height;
	};

	// WindowCloseEvent class represents an event that occurs when the window is closed.
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)									// Define the event type as WindowClose
			EVENT_CLASS_CATEGORY(EventCategoryApplication)				// Define the event category as Application
	};

	// AppTickEvent class represents an event that occurs on each application tick.
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)										// Define the event type as AppTick
			EVENT_CLASS_CATEGORY(EventCategoryApplication)				// Define the event category as Application
	};

	// AppUpdateEvent class represents an event that occurs when the application is updated.
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)									// Define the event type as AppUpdate
			EVENT_CLASS_CATEGORY(EventCategoryApplication)			// Define the event category as Application
	};

	// AppRenderEvent class represents an event that occurs when the application is rendered.
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)									// Define the event type as AppRender
			EVENT_CLASS_CATEGORY(EventCategoryApplication)			// Define the event category as Application
	};
}