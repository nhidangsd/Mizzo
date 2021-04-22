//James Lee (RedId:820655947)
#ifndef PRODUCERS_H
#define PRODUCERS_H

#include "structs.h"
#include "belt.h"
#include "pthread.h"
#include "io.h"
#include "consumers.h"
#include "production.h"
#include <stdio.h>
#include <semaphore.h>
using namespace std;

class ProducerClass{
    public:
        int totalAmountProduced;
        int producerSpeed;
        int currentCounter;
        Belt* onBelt;
        ProducerClass(Belt* onBelt, int speed);
        ~ProducerClass();

};

#endif