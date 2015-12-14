#include <pthread.h>

class Scoped_Lock 
{
public:
	Scoped_Lock(pthread_mutex_t &mtx)
	{
		_mtx = &mtx;
		pthread_mutex_lock(_mtx);
	}
	
	~Scoped_Lock()
	{
		pthread_mutex_unlock(_mtx);
	}
private:
	pthread_mutex_t *_mtx;
};