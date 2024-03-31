#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <queue>
#include<fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

mutex mutex1; // Mutex for synchronizing access to shared resources

// Initializing number of threads for producer and consumer
int p_num_threads = 2;
int c_num_threads = 2;

int hour_ind = 48; // Value to check if an hour has passed (48 rows for an hour)

int ccount = 0; // Producer counter
int con_count = 0; // Consumer counter
int m = 0; // Number of rows

condition_variable producer_cv, consumer_cv; // Condition variables for producer and consumer synchronization

// String variables and vectors to store data from the file
string ind, t_stamp, tr_light_id, no_of_cars;
vector<int> in;
vector<int> tr_light;
vector<int> no_cars;
vector<string> tstamp;

// Struct for traffic data row
struct tr_signal
{
    int ind;
    std::string t_stamp;
    int tr_id;
    int num_cars;
};

// Array to hold the totals of each of the 4 traffic lights
tr_signal tlSorter[4] = {{0, "", 1, 0}, {0, "", 2, 0}, {0, "", 3, 0}, {0, "", 4, 0}};

// Queue to store traffic light data
queue<tr_signal> tr_sig_queue;
tr_signal sig;

// Function to sort traffic light data
bool sort_method(struct tr_signal first, struct tr_signal second)
{
    if (first.num_cars > second.num_cars)
        return 1;
    return 0;
}

// Producer function
void* produce(void* args)
{
    while (ccount < m)
    {
        unique_lock<mutex> lk(mutex1); // Locking until producer finishes processing

        if (ccount < m) // If count is less than the number of rows in the dataset
        {
            tr_sig_queue.push(tr_signal{in[ccount], tstamp[ccount], tr_light[ccount], no_cars[ccount]}); // Push into the queue
            consumer_cv.notify_all(); // Notify consumer threads
            ccount++;
        }
        else
        {
            producer_cv.wait(lk, []{ return ccount < m; }); // If count is greater than the number of rows in the dataset, wait
        }

        lk.unlock(); // Unlock after processing
        sleep(rand() % 3); // Sleep for random time
    }
    return nullptr;
}

// Consumer function
void* consume(void* args)
{
    while (con_count < m)
    {
        unique_lock<mutex> lk(mutex1); // Lock until processing

        if (!tr_sig_queue.empty())
        {
            sig = tr_sig_queue.front(); // Get the front element of the queue

            // Add the number of cars into the respective traffic light ID
            if (sig.tr_id == 1)
            {
                tlSorter[0].num_cars += sig.num_cars;
            }
            if (sig.tr_id == 2)
            {
                tlSorter[1].num_cars += sig.num_cars;
            }
            if (sig.tr_id == 3)
            {
                tlSorter[2].num_cars += sig.num_cars;
            }
            if (sig.tr_id == 4)
            {
                tlSorter[3].num_cars += sig.num_cars;
            }
            tr_sig_queue.pop(); // Pop the data
            producer_cv.notify_all(); // Notify producer
            con_count++;
        }
        else
        {
            consumer_cv.wait(lk, []{ return !tr_sig_queue.empty(); }); // If queue is empty, wait until producer produces
        }

        // Check if an hour has passed by (checking every 48th row)
        if (con_count % hour_ind == 0)
        {
            // Sort traffic lights data
            sort(tlSorter, tlSorter + 4, sort_method);
            printf("Traffic lights sorted according to most busy | Time: %s \n", sig.t_stamp.c_str());
            cout << "Traffic Light" << "\t" << "Number of Cars" << endl;
            cout << tlSorter[0].tr_id << "\t\t\t" << "\t" << tlSorter[0].num_cars << endl;
            cout << tlSorter[1].tr_id << "\t\t\t" << "\t" << tlSorter[1].num_cars << endl;
            cout << tlSorter[2].tr_id << "\t\t\t" << "\t" << tlSorter[2].num_cars << endl;
            cout << tlSorter[3].tr_id << "\t\t\t" << "\t" << tlSorter[3].num_cars << endl;
        }
        lk.unlock(); // Unlock after processing
        sleep(rand() % 3); // Sleep for random time
    }
    return nullptr;
}

// Function to get data from file
void get_traff_data()
{
    ifstream infile;

    string file;
    cout << "Using file.txt ....";
    file = "textfile.txt";

    infile.open(file);

    if (infile.is_open())
    {
        std::string line;
        getline(infile, line);

        // Read data from file
        while (!infile.eof())
        {
            getline(infile, ind, ',');
            in.push_back(stoi(ind));
            getline(infile, t_stamp, ',');
            tstamp.push_back(t_stamp);
            getline(infile, tr_light_id, ',');
            tr_light.push_back(stoi(tr_light_id));
            getline(infile, no_of_cars, '\n');
            no_cars.push_back(stoi(no_of_cars));

            m += 1; // Increment row count
        }
        infile.close();
    }
    else
    {
        printf("Could not open file, try again.");
    }
}

int main()
{
    // Get traffic data from the file
    get_traff_data();

    // Create producer and consumer threads
    pthread_t producers[p_num_threads];
    pthread_t consumers[c_num_threads];

    for (long i = 0; i < p_num_threads; i++)
        pthread_create(&producers[i], NULL, produce, (void*) i);

    for (long i = 0; i < c_num_threads; i++)
        pthread_create(&consumers[i], NULL, consume, (void*) i);

    // Wait for all producer threads to finish
    for (long i = 0; i < p_num_threads; i++)
        pthread_join(producers[i], NULL);

    // Wait for all consumer threads to finish
    for (long i = 0; i < c_num_threads; i++)
        pthread_join(consumers[i], NULL);

    return 0;
}
