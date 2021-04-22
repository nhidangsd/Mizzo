#include "consumers.h"
//James Lee (RedId:820655947)
#include <unistd.h>
#include <semaphore.h>
#include "structs.h"
#include <string.h>
using namespace std;

ConsumerClass::ConsumerClass(Belt* belt, ConsumerType consumerName, int speed){
    this->onBelt = belt;
    this->consumerName = consumerName;
    this->speed = speed;
    
    this->frogsEaten = 0;
    this->escargotEaten = 0;
    this->totalEaten = 0;
}

ConsumerClass::~ConsumerClass(){}