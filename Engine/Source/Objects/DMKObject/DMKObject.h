#pragma once
#ifndef _DYNAMIK_OBJECT_H
#define _DYNAMIK_OBJECT_H

#define DMK_CLASS_SINGLETON(className)		private:												\
												static className instance;							\
											public:													\
												className(const className&) = delete;				\
												className(className&&) = delete;					\
												className& operator=(const className&) = delete;	\
												className& operator=(className&&) = delete;			
#define DMK_CLASS_SINGLETON_IMPL(className)	className className::instance

namespace Dynamik {
	class DMKObject {
	public:
		DMKObject() {}
		virtual ~DMKObject() {}
	};
}

#endif // !_DYNAMIK_OBJECT_H
