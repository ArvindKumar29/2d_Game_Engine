#pragma once

//#include "Hazle/Debug/Instrumentor.h"
#include "Hazle/Core/core.h"
#include <string>
#include <type_traits>
#define FMT_HEADER_ONLY
#include <spdlog/fmt/ostr.h>

namespace Hazle {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	// EventType is used to categorize events. We can have a mouse event, a keyboard event, a window resize event, etc.
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// EventCategory is used to group events into categories. For example, a mouse event can be in the Mouse category, and a keyboard event can be in the Keyboard category.
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};


// Macros to help define event classes. These macros will be used in the event classes to define type of the event.
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

// Macros to help define event classes. These macros will be used in the event classes to define the category of the event.
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	// EventDispatcher is a utility class that helps in dispatching events to the appropriate event handler based on the event type. 
	// It takes an event and allows you to call a function if the event type matches.
	// The Dispatch function is a template function that takes a function as an argument and calls it if the event type matches and the event is not already handled.
	// In case if the event type does not match, it returns false and does not call the function.
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	// Osstream operator overload for Event class. 
	// This allows us to use the << operator to print the event to an output stream.
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<Hazle::Event, T>, char>> : fmt::ostream_formatter {};