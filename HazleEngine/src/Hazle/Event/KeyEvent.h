#pragma once

#include "Hazle/Event/Event.h"
#include "Hazle/Core/KeyCodes.h"
#include <sstream>

namespace Hazle {

	// Class KeyEvent is an abstract class that represents a keyboard event. 
	// It inherits from the Event class and provides a common interface for all keyboard events. 
	// The KeyEvent class contains a member variable m_KeyCode that stores the key code associated with the event. 
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput) // This is a keyboard event and an input event
	protected:
		KeyEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	// The KeyPressedEvent class represents a key press event. 
	// It inherits from the KeyEvent class and adds a member variable m_RepeatCount that stores the number of times the key has been repeated.
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)			// This is a key pressed event
	private:
		bool m_IsRepeat;						// Indicates whether the key press event is a repeat event(press and hold)
	};

	// The KeyReleasedEvent class represents a key release event as same as the KeyPressedEvent.
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	// The KeyTypedEvent class represents a key typed event.
	// Unlike the KeyPressedEvent class, the KeyTypedEvent class does not have a repeat count.
	// Therefore, it is used to represent a single key press event that results in a character being typed.
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)			// This is a key typed event
	};
}