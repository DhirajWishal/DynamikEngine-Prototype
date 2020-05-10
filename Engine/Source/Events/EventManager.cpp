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
	DMKEventManager DMKEventManager::myInstance;
	
	void DMKEventManager::setEventCallbacks(GLFWwindow* window)
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
	B1 DMKEventManager::pollEventsGLFW()
	{
		glfwPollEvents();

		return !myInstance.isWindowClosed;
	}
	
	CursorPosition DMKEventManager::getCursorPosition()
	{
		glfwGetCursorPos(myInstance.myWindowPointer, &myInstance.myCursorPosition.xOffset, &myInstance.myCursorPosition.yOffset);
		return myInstance.myCursorPosition;
	}

	D64 DMKEventManager::getTime()
	{
		return glfwGetTime();
	}

	void DMKEventManager::setTime(D64 time)
	{
		glfwSetTime(time);
	}

	std::string DMKEventManager::getClipboard()
	{
		return glfwGetClipboardString(NULL);
	}

	void DMKEventManager::setClipboard(std::string text)
	{
		glfwSetClipboardString(myInstance.myWindowPointer, text.c_str());
	}

	POINTER<GLFWwindow> DMKEventManager::getCurrentContext()
	{
		return glfwGetCurrentContext();
	}

	ARRAY<POINTER<DMKEventComponent>> DMKEventManager::getEventComponents()
	{
		DMK_BEGIN_PROFILE_TIMER();

		return myInstance.events;
	}

	void DMKEventManager::clearContainer()
	{
		myInstance._clearContainer();
	}

	B1 DMKEventManager::isCursorOnCurrent()
	{
		return myInstance.isCursorInThisWindow;
	}

	void DMKEventManager::_keyCallback(GLFWwindow* window, I32 key, I32 scancode, I32 action, I32 mods)
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
	
	void DMKEventManager::_textCallback(GLFWwindow* window, UI32 codepoint)
	{
		DMKTextEventComponent _component;
		_component.window = window;

		_component.charCode = codepoint;

		_pushToContainer(_component);
	}
	
	void DMKEventManager::_cursorPositionCallback(GLFWwindow* window, D64 xOffset, D64 yOffset)
	{
		CursorPosition _positionContainer;

		_positionContainer.xOffset = xOffset;
		_positionContainer.yOffset = yOffset;

		myInstance.myCursorPosition = _positionContainer;
	}
	
	void DMKEventManager::_mouseButtonCallback(GLFWwindow* window, I32 button, I32 action, I32 mods)
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
	
	void DMKEventManager::_mouseScrollCallback(GLFWwindow* window, D64 xOffset, D64 yOffset)
	{
		DMKMouseScrollEventComponent _component;
		_component.window = window;

		_component.xOffset = xOffset;
		_component.yOffset = yOffset;

		_pushToContainer(_component);
	}
	
	void DMKEventManager::_mouseCursorEnterCallback(GLFWwindow* window, I32 entered)
	{
		myInstance.isCursorInThisWindow = entered;
	}
	
	void DMKEventManager::_applicationDropPathCallback(GLFWwindow* window, I32 count, const CHR** strings)
	{
		DMKDropPathEventComponent _component;
		_component.window = window;

		_component.count = count;

		for (UI32 i = 0; i < count; i++)
			_component.paths.pushBack(strings[i]);

		_pushToContainer(_component);
	}
	
	void DMKEventManager::_applicationResizeCallback(GLFWwindow* window, I32 width, I32 height)
	{
		DMKWindowResizeEventComponent _component;
		_component.window = window;

		_component.width = width;
		_component.height = height;

		_pushToContainer(_component);
	}
	
	void DMKEventManager::_windowCloseCallback(GLFWwindow* window)
	{
		myInstance.isWindowClosed = true;
	}
	
	inline void DMKEventManager::_clearContainer()
	{
		if (!myInstance.events.size())
			return;

		for (auto _component : myInstance.events)
			StaticAllocator<DMKEventComponent>::deAllocate(_component, 0);

		myInstance.events.clear();
	}
}