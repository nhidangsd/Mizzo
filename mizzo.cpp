//James Lee (RedId:820655947)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <string>

#include "producers.h"
#include "consumers.h"
#include "belt.h"
#include "structs.h"
#include "production.h"


using namespace std;

void* produceFrogs(void* Ptr);
void* produceEscargot(void* Ptr);
void* consume(void* Ptr);

int main(int argc, char **argv) 
{
    //Initialize the int variables we will be using throughout this code.
    int frogSpeed = 0;
    int frogFlag = 0;
    int escargotSpeed = 0;
    int escargotFlag = 0;
    int ethelSpeed = 0;
    int ethelFlag = 0;
    int lucySpeed = 0;
    int lucyFlag = 0;

    //Initialize the pthreads we will be using.
    pthread_t frog;
    pthread_t escargot;
    pthread_t lucy;
    pthread_t ethel;

    int flag = 0;

    while((flag = getopt(argc, argv, "f:e:E:L:")) != -1){
        switch(flag){
            case 'f':
                frogSpeed = atoi(optarg);
                frogFlag = 1;
                printf("%d", frogSpeed);
                break;
            case 'e':
                escargotSpeed = atoi(optarg);
                escargotFlag = 1;
                printf("%d", escargotSpeed);
                break;
            case 'E':
                ethelSpeed = atoi(optarg);
                ethelFlag = 1;
                printf("%d", ethelSpeed);
                break;
            case 'L':
                lucySpeed = atoi(optarg);
                lucyFlag = 1;
                printf("%d", lucySpeed);
                break;
            default:
                printf("Error occurs");
                exit(1);
        }
    }

    //This creates our belt that we will be using throughout the code.
    Belt *currentBelt = new Belt();

    //We now create the consumers, and following #4 on the assignment, Lucy is made first.
    ConsumerClass* LucyConsumer = new ConsumerClass(currentBelt, Lucy, lucySpeed);
    ConsumerClass* EthelConsumer = new ConsumerClass(currentBelt, Ethel, ethelSpeed);

    //We now create the Producer
    ProducerClass* frogProduce = new ProducerClass(currentBelt, frogSpeed);
    ProducerClass* escargotProduce = new ProducerClass(currentBelt, escargotSpeed);

    //Create Pthreads for consumers
    pthread_create(&lucy, NULL, consume, LucyConsumer);
    pthread_create(&ethel, NULL, consume, EthelConsumer);

    //Create Pthreads for Candy 
    pthread_create(&frog, NULL, produceEscargot, (void *) frogProduce);
    pthread_create(&escargot, NULL, produceEscargot, (void *) escargotProduce);

    //Join all the threads
    pthread_join(lucy, NULL);
    pthread_join(ethel, NULL);
    pthread_join(frog, NULL);
    pthread_join(escargot, NULL);

    sem_wait(&(currentBelt->limit));

    //Production report
    int totalProduced[] = {frogProduce->totalAmountProduced, escargotProduce->totalAmountProduced};
    int* consumedFrogs = new int(2);
    int* consumedEscargots = new int(2);
    *consumedFrogs = LucyConsumer->frogsEaten;
    *(consumedFrogs+ 1) = EthelConsumer->frogsEaten;
    *consumedEscargots = LucyConsumer->escargotEaten;
    *(consumedEscargots + 1) = EthelConsumer->escargotEaten;
    int* totalConsumed[] = {consumedFrogs, consumedEscargots};
    io_production_report(totalProduced, totalConsumed);
}
 
void* produceFrogs(void* Ptr){
    ProducerClass* producedFrogs = (ProducerClass*) Ptr;

    while(1){
        //This will allow us to enter the critical region.
        sem_wait(&(producedFrogs->onBelt->slotsAvailable));
        sem_wait(&(producedFrogs->onBelt->mutex));

        //Check to see if the max of 100 was reached, and will break if it has.
        if(producedFrogs->totalAmountProduced == MAX_CANDY){
            sem_post(&(producedFrogs->onBelt->mutex));
            sem_post(&(producedFrogs->onBelt->itemOnBelt));
            break;
        }

        //If we have space on the belt still, we will go through this code to add more candy.
        if(producedFrogs->onBelt->push(FrogBite)){
            producedFrogs->onBelt->frogAmount++;
            producedFrogs->totalAmountProduced++;
            producedFrogs->currentCounter = producedFrogs->onBelt->frogAmount + producedFrogs->onBelt->escargotAmount;

            //Setting up the onbelt and such in order to use the io add type.
            int onBelt[] = {producedFrogs->onBelt->frogAmount, producedFrogs->onBelt->escargotAmount};
            int produced[] = {producedFrogs->totalAmountProduced, producedFrogs->totalAmountProduced};
            ProductType frogCandy = FrogBite;
            io_add_type(frogCandy, onBelt, produced);

            //Checks to see if 100 candies were created second checker after adding a candy.
            if(producedFrogs->onBelt->totalProduce == MAX_CANDY){
                sem_post(&(producedFrogs->onBelt->mutex));
                sem_post(&(producedFrogs->onBelt->itemOnBelt));

                break;
            }

            //Checks to see if there are currently 3 frogs on the belt after adding a candy.
            if(producedFrogs->onBelt->frogAmount == 3){
                sem_post(&(producedFrogs->onBelt->mutex));
                sem_post(&(producedFrogs->onBelt->itemOnBelt));
                sem_wait(&(producedFrogs->onBelt->frogMax));
            }

            //Otherwise do this.
            else{
                sem_post(&(producedFrogs->onBelt->mutex));
                sem_post(&(producedFrogs->onBelt->itemOnBelt));
            }

            usleep((producedFrogs->producerSpeed) * 1000);
        }

    }
    return nullptr;
}

void* produceEscargot(void* Ptr){
    ProducerClass * producedEscargot = (ProducerClass*) Ptr;

    while(1){
        //Check to see if the max of 100 was reached, and will break if it has.
        if(producedEscargot->totalAmountProduced == MAX_CANDY){

            break;
        }
        //This will allow us to enter the critical region.

        usleep((producedEscargot->producerSpeed) * 1000);

        //Check to see if the max of 100 was reached, and will break if it has.
        if(producedEscargot->totalAmountProduced == MAX_CANDY){

            break;
        }
        sem_wait(&(producedEscargot->onBelt->slotsAvailable));
        sem_wait(&(producedEscargot->onBelt->mutex));

        //If we have space on the belt still, we will go through this code to add more candy.
        if(producedEscargot->onBelt->push(FrogBite)){
            producedEscargot->onBelt->frogAmount++;
            producedEscargot->totalAmountProduced++;
            producedEscargot->currentCounter = producedEscargot->onBelt->frogAmount + producedEscargot->onBelt->escargotAmount;

            //Setting up the onbelt and such in order to use the io add type.
            int onBelt[] = {producedEscargot->onBelt->frogAmount, producedEscargot->onBelt->escargotAmount};
            int produced[] = {producedEscargot->totalAmountProduced, producedEscargot->totalAmountProduced};
            ProductType escargotCandy = Escargot;
            io_add_type(escargotCandy, onBelt, produced);
        }
        //Checks to see if 100 candies were created second checker after adding a candy.
        if(producedEscargot->onBelt->totalProduce == MAX_CANDY){
            sem_post(&(producedEscargot->onBelt->mutex));
            sem_post(&(producedEscargot->onBelt->itemOnBelt));

            break;
        }
            //Otherwise do this.
        else{
            sem_post(&(producedEscargot->onBelt->mutex));
            sem_post(&(producedEscargot->onBelt->itemOnBelt));
        }

    }
    return nullptr;
}


//Consume function
void* consume(void* Ptr){
    ConsumerClass *consumeCandy = (ConsumerClass *) Ptr; 

    ConsumerType name = consumeCandy->consumerName; 

    while(1) {
        // This starting if is to ensure other thread leaves.
        if(consumeCandy->onBelt->totalConsume == MAX_CANDY) {
            break;
        }
        usleep((consumeCandy->speed) * 1000);  
        // Stops the consumption if Consumption Task has been finished by other thread 
        if(consumeCandy->onBelt->totalConsume == MAX_CANDY) {
            break;
        }
        sem_wait(&(consumeCandy->onBelt->itemOnBelt)); 
        sem_wait(&(consumeCandy->onBelt->mutex)); 

        //If you can consume a candy, then it will start consuming.
        
        int candyConsumed = consumeCandy->onBelt->pop(); 

        //This is for when the candy is an Escargot.
        if(candyConsumed == Escargot){
            consumeCandy->escargotEaten++;
            consumeCandy->onBelt->escargotAmount--;
            int currentTotal = consumeCandy->onBelt->escargotAmount + consumeCandy->onBelt->frogAmount;
            int onBelt[] = {consumeCandy->onBelt->frogAmount, consumeCandy->onBelt->escargotAmount};
            int consumed[] = {consumeCandy->totalEaten, consumeCandy->totalEaten};
            ProductType escargotCandy = Escargot;
            io_remove_type(name, escargotCandy, onBelt, consumed);
        }

            //This is for when the candy is a frog.
        if(candyConsumed == FrogBite){ 
            consumeCandy->frogsEaten++;
            consumeCandy->onBelt->frogAmount--;
            int currentTotal = consumeCandy->onBelt->escargotAmount + consumeCandy->onBelt->frogAmount;

            //If there isn't 3 anymore, it will create more candy.
            if(consumeCandy->onBelt->frogAmount == 2){
                sem_post(&(consumeCandy->onBelt->frogMax)); 
            }
        }
            
        if(consumeCandy->onBelt->totalConsume== MAX_CANDY) { 
            sem_post(&(consumeCandy->onBelt->mutex)); 
            sem_post(&(consumeCandy->onBelt->limit)); 
        }

            //We will let one go then tell the semaphore that there is a spot available.
        else {
            sem_post(&(consumeCandy->onBelt->mutex));
            sem_post(&(consumeCandy->onBelt->slotsAvailable)); 
        }

    }
    return NULL;
}