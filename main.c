#include <stdio.h>
#include <stdlib.h>
#include <values.h>

#define POINTER_VECTOR_DIM 100

typedef struct Car{
    int battery;
    struct Car* nextCar;
}Car;

typedef struct ServiceStation{
    int distance;
    struct Car* carList;
    struct ServiceStation* nextStation;
    struct ServiceStation* prevStation;
}ServiceStation;

typedef struct ServiceStation* SSPointer;

int scanfTmp = 0;

//typedef char String[];

SSPointer pointerVector[POINTER_VECTOR_DIM];   //vector to optimize access to stations

///initialize pointer-vector to NULL
void init_pointer_vector(SSPointer pv[]){
    int i;
    for(i = 0; i < POINTER_VECTOR_DIM; i++)
        pointerVector[i] = NULL;
}

///in-order insertion of a new station
void station_inorder_insert(SSPointer newStation){
    if(pointerVector[0] == NULL && pointerVector[POINTER_VECTOR_DIM-1] == NULL){
        pointerVector[0] = pointerVector[POINTER_VECTOR_DIM-1] = newStation;
        newStation->nextStation = newStation->prevStation = NULL;
        return;
    }

    SSPointer actualStation = pointerVector[0];
    while(newStation->distance > actualStation->distance){
        actualStation = actualStation->nextStation;
    }

    newStation->nextStation = actualStation->nextStation;
    if(newStation->nextStation == NULL)
        pointerVector[POINTER_VECTOR_DIM-1] = newStation;
    actualStation->nextStation = newStation;
    newStation->prevStation = actualStation;
    if(newStation->prevStation == NULL)
        pointerVector[0] = newStation;
    newStation->nextStation->prevStation = newStation;
}

///in-order insertion of a new car in a station
void insert_inorder_car(SSPointer station, int battery) {
    Car *newCar = malloc(sizeof(Car));
    newCar->battery = battery;
    Car *actualCar = station->carList;
    Car *actualCarPrev = NULL;
    while (newCar->battery < actualCar->battery) {
        actualCarPrev = actualCar;
        actualCar = actualCar->nextCar;
    }

    if(actualCarPrev == NULL)
        station->carList = newCar;
    else
        actualCarPrev->nextCar = newCar;

    newCar->nextCar = actualCar;
}

///memory allocation for a new station and possibly new cars
void create_station(int distance, int carsNumber){
    SSPointer tmp;
    tmp = malloc(sizeof(ServiceStation));
    tmp->distance = distance;
    station_inorder_insert(tmp);

    if(carsNumber == 0)
        return;

    int i;
    for(i = 0; i < carsNumber; i++){

    }
}

int main() {
    while(1){
        char commandType;
        scanfTmp = scanf("%c", &commandType);
        if(scanfTmp == EOF)
            return 0;

        if(commandType == 'a'){ //aggiungi-auto || aggiungi-stazione
            while(commandType != '-')
                scanfTmp = scanf("%c", &commandType);
            scanfTmp = scanf("%c", &commandType);

            if(commandType == 'a'){     //aggiungi-auto

            }

            else if(commandType == 's'){    //aggiungi-stazione

            }
        }

        else if(commandType == 'd'){    //demolisci-stazione

        }

        else if(commandType == 'r'){    //rottama-auto

        }

        else if(commandType == 'p'){    //pianifica-percorso

        }
    }
}
