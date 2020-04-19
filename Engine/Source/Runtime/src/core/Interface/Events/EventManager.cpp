#include "dmkafx.h"
#include "EventManager.h"

#include "DMKKeyEventComponent.h"
#include "DMKTextEventComponent.h"
#include "DMKMouseButtonEventComponent.h"
#include "DMKMouseScrollEventComponent.h"
#include "DMKDropPathEventComponent.h"
#include "DMKWindowResizeEventComponent.h"

#define LOCK_AND_ADD_COMPONENT(component)	{														\
												std::lock_guard<std::mutex> _lockGuard(myMutex);	\
												myInstance.events.pushBack(&component);				\
											}

namespace Dynamik {
	std::mutex myMutex;

	EventManager EventManager::myInstance;
	
	void EventManager::setEventCallbacks(GLFWwindow* window)
	{
		myInstance.myWindowPointer = window;

		glfwSetKeyCallback(window, _keyCallback);

		glfwSetCharCallback(window, _textCallback);

		glfwSetCursorPosCallback(window, _cursorPositionCallback);
		glfwSetMouseButtonCallback(window, _mouseButtonCallback);
		glfwSetScrollCallback(window, _mouseScrollCallback);
		glfwSetCursorEnterCallback(window, _mouseCursorEnterCallback);

		glfwSetDropCallback(window, _applicationDropPathCallback);

		glfwSetFramebufferSizeCallback(window, _applicationResizeCallback);
	}

	void EventManager::pollEventsGLFW()
	{
		myInstance.events.clear();
		glfwPollEvents();
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

	ARRAY<DMKEventComponent*> EventManager::getEventComponents()
	{
		return myInstance.events;
	}

	B1 EventManager::isCursorOnCurrent()
	{
		return myInstance.isCursorInThisWindow;
	}

	void EventManager::_keyCallback(GLFWwindow* window, I32 key, I32 scancode, I32 action, I32 mods)
	{
		DMKKeyEventComponent _component;
		_component.window = window;

		switch (action)
		{
		case DMK_PRESS:
			_component.type = DMKEventType::DMK_EVENT_TYPE_KEY_PRESS;
			break;
		case DMK_REPEAT:
			_component.type = DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT;
			break;
		case DMK_BUTTON_RELEASE:
			_component.type = DMKEventType::DMK_EVENT_TYPE_KEY_RELEASE;
			break;

		default:
			_component.type = DMKEventType::DMK_EVENT_TYPE_UNKNOWN;
			break;
		}

		_component.keycode = key;
		_component.scancode = scancode;
		_component.mods = mods;

		LOCK_AND_ADD_COMPONENT(_component)
	}
	
	void EventManager::_textCallback(GLFWwindow* window, UI32 codepoint)
	{
		DMKTextEventComponent _component;
		_component.window = window;

		_component.charCode = codepoint;

		LOCK_AND_ADD_COMPONENT(_component)
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
			_component.type = DMKEventType::DMK_EVENT_TYPE_KEY_PRESS;
			break;
		case DMK_REPEAT:
			_component.type = DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT;
			break;
		case DMK_BUTTON_RELEASE:
			_component.type = DMKEventType::DMK_EVENT_TYPE_KEY_RELEASE;
			break;

		default:
			_component.type = DMKEventType::DMK_EVENT_TYPE_UNKNOWN;
			break;
		}

		_component.button = button;
		_component.mods = mods;

		LOCK_AND_ADD_COMPONENT(_component)
	}
	
	void EventManager::_mouseScrollCallback(GLFWwindow* window, D64 xOffset, D64 yOffset)
	{
		DMKMouseScrollEventComponent _component;
		_component.window = window;

		_component.xOffset = xOffset;
		_component.yOffset = yOffset;

		LOCK_AND_ADD_COMPONENT(_component)
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

		LOCK_AND_ADD_COMPONENT(_component)
	}
	
	void EventManager::_applicationResizeCallback(GLFWwindow* window, I32 width, I32 height)
	{
		DMKWindowResizeEventComponent _component;
		_component.window = window;

		_component.width = width;
		_component.height = height;

		LOCK_AND_ADD_COMPONENT(_component)
	}
}