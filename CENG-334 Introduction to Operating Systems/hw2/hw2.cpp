#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <time.h>
#include <sys/time.h>
extern "C" {
#include "hw2_output.h"
}

// grid
std::vector<std::vector < int>> cigButtsGrid;

// semaphores
std::vector<std::vector < sem_t>> cigButtsGridLocks;
sem_t bLock;
sem_t bPrev;

// orders
std::vector<int> orderTimes;
std::vector<std::string > orders;

// class to store proper private info
class properPrivateClass
{
public:
    int gid;
    int si;
    int sj;
    int time;
    int ng;
    std::vector<std::vector < int>> topCorners;
};

// class to store smoker private info
class smokerPrivateClass
{
public:
    int sid;
    int smokeTime;
    int cellCount;
    std::vector<int> smokerCenter;
};
// PART - 1
// collect function
void *collectCigButtsPart_1(void *arguments)
{
    // take private info from arguments
    class properPrivateClass myPrivate;
    myPrivate.gid = ((class properPrivateClass *) arguments)->gid;
    myPrivate.time = ((class properPrivateClass *) arguments)->time;
    myPrivate.si = ((class properPrivateClass *) arguments)->si;
    myPrivate.sj = ((class properPrivateClass *) arguments)->sj;
    myPrivate.ng = ((class properPrivateClass *) arguments)->ng;
    myPrivate.topCorners = ((class properPrivateClass *) arguments)->topCorners;

    // PRIVATE CREATED
    hw2_notify(PROPER_PRIVATE_CREATED, ((class properPrivateClass *) arguments)->gid, 0, 0);

    // calculate sleepTime in miliseconds
    int sleepTime = myPrivate.time * 1000;

    // traverse all ng's of private
    for (int num = 0; num < myPrivate.ng; num++)
    {
        sem_wait(&bPrev);
        // lock the area
        for (int i = 0; i < myPrivate.si; i++)
        {
            for (int j = 0; j < myPrivate.sj; j++)
            {
                int iCoordinate = myPrivate.topCorners[num][0] + i;
                int jCoordinate = myPrivate.topCorners[num][1] + j;
                sem_wait(&cigButtsGridLocks[iCoordinate][jCoordinate]);
            }
        }
        sem_post(&bPrev);

        // PRIVATE ARRIVED TO  AREA
        hw2_notify(PROPER_PRIVATE_ARRIVED, myPrivate.gid, myPrivate.topCorners[num][0], myPrivate.topCorners[num][1]);

        for (int i = 0; i < myPrivate.si; i++)
        {
            for (int j = 0; j < myPrivate.sj; j++)
            {
                int iCoordinate = myPrivate.topCorners[num][0] + i;
                int jCoordinate = myPrivate.topCorners[num][1] + j;
                while (cigButtsGrid[iCoordinate][jCoordinate] > 0)
                {
                    // sleep in miliseconds before starting
                    usleep(sleepTime);
                    // Lock before changing global variable
                    sem_wait(&bLock);
                    // clear one cig butt
                    cigButtsGrid[iCoordinate][jCoordinate]--;
                    // PRIVATE GATHERED
                    hw2_notify(PROPER_PRIVATE_GATHERED, myPrivate.gid, iCoordinate, jCoordinate);
                    sem_post(&bLock);
                }
            }
        }

        for (int i = 0; i < myPrivate.sj; i++)
        {
            for (int j = 0; j < myPrivate.si; j++)
            {
                int iCoordinate = myPrivate.topCorners[num][0] + j;
                int jCoordinate = myPrivate.topCorners[num][1] + i;
                sem_post(&cigButtsGridLocks[iCoordinate][jCoordinate]);
            }
        }
        // PRIVATE CLEARED
        hw2_notify(PROPER_PRIVATE_CLEARED, myPrivate.gid, 0, 0);
    }
    // PRIVATE EXITED
    hw2_notify(PROPER_PRIVATE_EXITED, myPrivate.gid, 0, 0);

    return 0;
}

int main()
{
    // HANDLE 1st part INPUT
    int val1, val2, val3;
    int xDimension, yDimension;
    int numPrivates;

    std::cin >> xDimension;
    std::cin >> yDimension;

    cigButtsGridLocks.resize(xDimension);

    for (int i = 0; i < xDimension; i++)
    {
        // vector initializing
        cigButtsGridLocks[i].resize(yDimension);
        cigButtsGrid.push_back(std::vector<int> ());

        for (int j = 0; j < yDimension; j++)
        {
            std::cin >> val1;
            cigButtsGrid[i].push_back(val1);
            sem_init(&cigButtsGridLocks[i][j], 0, 1);
        }
    }

    sem_init(&bLock, 0, 1);
    sem_init(&bPrev, 0, 1);

    std::cin >> numPrivates;

    properPrivateClass privates[numPrivates];
    pthread_t privateThread[numPrivates];

    for (int i = 0; i < numPrivates; i++)
    {
        std::cin >> privates[i].gid;
        std::cin >> privates[i].si;
        std::cin >> privates[i].sj;
        std::cin >> privates[i].time;
        std::cin >> privates[i].ng;
        for (int j = 0; j < privates[i].ng; j++)
        {
            // vector initializing
            privates[i].topCorners.push_back(std::vector<int> ());
            std::cin >> val1;
            std::cin >> val2;
            privates[i].topCorners[j].push_back(val1);
            privates[i].topCorners[j].push_back(val2);
        }
    }

    // TAKE 2ND PART INPUT

    std::string line;

    int numOrder = 0;
    
    if (std::cin >> numOrder) {
        orderTimes.resize(numOrder);
        orders.resize(numOrder);
        for (int i = 0; i < numOrder; i++) {
            std::cin >> orderTimes[i];
            std::cin >> orders[i];
        }
    }

    // TAKE 3RD PART INPUT

    int numSmoker = 0;
    
    if (std::cin >> numSmoker) {
        smokerPrivateClass smokers[numSmoker];
        for (int i = 0; i< numSmoker; i++) {
            std::cin >> smokers[i].sid;
            std::cin >> smokers[i].smokeTime;
            std::cin >> smokers[i].cellCount;

            smokers[i].smokerCenter.resize(smokers[i].cellCount*3);

            for (int j = 0; j < smokers[i].cellCount; j++) {
                for (int k = 0; k<3; k++) {
                    int val;
                    std::cin >> val;
                    smokers[i].smokerCenter.push_back(val);
                }
            }
        }
    }

    // hw2_init_notifier called at the beginning
    hw2_init_notifier();

    // FIRST PART
    if (numOrder == 0 && numSmoker == 0) {
        // CREATE PRIVATE THREADS
        for (int i = 0; i < numPrivates; i++)
        {
            pthread_create(&privateThread[i], NULL, collectCigButtsPart_1, (void*) &privates[i]);
        }

        // WAIT FOR OTHER THREADS TO JOIN
        for (int i = 0; i < numPrivates; i++)
        {
            pthread_join(privateThread[i], NULL);
        }
        // DESTROY SEMAPHORES
        sem_destroy(&bLock);
        sem_destroy(&bPrev);

        for (int i = 0; i < xDimension; i++)
        {
            for (int j = 0; j < yDimension; j++)
            {
                sem_destroy(&cigButtsGridLocks[i][j]);
            }
        }
    }

    // 2nd and 3rd part inputs have taken but couldn't implement the necessary tasks.
    // Condition variables must be used for those parts.


    return 0;
}