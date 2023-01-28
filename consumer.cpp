#include <bits/stdc++.h>
#include <regex>
#include <string.h>
#include <random>
#include <chrono>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;

// This code implements a producer-consumer problem with a single consumer and multiple producers.
// The producer and consumer threads are synchronized using a mutex and a condition variable.
// The mutex protects the buffer, the condition variable is used to signal the consumer when
// the buffer is not empty. The buffer is implemented as a queue, with the front of the queue
// representing the oldest element and the back of the queue representing the newest element.
// The producer threads push new elements to the back of the queue, while the consumer thread
// pops elements from the front of the queue. The consumer thread signals the producers when
// the buffer is not full, and the producers signal the consumer thread when the buffer is
// not empty.

struct msg
{
    int producer_id;
    double price_producer;
    double price_mean_producer;
};
struct msg_to_display
{
    int producer_id;
    string producer_name;
    double price_producer;
    double price_mean_producer;
};
struct buffer
{
    int start = 0;
    int end = 0;
};
msg_to_display create_message_to_be_displayed(msg &message)
{
    msg_to_display message_to_display;
    message_to_display.producer_id = message.producer_id;
    message_to_display.price_producer = message.price_producer;
    message_to_display.price_mean_producer = message.price_mean_producer;

    if (message.producer_id == 0)
    {
        message_to_display.producer_name = "ALUMINIUM";
    }
    else if (message.producer_id == 1)
    {
        message_to_display.producer_name == "COPPER";
    }
    else if (message.producer_id == 2)
    {
        message_to_display.producer_name = "COTTON";
    }
    else if (message.producer_id == 3)
    {
        message_to_display.producer_name = "CRUDEOIL";
    }
    else if (message.producer_id == 4)
    {
        message_to_display.producer_name = "GOLD";
    }
    else if (message.producer_id == 5)
    {
        message_to_display.producer_name = "LEAD";
    }
    else if (message.producer_id == 6)
    {
        message_to_display.producer_name = "MENTHAOIL";
    }
    else if (message.producer_id == 7)
    {
        message_to_display.producer_name = "NATURALGAS";
    }
    else if (message.producer_id == 8)
    {
        message_to_display.producer_name = "NICKEL";
    }
    else if (message.producer_id == 9)
    {
        message_to_display.producer_name = "SLIVER";
    }
    else if (message.producer_id == 10)
    {
        message_to_display.producer_name = "ZINC";
    }
    return message_to_display;
}

struct consumer
{
    int buffer_size;
    vector<msg_to_display> list = {
        {0, "ALUMINUM", 0.00, 0.00},
        {1, "COPPER", 0.00, 0.00},
        {2, "COTTON", 0.00, 0.00},
        {3, "CRUDEOIL", 0.00, 0.00},
        {4, "GOLD", 0.00, 0.00},
        {5, "LEAD", 0.00, 0.00},
        {6, "MENTHAOIL", 0.00, 0.00},
        {7, "NATURALGAS", 0.00, 0.00},
        {8, "NICKEL", 0.00, 0.00},
        {9, "SLIVER", 0.00, 0.00},
        {10, "ZINC", 0.00, 0.00}};
    vector<string> delta_price = {{"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}};
    vector<string> delta_mean = {{"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}, {"→"}};

    void consume(msg_to_display message)
    {

        for (size_t i = 0; i < list.size(); i++)
        {
            if ((*this).list[i].producer_id == message.producer_id)
            {
                if ((*this).list[i].price_producer < message.price_producer)
                {
                    (*this).delta_price[i] = "↑";
                    (*this).list[i].price_producer = message.price_producer;
                }
                else if ((*this).list[i].price_producer > message.price_producer)
                {
                    (*this).delta_price[i] = "↓";
                    (*this).list[i].price_producer = message.price_producer;
                }

                if ((*this).list[i].price_mean_producer < message.price_mean_producer)
                {
                    (*this).delta_mean[i] = "↑";
                    (*this).list[i].price_mean_producer = message.price_mean_producer;
                }

                else if ((*this).list[i].price_mean_producer > message.price_mean_producer)
                {
                    (*this).delta_mean[i] = "↓";
                    (*this).list[i].price_mean_producer = message.price_mean_producer;
                }
            }
        }
    }

    void display()
    {
        printf("\e[1;1H\e[2J");
        printf("+-------------------------------------+\n");
        printf("| Currency     |   Price   | AvgPrice |\n");
        printf("+-------------------------------------+\n");
        for (int i = 0; i < (*this).list.size(); i++)
        {
            printf("| %s", (*this).list[i].producer_name.c_str());
            printf("\033[%d;%dH", 4 + i, 16);
            printf("|");

            if ((*this).delta_price[i] == "↑")
            {
                printf(" \033[;32m %7.2lf\033[0m", (*this).list[i].price_producer);
                printf("\033[;32m%s\033[0m", (*this).delta_price[i].c_str());
            }
            if ((*this).delta_price[i] == "↓")
            {
                printf(" \033[;31m %7.2lf\033[0m", (*this).list[i].price_producer);
                printf("\033[;31m%s\033[0m", (*this).delta_price[i].c_str());
            }
            if ((*this).delta_price[i] == "→")
            {
                printf("  %7.2lf", (*this).list[i].price_producer);
            }

            printf("\033[%d;%dH", 4 + i, 28);
            printf("|");
            printf("\033[%d;%dH", 4 + i, 29);

            if ((*this).delta_mean[i] == "↑")
            {
                printf("\033[;32m %7.2lf\033[0m", (*this).list[i].price_mean_producer);
                printf("\033[;32m%s\033[0m", (*this).delta_mean[i].c_str());
            }
            if ((*this).delta_mean[i] == "↓")
            {
                printf("\033[;31m %7.2lf\033[0m", (*this).list[i].price_mean_producer);
                printf("\033[;31m%s\033[0m", (*this).delta_mean[i].c_str());
            }
            if ((*this).delta_mean[i] == "→")
            {
                printf("  %7.2lf", (*this).list[i].price_mean_producer);
            }
            printf(" |\n");
        }
        printf("+-------------------------------------+\n");
    }
};
consumer create_consumer(int buffer_size)
{
    consumer consumer;
    consumer.buffer_size = buffer_size;
    return consumer;
}

int main(int argc, char *argv[])
{

/*
* 1. We create the consumer and the shared memory.
* 2. We create the 3 semaphores we need for the producer-consumer relationship: empty, full and exclusion.
* 3. We initialize the semaphores to the right values.
* 4. We initialize the start and end of the buffer to 0. 
*/

    consumer consumer = create_consumer(stoi(argv[1]));

    int buffer_size = stoi(argv[1]) * sizeof(msg);
    key_t key = ftok(".", 650000);
    int shmid = shmget(key, buffer_size, 0666 | IPC_CREAT);
    struct msg *shared_buffer = (struct msg *)shmat(shmid, (void *)0, 0);

    key_t key_start = ftok(".", 20000);
    int shmid_start = shmget(key_start, sizeof(buffer), 0666 | IPC_CREAT);
    struct buffer *area_pointer = (struct buffer *)shmat(shmid_start, (void *)0, 0);

    area_pointer->start = 0;
    area_pointer->end = 0;

    key_t key_semaphore_1 = ftok(".", 10000);
    int empty = semget(key_semaphore_1, 1, 0660 | IPC_CREAT); // semaphore to check if the buffer is fully consumed
    semctl(empty, 0, SETVAL, consumer.buffer_size);

    key_t key_semaphore_2 = ftok(".", 20000);
    int full = semget(key_semaphore_2, 1, 0660 | IPC_CREAT); // semaphore to check if the buffer is full

    semctl(full, 0, SETVAL, 0);

    key_t key_semaphore_3 = ftok(".", 30000);
    int exclusion = semget(key_semaphore_3, 1, 0660 | IPC_CREAT); // semaphore to check if the buffer is fully consumed
    semctl(exclusion, 0, SETVAL, 1);

    struct sembuf sem_wait, sem_signal;
    sem_signal.sem_num = sem_wait.sem_num = sem_wait.sem_flg = sem_signal.sem_flg = 0;
    sem_signal.sem_op = 1;
    sem_wait.sem_op = -1;



 while (true)
    {
        semop(full, &sem_wait, 1);
        semop(exclusion, &sem_wait, 1);
        msg_to_display msg = create_message_to_be_displayed(shared_buffer[area_pointer->start]);
        (area_pointer->start)++;
        if (area_pointer->start == consumer.buffer_size)
        {
            area_pointer->start = 0;
        }
        semop(exclusion, &sem_signal, 1);
        semop(empty, &sem_signal, 1);
        consumer.consume(msg);
        consumer.display();
    }

    // detach from shared memory
    shmdt(shared_buffer);

    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
}