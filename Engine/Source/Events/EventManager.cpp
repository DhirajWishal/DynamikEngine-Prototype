#include "dmkafx.h"
#include "EventManager.h"

#include "KeyEventComponent.h"
#include "TextEventComponent.h"
#include "MouseButtonEventComponent.h"
#include "MouseScrollEventComponent.h"
#include "DropPathEventComponent.h"
#include "WindowResizeEventComponent.h"

#include "keyCodes.h"

#define LOCK_AND_ADD_COMPONENT(component)	{														\
												std::lock_guard<std::mutex> _lockGuard(myMutex);	\
												myInstance.events.pushBack(&component);				\
											}

namespace Dynamik {
	EventManager EventManager::myInstance;
	
	void EventManager::setEventCallbacks(GLFWwindow* window)
	{
		DMK_BEGIN_PROFILE_TIMER();

		myInstance.myWindowPointer = window;

		glfwSetKeyCallback(window, _keyCallback);

		glfwSetCharCallback(window, _textCallback);

		glfwSetCursorPosCallback(window, _cursorPositionCallback);
		glfwSetMouseButtonCallback(window, _mouseButtonCallback);
		glfwSetScrollCallback(window, _mouseScrollCallback);
		glfwSetCursorEnterCallback(window, _mouseCursorEnterCallback);

		glfwSetDropCallback(window, _applicationDropPathCallback);

		glfwSetFramebufferSizeCallback(window, _applicationResizeCallback);

		glfwSetWindowCloseCallback(window, _windowCloseCallback);
	}

	/* Returns false if the GLFW window is closed */
	B1 EventManager::pollEventsGLFW()
	{
		glfwPollEvents();

		return !myInstance.isWindowClosed;
	}
	
	CursorPosition EventManager::getCursorPosition()
	{
		return myInstance.myCursorPosition;
	}

	D64 EventManager::getTime()
	{
		return glfwGetTime();
	}

	void EventManager::setTime(D64 time)
	{
		glfwSetTime(time);
	}

	std::string EventManager::getClipboard()
	{
		return glfwGetClipboardString(NULL);
	}

	void EventManager::setClipboard(std::string text)
	{
		glfwSetClipboardString(myInstance.myWindowPointer, text.c_str());
	}

	POINTER<GLFWwindow> EventManager::getCurrentContext()
	{
		return glfwGetCurrentContext();
	}

	ARRAY<POINTER<DMKEventComponent>> EventManager::getEventComponents()
	{
		DMK_BEGIN_PROFILE_TIMER();

		return myInstance.events;
	}

	void EventManager::clearContainer()
	{
		myInstance._clearContainer();
	}

	B1 EventManager::isCursorOnCurrent()
	{
		return myInstance.isCursorInThisWindow;
	}

	std::pair<D64, D64> EventManager::getCursorPoss()
	{
		D64 _xOffset = 0.0, _yOffset = 0.0;
		glfwGetCursorPos(myInstance.myWindowPointer, &_xOffset, &_yOffset);
		return { _xOffset, _yOffset };
	}

	void EventManager::_keyCallback(GLFWwindow* window, I32 key, I32 scancode, I32 action, I32 mods)
	{
		DMKKeyEventComponent _component;
		_component.window = window;

		switch (action)
		{
		case DMK_PRESS:
			_component.type = DMKEventType::DMK_EVENT_TYPE_PRESS;
			break;
		case DMK_REPEAT:
			_component.type = DMKEventType::DMK_EVENT_TYPE_REPEAT;
			break;
		case DMK_BUTTON_RELEASE:
			_component.type = DMKEventType::DMK_EVENT_TYPE_RELEASE;
			break;

		default:
			_component.type = DMKEventType::DMK_EVENT_TYPE_UNKNOWN;
			break;
		}

		_component.keycode = key;
		_component.scancode = scancode;
		_component.mods = mods;

		_pushToContainer(_component);
	}
	
	void EventManager::_textCallback(GLFWwindow* window, UI32 codepoint)
	{
		DMKTextEventComponent _component;
		_component.window = window;

		_component.charCode = codepoint;

		_pushToContainer(_component);
	}
	
	void EventManager::_cursorPositionCallback(GLFWwindow* window, D64 xOffset, D64 yOffset)
	{
		CursorPosition _positionContainer;

		_positionContainer.xOffset = xOffset;
		_positionContainer.yOffset = yOffset;

		myInstance.myCursorPosition = _positionContainer;
	}
	
	void EventManager::_mouseButtonCallback(GLFWwindow* window, I32 button, I32 action, I32 mods)
	{
		DMKMouseButtonEventComponent _component;
		_component.window = window;

		switch (action)
		{
		case DMK_PRESS:
			_component.type = DMKEventType::DMK_EVENT_TYPE_PRESS;
			break;
		case DMK_REPEAT:
			_component.type = DMKEventType::DMK_EVENT_TYPE_REPEAT;
			break;
		case DMK_BUTTON_RELEASE:
			_component.type = DMKEventType::DMK_EVENT_TYPE_RELEASE;
			break;

		default:
			_component.type = DMKEventType::DMK_EVENT_TYPE_UNKNOWN;
			break;
		}

		_component.button = button;
		_component.mods = mods;

		_pushToContainer(_component);
	}
	
	void EventManager::_mouseScrollCallback(GLFWwindow* window, D64 xOffset, D64 yOffset)
	{
		DMKMouseScrollEventComponent _component;
		_component.window = window;

		_component.xOffset = xOffset;
		_component.yOffset = yOffset;

		_pushToContainer(_component);
	}
	
	void EventManager::_mouseCursorEnterCallback(GLFWwindow* window, I32 entered)
	{
		myInstance.isCursorInThisWindow = entered;
	}
	
	void EventManager::_applicationDropPathCallback(GLFWwindow* window, I32 count, const CHR** strings)
	{
		DMKDropPathEventComponent _component;
		_component.window = window;

		_component.count = count;

		for (UI32 i = 0; i < count; i++)
			_component.paths.pushBack(strings[i]);

		_pushToContainer(_component);
	}
	
	void EventManager::_applicationResizeCallback(GLFWwindow* window, I32 width, I32 height)
	{
		DMKWindowResizeEventComponent _component;
		_component.window = window;

		_component.width = width;
		_component.height = height;

		_pushToContainer(_component);
	}
	
	void EventManager::_windowCloseCallback(GLFWwindow* window)
	{
		myInstance.isWindowClosed = true;
	}
	
	inline void EventManager::_clearContainer()
	{
		if (!myInstance.events.size())
			return;

		for (auto _component : myInstance.events)
			StaticAllocator<DMKEventComponent>::deAllocate(_component, 0);

		myInstance.events.clear();
	}
}