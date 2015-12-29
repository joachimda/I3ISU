#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP
#include <osapi/Utility.hpp>
#include <pthread.h>

namespace osapi
{

	class Mutex
	{
	public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
	private:
		friend class Conditional;
		pthread_mutex_t mutte;
	};
}

#endif