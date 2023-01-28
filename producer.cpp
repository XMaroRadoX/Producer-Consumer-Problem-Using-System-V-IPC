#include <bits/stdc++.h>
#include <iostream>
#include <sys/time.h>
#include <regex>
#include <string.h>
#include <random>
#include <chrono>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <vector>

// vector max size = 11
using namespace std;
void time_stamp()
{
    char buffer[26];
    int millisec;
    struct tm *tm_info;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    millisec = lrint(tv.tv_usec / 1000.0);
    if (millisec >= 1000)
    {
        millisec -= 1000;
        tv.tv_sec++;
    }
    tm_info = localtime(&tv.tv_sec);
    strftime(buffer, 26, "[%d/%m/%Y %H:%M:%S", tm_info);
    fprintf(stderr, "%s.%03d]", buffer, millisec);
}
struct msg
{
    int producer_id;
    double price_producer;
    double price_mean_producer;
};
struct buffer
{
    int start;
    int end;
};
struct producer
{
    string name;
    int id;
    double price;
    double price_mean;
    double price_deviation;
    int sleep_time_in_ms;
    int shared_buffer_size;
    int messages_sent = 0;
    int state = 1; /*  1: Generating new value 2:trying to get mutex lock 3:putting new value  4: sleeping    */
    vector<double> last_5_prices = {0.00, 0.00, 0.00, 0.00, 0.00};
    double avg_last_5_prices = 0.00;
    msg message;

    void calculate_price()
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        normal_distribution<double> distribution((*this).price_mean, (*this).price_deviation);
        (*this).price = distribution(generator);
        (*this).last_5_prices.erase((*this).last_5_prices.begin());
        (*this).last_5_prices.push_back((*this).price);
    }

    void calculate_average()
    {
        double sum = 0.00;
        double count = 0.00;
        if ((*this).messages_sent == 0)
        {
            (*this).avg_last_5_prices = (*this).price;
            (*this).messages_sent++;
            return;
        }

        for (size_t i = 0; i < (*this).last_5_prices.size(); i++)
        {
            if ((*this).last_5_prices[i] == 0.00)
            {
                continue;
            }
            sum += (*this).last_5_prices[i];
            count++;
        }
        (*this).avg_last_5_prices = sum / count;
    }
    void assign_id()
    {
        if ((*this).name == "ALUMINIUM")
        {
            (*this).id = 0;
        }
        else if ((*this).name == "COPPER")
        {
            (*this).id = 1;
        }
        else if ((*this).name == "COTTON")
        {
            (*this).id = 2;
        }
        else if ((*this).name == "CRUDEOIL")
        {
            (*this).id = 3;
        }
        else if ((*this).name == "GOLD")
        {
            (*this).id = 4;
        }
        else if ((*this).name == "LEAD")
        {
            (*this).id = 5;
        }
        else if ((*this).name == "MENTHAOIL")
        {
            (*this).id = 6;
        }
        else if ((*this).name == "NATURALGAS")
        {
            (*this).id = 7;
        }
        else if ((*this).name == "NICKEL")
        {
            (*this).id = 8;
        }
        else if ((*this).name == "SLIVER")
        {
            (*this).id = 9;
        }
        else if ((*this).name == "ZINC")
        {
            (*this).id = 10;
        }
    }

    void create_msg()
    {
        calculate_price();
        calculate_average();
        (*this).message.producer_id = (*this).id;
        (*this).message.price_producer = (*this).price;
        (*this).message.price_mean_producer = (*this).avg_last_5_prices;
    }

    void produce()
    {
        (*this).state = 1;
        create_msg();
        (*this).log();
    }
    void producer_sleeping()
    {
        (*this).state = 4;
        sleep(float((*this).sleep_time_in_ms) / 1000);
        (*this).log();
    }
    void log()
    {
        if ((*this).state == 1)
        {
            time_stamp();
            fprintf(stderr, " %s: generating a new value %lf\n", (*this).name.c_str(), (*this).price);
        }
        if ((*this).state == 2)
        {
            time_stamp();
            fprintf(stderr, " %s: trying to get mutex on shared buffer\n", (*this).name.c_str());
        }
        if ((*this).state == 3)
        {
            time_stamp();
            fprintf(stderr, " %s: placing %lf on shared buffer\n", (*this).name.c_str(), (*this).price);
        }
        if ((*this).state == 4)
        {
            time_stamp();
            fprintf(stderr, " %s: sleeping for %d ms\n", (*this).name.c_str(), (*this).sleep_time_in_ms);
        }
    }
};

producer create_producer(string name, double price_mean, double price_deviation, int sleep_time_in_ms, int shared_buffer_size)
{
    producer created_producer;
    created_producer.name = name;
    created_producer.assign_id();
    created_producer.price_mean = price_mean;
    created_producer.price_deviation = pow(price_deviation, 2);
    created_producer.sleep_time_in_ms = sleep_time_in_ms;
    created_producer.shared_buffer_size = shared_buffer_size;
    return created_producer;
}

int main(int argc, char *argv[])
{
    producer producer = create_producer(argv[1], stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]));
    int buffer_size = stoi(argv[5]) * sizeof(msg);

    key_t key = ftok(".", 650000);
    int shmid = shmget(key, buffer_size, 0666);
    struct msg *shared_buffer = (struct msg *)shmat(shmid, (void *)0, 0);

    key_t key_start = ftok(".", 20000);
    int shmid_start = shmget(key_start, sizeof(buffer), 0666);
    struct buffer *area_pointer = (struct buffer *)shmat(shmid_start, (void *)0, 0);

    key_t key_semaphore_1 = ftok(".", 10000);
    int empty = semget(key_semaphore_1, 1, 0660); // semaphore to check if the buffer is fully consumed
    key_t key_semaphore_2 = ftok(".", 20000);
    int full = semget(key_semaphore_2, 1, 0660); // semaphore to check if the buffer is full
    key_t key_semaphore_3 = ftok(".", 30000);
    int exclusion = semget(key_semaphore_3, 1, 0660); // semaphore for exclusion

    struct sembuf sem_wait, sem_signal;
    sem_signal.sem_num = sem_wait.sem_num = sem_wait.sem_flg = sem_signal.sem_flg = 0;
    sem_signal.sem_op = 1;
    sem_wait.sem_op = -1;

    while (true)
    {
        producer.produce();
        producer.state = 2;
        producer.log();
        semop(empty, &sem_wait, 1);
        semop(exclusion, &sem_wait, 1);
        shared_buffer[area_pointer->end] = producer.message;
        (area_pointer->end)++;
        producer.state = 3;
        producer.log();
        if (area_pointer->end == producer.shared_buffer_size)
        {
            area_pointer->end = 0;
        }
        semop(full, &sem_signal, 1);
        semop(exclusion, &sem_signal, 1);
        producer.producer_sleeping();
    }
    shmdt(shared_buffer);
    shmdt(area_pointer);
    return 0;
}
