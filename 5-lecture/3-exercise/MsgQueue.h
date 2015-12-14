#ifndef MSGQUEUE_H
#define MSGQUEUE_H
#include <queue>
#include "Message.h"
#include <pthread.h>

struct qItem {
	unsigned long id_;
	Message* msg_;
};

class MsgQueue : public Message {
public:
	MsgQueue(unsigned long maxSize)	{
		maxSize_ = maxSize;
	}

	~MsgQueue()	{
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&notEmptySignal);
		pthread_cond_destroy(&notFullSignal);
	}

	void send(unsigned long id, Message* msg) {
		pthread_mutex_lock(&mutex);

			// Block if queue is full
		while (messageQueue.size() >= maxSize_) {
			pthread_cond_wait(&notFullSignal, &mutex);
		}

		qItem item = { .id_ = id, .msg_ = msg};
		messageQueue.push(item);

		pthread_cond_signal(&notEmptySignal);
		pthread_mutex_unlock(&mutex);
	}

	Message* receive(unsigned long &id) {
		pthread_mutex_lock(&mutex);

		// Block if queue is empty
		while (messageQueue.empty()) {
			pthread_cond_wait(&notEmptySignal, &mutex);
		}

		qItem item = messageQueue.front();
		messageQueue.pop();
		
		id = item.id_;
		pthread_cond_signal(&notFullSignal);
		pthread_mutex_unlock(&mutex);

		return item.msg_;
	}
private:
	std::queue<qItem> messageQueue;
	unsigned long maxSize_;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t notEmptySignal = PTHREAD_COND_INITIALIZER;
	pthread_cond_t notFullSignal = PTHREAD_COND_INITIALIZER;
};

#endif // MSGQUEUE_H