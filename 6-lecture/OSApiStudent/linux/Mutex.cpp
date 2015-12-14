#include <osapi/Mutex.hpp>

namespace osapi
{
  Mutex::Mutex()
  { 
    pthread_mutex_init(&mutte, NULL);
  }

    Mutex::~Mutex()
  {
    pthread_mutex_destroy(&mutte);
  }
    
  void Mutex::lock()
  {
    pthread_mutex_lock(&mutte);
  }
    
  void Mutex::unlock()
  {
    pthread_mutex_unlock(&mutte);
  }
}
