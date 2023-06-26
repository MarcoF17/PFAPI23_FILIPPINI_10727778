#include <stdio.h>
#include <stdlib.h>

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

int scanfTmp = 0, stationNumber = 0;
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
        if(newStation->distance == actualStation->distance){
            printf("non aggiunta\n");
            free(newStation);
            return;
        }
        actualStation = actualStation->nextStation;
        if(actualStation == NULL)
            break;
    }

    if(actualStation != NULL){
        newStation->nextStation = actualStation->nextStation;
        if(newStation->nextStation == NULL)
            pointerVector[POINTER_VECTOR_DIM-1] = newStation;
        else
            newStation->nextStation->prevStation = newStation;

        actualStation->nextStation = newStation;
        newStation->prevStation = actualStation;
        if(newStation->prevStation == NULL)
            pointerVector[0] = newStation;
    }
    else{
        newStation->prevStation = pointerVector[POINTER_VECTOR_DIM-1];
        newStation->nextStation = NULL;
        newStation->prevStation->nextStation = newStation;
        pointerVector[POINTER_VECTOR_DIM-1] = newStation;
    }

    //printf("aggiunta\n");
}

///returns the reference to the station with
SSPointer search_for_station(int distance){
    SSPointer actualStation = pointerVector[0];

    while(actualStation != NULL){
        if(actualStation->distance == distance)
            return actualStation;
        actualStation = actualStation->nextStation;
    }

    return NULL;
}

///in-order insertion of a new car in a station
int create_insert_inorder_car(SSPointer station, int battery) {
    if(station == NULL)
        return 0;       //0 stands for "non aggiunta"

    Car *newCar = malloc(sizeof(Car));
    newCar->battery = battery;
    newCar->nextCar = NULL;
    Car *actualCar = station->carList;
    Car *actualCarPrev = NULL;

    if(actualCar == NULL)
        station->carList = newCar;

    else {
        while (newCar->battery < actualCar->battery) {
            actualCarPrev = actualCar;
            actualCar = actualCar->nextCar;
            if(actualCar == NULL)
                break;
        }

        newCar->nextCar = actualCar;
        if(actualCarPrev != NULL)
            actualCarPrev->nextCar = newCar;
        else
            station->carList = newCar;
    }

    return 1;       //1 stands for "aggiunta"
}

///memory allocation for a new station and possibly new cars
void create_station(int distance, int carsNumber){
    if(search_for_station(distance) != NULL){
        printf("non aggiunta\n");
        return;
    }

    SSPointer tmp;
    tmp = malloc(sizeof(ServiceStation));
    tmp->distance = distance;
    tmp->carList = NULL;
    station_inorder_insert(tmp);

    if(carsNumber > 0){
        //adding cars list to newStation
        int i, battery;
        for(i = 0; i < carsNumber; i++){
            scanfTmp = scanf("%d", &battery);
            create_insert_inorder_car(tmp, battery);
        }
    }

    printf("aggiunta\n");
    stationNumber++;
}

///deletes a station which is at the given distance (if it exists)
void demolish_station(int distance){
    SSPointer pointer = search_for_station(distance);
    if(pointer == NULL){
        printf("non demolita\n");
        return;
    }

    if(pointer->prevStation != NULL)
        pointer->prevStation->nextStation = pointer->nextStation;
    if(pointer->nextStation != NULL)
        pointer->nextStation->prevStation = pointer->prevStation;

    Car *actualCar = pointer->carList, *nextCar;
    while(actualCar != NULL){
        nextCar = actualCar->nextCar;
        free(actualCar);
        actualCar = nextCar;
    }

    printf("demolita\n");
    stationNumber--;
    if(stationNumber == 0){
        pointerVector[0] = pointerVector[POINTER_VECTOR_DIM-1] = NULL;
    }
}

///scraps the car with the given battery capacity (if it exists)
void scrap_car(int distance, int battery){
    SSPointer pointer = search_for_station(distance);
    if(pointer == NULL){
        printf("non rottamata\n");
        return;
    }

    Car *actualCar = pointer->carList, *prevCar = NULL;
    while(actualCar != NULL){
        if(actualCar->battery == battery){
            if(prevCar != NULL)
                prevCar->nextCar = actualCar->nextCar;
            else
                pointer->carList = actualCar->nextCar;

            free(actualCar);
            printf("rottamata\n");
            return;
        }
        actualCar = actualCar->nextCar;
    }

    printf("non rottamata\n");
}

///just for debug
void print_cars_batteries(SSPointer station){
    Car *actualCar = station->carList;
    while(actualCar != NULL){
        printf("%d\n", actualCar->battery);
        actualCar = actualCar->nextCar;
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
                while(commandType != ' ')
                    scanfTmp = scanf("%c", &commandType);
                int distance, battery;
                scanfTmp = scanf("%d %d", &distance, &battery);
                int res = create_insert_inorder_car(search_for_station(distance), battery);
                if(res == 0)
                    printf("non aggiunta\n");
                else
                    printf("aggiunta\n");
            }

            else if(commandType == 's'){    //aggiungi-stazione
                while(commandType != ' ')
                    scanfTmp = scanf("%c", &commandType);

                int distance, carsNumber;
                scanfTmp = scanf("%d %d", &distance, &carsNumber);
                create_station(distance, carsNumber);
                //print_cars_batteries(search_for_station(distance));
            }
        }

        else if(commandType == 'd'){    //demolisci-stazione
            while(commandType != ' ')
                scanfTmp = scanf("%c", &commandType);
            int distance;
            scanfTmp = scanf("%d", &distance);
            demolish_station(distance);
        }

        else if(commandType == 'r'){    //rottama-auto
            while(commandType != ' ')
                scanfTmp = scanf("%c", &commandType);

            int distance, battery;
            scanfTmp = scanf("%d %d", &distance, &battery);
            scrap_car(distance, battery);
        }

        else if(commandType == 'p'){    //pianifica-percorso

        }
    }
}
