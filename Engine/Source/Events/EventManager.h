#pragma once
#ifndef _DYNAMIK_EVENT_MANAGER_H
#define _DYNAMIK_EVENT_MANAGER_H

#include <mutex>
#include <vector>
#include "EventComponent.h"

namespace Dynamik {
	struct CursorPosition {
		D64 xOffset = 0.0f;
		D64 yOffset = 0.0f;
	};

	class DMKEventManager {
		DMKEventManager() {}
		~DMKEventManager() {}
		static DMKEventManager myInstance;

	public:
		DMKEventManager(const DMKEventManager&) = delete;
		DMKEventManager& operator=(DMKEventManager&&) = delete;

		static void setEventCallbacks(GLFWwindow* window);
		static B1 pollEventsGLFW();
		static CursorPosition getCursorPosition();

		static D64 getTime();
		static void setTime(D64 time);

		static std::string getClipboard();
		static void setClipboard(std::string text);

		static POINTER<GLFWwindow> getCurrentContext();

		static std::vector<POINTER<DMKEventComponent>> getEventComponents();
		static void clearContainer();
		static B1 isCursorOnCurrent();
		/* Returns the last recorded event of the button */
		static I32 getMouseButton(I32 button);
		/* Returns the last recorded event of the key */
		static I32 getKey(UI32 key);

	private:
		static void _keyCallback(GLFWwindow* window, I32 key, I32 scancode, I32 action, I32 mods);
		static void _textCallback(GLFWwindow* window, UI32 codepoint);
		static void _cursorPositionCallback(GLFWwindow* window, D64 xOffset, D64 yOffset);
		static void _mouseButtonCallback(GLFWwindow* window, I32 button, I32 action, I32 mods);
		static void _mouseScrollCallback(GLFWwindow* window, D64 xOffset, D64 yOffset);
		static void _mouseCursorEnterCallback(GLFWwindow* window, I32 entered);
		static void _applicationDropPathCallback(GLFWwindow* window, I32 count, const CHR** strings);
		static void _applicationResizeCallback(GLFWwindow* window, I32 width, I32 height);

		static void _windowCloseCallback(GLFWwindow* window);

		GLFWwindow* myWindowPointer;
		CursorPosition myCursorPosition;

		std::vector<POINTER<DMKEventComponent>> events;
		B1 isCursorInThisWindow = false;
		B1 isWindowClosed = false;

		/* This function is passed to GLFW to push a new ecent container to the queue. */
		template<class TYPE>
		static inline void _pushToContainer(const TYPE& component)
		{
			POINTER<DMKEventComponent> _component = StaticAllocator<DMKEventComponent>::allocate(sizeof(TYPE));
			StaticAllocator<TYPE>::set(_component, (TYPE&&)component);

			std::lock_guard<std::mutex> _lockGuard(myInstance.myMutex);
			myInstance.events.push_back(_component);
		}

		static inline void _clearContainer();

		POINTER<std::thread> utilityThread;
		std::mutex myMutex;
	};

	class DMKEventBuffer {
	public:
		DMKEventBuffer() {}
		~DMKEventBuffer() {}
	};
}

#endif // !_DYNAMIK_EVENT_H
