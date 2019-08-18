#pragma once
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

