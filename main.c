#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POINTER_VECTOR_DIM 2

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

typedef struct StopDuringRoute{
    int distance;
    struct StopDuringRoute* prevStop;
    struct StopDuringRoute* nextStop;
}StopDuringRoute;

typedef struct StopDuringRoute* StopPointer;

int scanfTmp = 0, stationNumber = 0;
SSPointer pointerVector[POINTER_VECTOR_DIM];   //vector to optimize access to stations
StopPointer finalStopPointer = NULL, initialStopPointer = NULL;
int *resultVector = NULL, resultVectorDimension = 0;

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
        newStation->nextStation = currentStation;
        newStation->prevStation = currentStation->prevStation;

        if(newStation->nextStation == NULL)
            pointerVector[POINTER_VECTOR_DIM-1] = newStation;
        else
            newStation->nextStation->prevStation = newStation;

        if(newStation->prevStation == NULL)
            pointerVector[0] = newStation;
        else
            newStation->prevStation->nextStation = newStation;
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
    else
        pointerVector[0] = pointer->nextStation;

    if(pointer->nextStation != NULL)
        pointer->nextStation->prevStation = pointer->prevStation;
    else
        pointerVector[POINTER_VECTOR_DIM - 1] = pointer->prevStation;

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
        prevCar = currentCar;
        currentCar = currentCar->nextCar;
    }

    printf("non rottamata\n");
}

char *convert_int_to_char(int toBeConverted){
    char *res = malloc(10);
    int counter = 0;
    while(toBeConverted > 0){
        counter++;
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

    char *finalResult = (char*) malloc(counter+1);
    int i;
    for(i = 0; i < counter; i++){
        finalResult[i] = res[counter - i -1];
    }

    return finalResult;
}

///insertion of stop in a separate list
void stop_insert(SSPointer station){
    StopDuringRoute *newStop = malloc(sizeof(StopDuringRoute));
    newStop->distance = station->distance;

    if(finalStopPointer == NULL && initialStopPointer == NULL) {
        finalStopPointer = initialStopPointer = newStop;
        newStop->prevStop = newStop->nextStop = NULL;
    }
    else{
        newStop->prevStop = NULL;
        newStop->nextStop = initialStopPointer;
        if(initialStopPointer->nextStop != NULL)
            initialStopPointer->nextStop->prevStop = newStop;
        initialStopPointer = newStop;
    }
}

///prints the route planned
void print_route(int start, int finish){
    printf("%d ", start);
    StopPointer currentIterationStop = initialStopPointer, prevStop = NULL;
    while(currentIterationStop != NULL){
        printf("%d ", currentIterationStop->distance);
        prevStop = currentIterationStop;
        currentIterationStop = currentIterationStop->nextStop;
        free(prevStop);
    }
    printf("%d\n", finish);
    initialStopPointer = finalStopPointer = NULL;
}

///cleans-up the list with stop-stations after route is planned
void stopList_cleanup(){
    StopPointer prev, curr = initialStopPointer;
    while(curr != NULL){
        prev = curr;
        curr = curr->nextStop;
        free(prev);
    }

    initialStopPointer = finalStopPointer = NULL;
}

///plans a route (if it exists) between start and finish stations (start < finish)
void plan_route_forwards(int start, int finish){
    const SSPointer startingStation = search_for_station(start);

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
    const SSPointer arrivingStation = search_for_station(finish);
    SSPointer currentArrivingStation = arrivingStation, selectedStationForNextIteration = arrivingStation->prevStation, currentStationDuringIteration = arrivingStation->prevStation;
    int done = 0, check = 0;
    while(1){
        check = 0;
        while(1){
            if(currentStationDuringIteration->carList == NULL)
                continue;
            else{
                if(currentStationDuringIteration->carList->battery + currentStationDuringIteration->distance >= currentArrivingStation->distance){
                    selectedStationForNextIteration = currentStationDuringIteration;
                    check = 1;
                }
            }

            if(currentStationDuringIteration == startingStation)
                break;


            currentStationDuringIteration = currentStationDuringIteration->prevStation;
            if(currentStationDuringIteration == NULL)
                break;
        }

        currentArrivingStation = selectedStationForNextIteration;
        currentStationDuringIteration = selectedStationForNextIteration->prevStation;
        if(selectedStationForNextIteration->distance < startingStation->distance){
            break;
        }
        else if(selectedStationForNextIteration == startingStation && check){
            done = 1;
            break;
        }
        else if(!check)
            break;
        else{
            stop_insert(currentArrivingStation);
        }
    }

    if(done == 0){
        printf("nessun percorso\n");
        stopList_cleanup();
    }
    else
        print_route(start, finish);
}

///calculates minReachableStation from the given startingStation
SSPointer calculate_minReachableStation(SSPointer startingStation, int finish){ //needs to be fixed
    if(startingStation->carList == NULL)
        return startingStation;

    int minReachableDistanceFromStartingStation = startingStation->distance - startingStation->carList->battery;
    //looking for minReachableStation
    SSPointer minReachableStation = startingStation;
    while(1){
        if(minReachableStation->distance >= minReachableDistanceFromStartingStation){
            if(minReachableStation == pointerVector[0])
                return pointerVector[0];
            if(minReachableStation->prevStation != NULL)
                minReachableStation = minReachableStation->prevStation;
            else if(minReachableStation->distance == finish)
                return minReachableStation->nextStation;
            //else return minReachableStation;
        }
        else break;
    }

    return minReachableStation->nextStation;
}

///prints backwards-planned route
void print_result_vector(int finish){
    int i;
    for(i = 0; i < resultVectorDimension; i++){
        printf("%d ", resultVector[i]);
    }
    printf("%d\n", finish);
}

///if the previous stop chosen is not the minimum one based on next stop
void route_fixup(SSPointer prevPrevStop, SSPointer prevStop, SSPointer newStop) {
    //looking for the minimum previous-stop to reach new-stop (prev-stop needs to be reachable from prevPrev-stop)
    SSPointer minReachableStationFromPrevPrev = calculate_minReachableStation(prevPrevStop, -1);
    if (minReachableStationFromPrevPrev == prevStop) {
        //all ok
        return;
    }
    else {
        SSPointer startingPoint = prevStop->prevStation, currentIterationPoint = startingPoint;
        while (1) {
            if (currentIterationPoint->carList != NULL) {
                if (currentIterationPoint->distance - currentIterationPoint->carList->battery <= newStop->distance) {
                    if(prevPrevStop->carList != NULL){
                        if(prevPrevStop->distance - prevPrevStop->carList->battery <= currentIterationPoint->distance){
                            resultVector[resultVectorDimension - 2] = currentIterationPoint->distance;
                        }
                    }
                }
            }

            if (currentIterationPoint == minReachableStationFromPrevPrev)
                return;

            currentIterationPoint = currentIterationPoint->prevStation;
            if (currentIterationPoint->prevStation == NULL)
                return;
        }
    }
}

///plans a route (if it exists) between start and finish stations (start > finish)
void plan_route_backwards(int start, int finish){    //0 means no route found, 1 means route found
    //ci vuole un ciclo che contenga tutto probabilmente
    const SSPointer startingStation = search_for_station(start);  //startingStation in the station at distance == start

    if(startingStation->carList == NULL)
        printf("nessun percorso\n");
    else{
        //looking for minReachableStationFromStartingStation
        SSPointer minReachableStationFromStartingStation = calculate_minReachableStation(startingStation, finish);
        if(minReachableStationFromStartingStation == NULL){
            printf("error\n");
            return;
        }

        else if(minReachableStationFromStartingStation->distance == finish){
            printf("%d %d\n", start, finish);
            return;
        }

        resultVector = malloc(sizeof(int));
        resultVector[resultVectorDimension] = start;
        resultVectorDimension++;

        //iteration on station between startingStation and minReachableStationFromStartingStation to select nextStopStation
        SSPointer currentIterationStation = startingStation->prevStation, possibleNextStopStation = minReachableStationFromStartingStation, nextMinReachableStation = NULL;
        SSPointer prevIterationStation = NULL, prevPrevIterationStation = NULL;
        int check;
        int done = 0;
        //assert currentIterrationStation != NULL
        while(1){
            check = 0;
            SSPointer minReachableStationFromMinReachableStation = calculate_minReachableStation(minReachableStationFromStartingStation, finish);

            while(currentIterationStation->distance >= minReachableStationFromStartingStation->distance){
                nextMinReachableStation = calculate_minReachableStation(currentIterationStation, finish);
                if(nextMinReachableStation == currentIterationStation){
                    currentIterationStation = currentIterationStation->prevStation;
                    continue;
                }

                //printf("current: %d; minReachableFromCurrent: %d\n", currentIterationStation->distance, nextMinReachableStation->distance);

                currentIterationStation = currentIterationStation->prevStation;

                if(nextMinReachableStation->distance <= finish){
                    check = 2;
                    possibleNextStopStation = currentIterationStation->nextStation;
                    if(done == 0){
                        if(resultVector[resultVectorDimension-1] != possibleNextStopStation->distance){
                            resultVector = realloc(resultVector, (resultVectorDimension + 1) * sizeof(int));
                            resultVector[resultVectorDimension] = possibleNextStopStation->distance;
                            resultVectorDimension++;
                            //printf("in the if: %d\n", possibleNextStopStation->distance);
                            done = 1;
                        }
                    }
                    else{
                        if(resultVector[resultVectorDimension-1] != possibleNextStopStation->distance)
                            resultVector[resultVectorDimension-1] = possibleNextStopStation->distance;
                        //printf("changed: %d\n", possibleNextStopStation->distance);
                    }

                }
                else if(currentIterationStation->distance <= finish){
                    check = 2;
                    break;
                }
                else if(nextMinReachableStation->distance <= minReachableStationFromMinReachableStation->distance && check != 2/* || nextMinReachableStation->distance <= finish*/){
                    possibleNextStopStation = currentIterationStation->nextStation;
                    minReachableStationFromMinReachableStation = nextMinReachableStation;
                    check = 1;
                }

                if(currentIterationStation == NULL){
                    printf("error\n");
                    return;
                }

            }

            if(check == 0){
                printf("nessun percorso\n");
                //free(resultVector);
                return;
            }

            //printf("----------------------------------------\n");

            if(check == 2){
                print_result_vector(finish);
                return;
            }

            //possibleNextStopStation is the nextStop
            if(resultVector[resultVectorDimension-1] != possibleNextStopStation->distance){
                resultVector = realloc(resultVector, (resultVectorDimension + 1) * sizeof(int));
                resultVector[resultVectorDimension] = possibleNextStopStation->distance;
                resultVectorDimension++;
            }

            //printf("added: %d\n", possibleNextStopStation->distance);

            if(possibleNextStopStation->distance <= finish){
                print_result_vector(finish);
                break;
            }

            if(prevPrevIterationStation != NULL)
                route_fixup(prevPrevIterationStation, prevIterationStation, possibleNextStopStation);

            prevPrevIterationStation = prevIterationStation;
            prevIterationStation = possibleNextStopStation;

            minReachableStationFromStartingStation = calculate_minReachableStation(possibleNextStopStation, finish);
            if(minReachableStationFromStartingStation == possibleNextStopStation){
                printf("nessun percorso\n");
                return;
            }
            nextMinReachableStation = NULL;
        }
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
            else if(s1 > s2){
                plan_route_backwards(s1, s2);
                //print_result_vector(s1);
                if(resultVector != NULL)
                    free(resultVector);
                resultVectorDimension = 0;
            }
        }
    }
}
