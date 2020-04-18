#pragma once

#ifndef _DYNAMIK_AUDIO_LOG_H
#define _DYNAMIK_AUDIO_LOG_H

#include <iostream>

#define ASSERT(x, ...)	{																						\
							if(!(x)){																			\
								std::cout << __VA_ARGS__ << " @ " <<  __FILE__ << ":" << __LINE__ << std::endl;	\
								__debugbreak();																	\
							}																					\
					}

#ifdef AUDIO_LOG1
#define LOG(...) std::cout << __VA_ARGS__ << std::endl;

#else
#define LOG(...)

#endif

#endif // !_DYNAMIK_AUDIO_LOH_H
