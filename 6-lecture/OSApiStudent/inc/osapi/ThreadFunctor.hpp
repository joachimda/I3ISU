#ifndef OSAPI_THREADFUNCTOR_HPP
#define OSAPI_THREADFUNCTOR_HPP


#if defined(OS_WINX86)
#include "osapi/winx86/ThreadFunctor.hpp"
#elif defined(OS_LINUX) or defined(OS_APPLE_DARWIN)
#include "osapi/linux/ThreadFunctor.hpp"
#elif defined(OS_FREERTOS)
#include "osapi/freertos/ThreadFunctor.hpp"
#else
#error "No known OS defined"
#endif



#endif
