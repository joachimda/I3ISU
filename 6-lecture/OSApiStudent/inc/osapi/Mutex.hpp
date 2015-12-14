#ifndef OSAPI_MUTEX_HPP_
#define OSAPI_MUTEX_HPP_

#include <osapi/MutexError.hpp>

#if defined(OS_WINX86)
#include "osapi/winx86/Mutex.hpp"
#elif defined(OS_LINUX) or defined(OS_APPLE_DARWIN)
#include "osapi/linux/Mutex.hpp"
#elif defined(OS_FREERTOS)
#include "osapi/freertos/Mutex.hpp"
#else
#error "No known OS defined"
#endif



#endif
