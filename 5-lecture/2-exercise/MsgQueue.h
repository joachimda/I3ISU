#include <queue>
#include "Message.h"
#include <pthread.h>

struct qItem
{
	unsigned long id_;
	Message * msg_;
};

class MsgQueue
{
	public:
		MsgQueue(unsigned long maxSize)
		{
			maxSize_ = maxSize;
		}
		void send(unsigned long id, Message* msg = NULL)
		{
			pthread_mutex_lock(&mtx);

			// Block if queue is full
			while (msgContainer.size() >= maxSize_) 
			{
				pthread_cond_wait(&reciever, &mtx);
			}
			qItem item = { .id_ = id, .msg_ = msg};
			msgContainer.push(item);
			pthread_cond_signal(&sender);
			pthread_mutex_unlock(&mtx);
		}

		Message* recieve(unsigned long &id) 
		{
			pthread_mutex_lock(&mtx);

			// Block if queue is empty
			while (msgContainer.empty()) 
			{
				pthread_cond_wait(&sender, &mtx);
			}
			qItem item = msgContainer.front();
			msgContainer.pop();
			pthread_cond_signal(&reciever);
			pthread_mutex_unlock(&mtx);
			id = item.id_;
			return item.msg_;
		}

		~MsgQueue()
		{
			pthread_mutex_destroy(&mtx);
			pthread_cond_destroy(&sender);
			pthread_cond_destroy(&reciever);
		}

	private:
		std::queue<qItem> msgContainer;
		unsigned long maxSize_;
		pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t sender = PTHREAD_COND_INITIALIZER;
		pthread_cond_t reciever = PTHREAD_COND_INITIALIZER;
};