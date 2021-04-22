//James Lee (RedId:820655947)
#ifndef CONSUMERS_H
#define CONSUMERS_H

#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "structs.h"
#include <string>
#include "production.h"

class ConsumerClass{
    public:
        ConsumerType consumerName;
        Belt* onBelt;

        //Int Variables
        int speed, frogsEaten, escargotEaten, totalEaten;
        ConsumerClass(Belt* belt, ConsumerType consumerName, int speed);
        ~ConsumerClass();
};

#endif