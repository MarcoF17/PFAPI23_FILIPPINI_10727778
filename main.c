#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char stopNumber;
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

    SSPointer currentStation = pointerVector[0];
    while(newStation->distance > currentStation->distance){
        if(newStation->distance == currentStation->distance){
            printf("non aggiunta\n");
            free(newStation);
            return;
        }
        currentStation = currentStation->nextStation;
        if(currentStation == NULL)
            break;
    }

    if(currentStation != NULL){
        newStation->nextStation = currentStation->nextStation;
        if(newStation->nextStation == NULL)
            pointerVector[POINTER_VECTOR_DIM-1] = newStation;
        else
            newStation->nextStation->prevStation = newStation;

        currentStation->nextStation = newStation;
        newStation->prevStation = currentStation;
        if(newStation->prevStation == NULL)
            pointerVector[0] = newStation;
    }
    else{
        newStation->prevStation = pointerVector[POINTER_VECTOR_DIM-1];
        newStation->nextStation = NULL;
        newStation->prevStation->nextStation = newStation;
        pointerVector[POINTER_VECTOR_DIM-1] = newStation;
    }
}

///returns the reference to the station with the given distance
SSPointer search_for_station(int distance){
    SSPointer currentStation = pointerVector[0];

    while(currentStation != NULL){
        if(currentStation->distance == distance)
            return currentStation;
        currentStation = currentStation->nextStation;
    }

    return NULL;
}

///in-order insertion of a new car in a station
int car_create_inorder_insert(SSPointer station, int battery) {
    if(station == NULL)
        return 0;       //0 stands for "non aggiunta"

    Car *newCar = malloc(sizeof(Car));
    newCar->battery = battery;
    newCar->nextCar = NULL;
    Car *currentCar = station->carList;
    Car *currentCarPrev = NULL;

    if(currentCar == NULL)
        station->carList = newCar;

    else {
        while (newCar->battery < currentCar->battery) {
            currentCarPrev = currentCar;
            currentCar = currentCar->nextCar;
            if(currentCar == NULL)
                break;
        }

        newCar->nextCar = currentCar;
        if(currentCarPrev != NULL)
            currentCarPrev->nextCar = newCar;
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
    tmp->stopNumber = -1;
    station_inorder_insert(tmp);

    if(carsNumber > 0){
        //adding cars list to newStation
        int i, battery;
        for(i = 0; i < carsNumber; i++){
            scanfTmp = scanf("%d", &battery);
            car_create_inorder_insert(tmp, battery);
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

    Car *currentCar = pointer->carList, *nextCar;
    while(currentCar != NULL){
        nextCar = currentCar->nextCar;
        free(currentCar);
        currentCar = nextCar;
    }

    printf("demolita\n");
    stationNumber--;
    if(stationNumber == 0){
        pointerVector[0] = pointerVector[POINTER_VECTOR_DIM-1] = NULL;
    }
}

///scraps the car with the given battery capacity at the given station (if it exists)
void scrap_car(int distance, int battery){
    SSPointer pointer = search_for_station(distance);
    if(pointer == NULL){
        printf("non rottamata\n");
        return;
    }

    Car *currentCar = pointer->carList, *prevCar = NULL;
    while(currentCar != NULL){
        if(currentCar->battery == battery){
            if(prevCar != NULL)
                prevCar->nextCar = currentCar->nextCar;
            else
                pointer->carList = currentCar->nextCar;

            free(currentCar);
            printf("rottamata\n");
            return;
        }
        currentCar = currentCar->nextCar;
    }

    printf("non rottamata\n");
}

char* convert_int_to_char(int toBeConverted){
    char res[] = "";
    while(toBeConverted > 0){
        int remainder = toBeConverted % 10;
        toBeConverted /= 10;
        switch (remainder) {
            case 0:
                strcat(res, "0");
                break;
            case 1:
                strcat(res, "1");
                break;
            case 2:
                strcat(res, "2");
                break;
            case 3:
                strcat(res, "3");
                break;
            case 4:
                strcat(res, "4");
                break;
            case 5:
                strcat(res, "5");
                break;
            case 6:
                strcat(res, "6");
                break;
            case 7:
                strcat(res, "7");
                break;
            case 8:
                strcat(res, "8");
                break;
            case 9:
                strcat(res, "9");
                break;
        }
    }

    char finalResult[] = "";
    int i;
    for(i = 0; i < strlen(res); i++){
        finalResult[i] = res[strlen(res) - i -1];
    }

    return finalResult;
}

///plans a route (if it exists) between start and finish stations (start < finish)
void plan_route_forwards(int start, int finish){
    const SSPointer startingStation = search_for_station(start), arrivingStation = search_for_station(finish);

    //if there are no cars at the station
    if(startingStation->carList == NULL){
        printf("nessun percorso\n");
        return;
    }

    //no stop during the trip (start -> finish)
    if(finish - start <= startingStation->carList->battery){
        printf("%d %d\n", start, finish);
        return;
    }

    //at least one stop is required
    SSPointer nextPossibleStop = startingStation->nextStation, previousStop = startingStation, maxReachableStation = NULL;  //nextPossibleStop: all possible next-stop starting from previousStop; previousStop: starting station for the new iteration fo the algorithm; maximumReachableStation: maximum reachable station from previousStop (not necessary equal to maxReachableDistance)
    char result[] = "";
    int maxReachableDistance = 0, minimumCurrentStop = 1, maxBattery = 0;   //maxReachableDistance: maximum reachable distance from previousStop; minimumCurrentStop: minimum stop to try to reach destination

    strcat(result, convert_int_to_char(start));
    while(1){
        if(previousStop->carList !=NULL){
            maxReachableDistance = previousStop->distance + previousStop->carList->battery;

            while(nextPossibleStop->distance <= maxReachableDistance){   //slides through the cars in the window (starting from previousStop->nextStation)
                if(nextPossibleStop->carList != NULL) {  //if there are no cars at the currentStation, it needs to be skipped
                    if (nextPossibleStop->carList->battery + nextPossibleStop->distance >= finish) {
                        //route found
                        return;
                    }

                    if(nextPossibleStop->carList->battery > maxBattery)
                        maxBattery = nextPossibleStop->carList->battery;
                }

                maxReachableStation = nextPossibleStop;

                if(nextPossibleStop->nextStation != NULL)
                    if(nextPossibleStop->nextStation->distance > maxReachableDistance)
                        break;

                //eliminazione di tutte le stazioni precedenti a maxBattery
            }
        }
    }
}

///plans a route (if it exists) between start and finish stations (start > finish)
void plan_route_backwards(int start, int finish){
    SSPointer startingStation, arrivingStation, currentStation;
    startingStation = currentStation = search_for_station(start);
    arrivingStation = search_for_station(finish);

    //if there are no cars at the station
    if(startingStation->carList == NULL){
        printf("nessun percorso\n");
        return;
    }

    //no stop during the trip (start -> finish)
    if(start - finish <= search_for_station(start)->carList->battery){
        printf("%d %d\n", start, finish);
        return;
    }

}

///just for debug
void print_cars_batteries(SSPointer station){
    Car *currentCar = station->carList;
    while(currentCar != NULL){
        printf("%d\n", currentCar->battery);
        currentCar = currentCar->nextCar;
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
                int res = car_create_inorder_insert(search_for_station(distance), battery);
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
            while(commandType != ' '){
                scanfTmp = scanf("%c", &commandType);
            }

            int s1, s2;
            scanfTmp = scanf("%d %d", &s1, &s2);

            if(s1 < s2)
                plan_route_forwards(s1, s2);
            else if(s1 > s2)
                plan_route_backwards(s1, s2);
        }
    }
}
