#include <iostream>
#include <pthread.h>
#include <unistd.h>

int counter = 10; // lets the threads run X number of cycles

void* incrementer(void * arg);
void* reader(void * arg);

int main()
{
    pthread_t threads[2];

    unsigned int shared = 0;

    std::cout << "Creating threads..\n";
    pthread_create(&threads[0], NULL, &incrementer, (void*)&shared);
    pthread_create(&threads[1], NULL, &reader, (void*)&shared);

    std::cout << "Threads doing their thing..\n";

    std::cout << "Joining threads\n";
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    std::cout << "Done! Process exiting..\n";
    return 0;
}

void* incrementer(void* arg)
{
    while(counter > 0)
    {
//Incrementing casted arg
        (*(unsigned int*)arg)++;
        sleep(1);
        counter--;
    }
}

void* reader(void* arg)
{
    while(counter > 0)
    {
//Printing casted arg
        std::cout << "Shared = " << (*(unsigned int*)arg) << "\n";
        sleep(1);
    }
}
