#ifndef BELT_H
#define BELT_H

//James Lee (RedId:820655947)
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include "structs.h"
#include "production.h"
using namespace std;


class Belt{
    public:
        std::queue<int> *currentBelt;

        //Semaphore Variables
        sem_t itemOnBelt, slotsAvailable, frogMax, mutex, limit;

        //Int variables
        int frogAmount, escargotAmount, totalProduce, totalConsume;

        Belt();
        ~Belt();

        int pop();
        bool push (int candy);
        
};



#endif