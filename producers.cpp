//James Lee (RedId:820655947)
#include "producers.h"
#include <unistd.h>
#include <semaphore.h>
#include "structs.h"
using namespace std;

//Constructor for the producer
ProducerClass::ProducerClass(Belt* belt, int speed){
    this->onBelt = belt;
    this->producerSpeed = speed;
    this->totalAmountProduced = 0;
    this->currentCounter = 0;
}

//Deconstructor
ProducerClass::~ProducerClass(){}