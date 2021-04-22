//James Lee (RedId:820655947)
#include <stdio.h>
#include <queue>
#include "belt.h"
#include "structs.h"
using namespace std;


Belt::Belt(){
    currentBelt = new std::queue<int>();

    sem_init(&(this->limit),0,0);
    sem_init(&(this->mutex),0,1);
    sem_init(&(this->frogMax),0,0);
    sem_init(&(this->slotsAvailable),0,10);
    sem_init(&(this->itemOnBelt),0,0);

    frogAmount = 0;
    escargotAmount = 0;
    totalConsume = 0;
    totalProduce = 0;
}

Belt::~Belt(){
    sem_destroy(&(this->limit));
    sem_destroy(&(this->mutex));
    sem_destroy(&(this->frogMax));
    sem_destroy(&(this->slotsAvailable));
    sem_destroy(&(this->itemOnBelt));
}

int Belt::pop(){
    int first;

    first = currentBelt->front();
    currentBelt->pop();
    totalConsume++;

    return first;
}

bool Belt::push(int candy){
    //Check the 10 candy limit on the belt
    if((this->currentBelt->size())>10){
        return false;
    }

    //If it is less than 10 candies proceed to pushing it onto the belt.
    currentBelt->push(candy);
    totalProduce++;

    return true;
}