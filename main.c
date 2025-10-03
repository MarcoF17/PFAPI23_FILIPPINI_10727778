#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Car{
    int battery;        // Maximum distance a car can travel
    struct Car* nextCar;
}Car;

typedef struct StationNode{
    int distance;          // Absolute distance from the beginning of the road
    struct Car* carList;
    struct StationNode* leftChild;
    struct StationNode* rightChild;
    struct StationNode* parent;
}StationNode;

typedef struct StationNode* NodePointer;

int scanfTmp = 0;           // Used to collect the return value of each scanf invocation
int *resultVector = NULL, resultVectorDimension = 0;
NodePointer root = NULL;

///returns the node with the minimum key of a tree given by its root
NodePointer tree_minimum(NodePointer node){
    NodePointer res = node;
    while(res -> leftChild != NULL){
        res = res ->leftChild;
    }

    return res;
}

///returns the node with the maximum key of a tree given by its root
NodePointer tree_maximum(NodePointer node){
    NodePointer res = node;
    while(res -> rightChild != NULL){
        res = res -> rightChild;
    }

    return res;
}

///return the following node of a given node
NodePointer tree_successor(NodePointer node){
    NodePointer res = node;

    if(node -> rightChild != NULL)
        return tree_minimum(node -> rightChild);

    else {
        res = node -> parent;
        while(res != NULL && node == res -> rightChild){
            node = res;
            res = res -> parent;
        }

        return res;
    }
}

///return the previous node of a given node
NodePointer tree_predecessor(NodePointer node){
    NodePointer res = node;

    if(node -> leftChild != NULL)
        return tree_maximum(node -> leftChild);

    else {
        res = node -> parent;
        while(res != NULL && node == res -> leftChild){
            node = res;
            res = res -> parent;
        }

        return res;
    }
}

///return the node with the given key if it exists, returns NULL otherwise
NodePointer tree_search(int distanceToFind){
    NodePointer res = root;
    while(res != NULL && res -> distance != distanceToFind){
        if(distanceToFind < res ->distance)
            res = res -> leftChild;
        else
            res = res -> rightChild;
    }

    return res;
}

///method that put a new node in the right place in the tree
void station_tree_insert(NodePointer newStation){
    NodePointer insertionPoint = root, tmpPointer = NULL;
    while(insertionPoint != NULL){
        tmpPointer = insertionPoint;
        if(newStation -> distance < insertionPoint -> distance)
            insertionPoint = insertionPoint -> leftChild;
        else
            insertionPoint = insertionPoint -> rightChild;
    }

    newStation -> parent = tmpPointer;
    if(newStation -> parent == NULL)
        root = newStation;
    else if(newStation -> distance < tmpPointer -> distance)
        tmpPointer -> leftChild = newStation;
    else
        tmpPointer -> rightChild = newStation;
}

///method that remove from the tree the node with the given key (if it exists)
void tree_delete(int distanceToDelete){
    NodePointer nodeToDelete = tree_search(distanceToDelete);
    if(nodeToDelete == NULL){
        printf("non demolita\n");
        return;
    }

    else{
        NodePointer nodeToBeDeleted = NULL, tmpPointer = NULL;
        if(nodeToDelete -> leftChild == NULL || nodeToDelete -> rightChild == NULL)
            nodeToBeDeleted = nodeToDelete;
        else
            nodeToBeDeleted = tree_successor(nodeToDelete);

        if(nodeToBeDeleted -> leftChild != NULL)
            tmpPointer = nodeToBeDeleted -> leftChild;
        else
            tmpPointer = nodeToBeDeleted -> rightChild;

        if(tmpPointer != NULL)
            tmpPointer -> parent = nodeToBeDeleted -> parent;
        if(nodeToBeDeleted -> parent == NULL)
            root = tmpPointer;
        else if(nodeToBeDeleted == nodeToBeDeleted -> parent -> leftChild)
            nodeToBeDeleted -> parent -> leftChild = tmpPointer;
        else nodeToBeDeleted -> parent -> rightChild = tmpPointer;

        if(nodeToBeDeleted != nodeToDelete){
            nodeToDelete -> distance = nodeToBeDeleted -> distance;
            nodeToDelete -> carList = nodeToBeDeleted -> carList;
        }

        free(nodeToBeDeleted);
        printf("demolita\n");
    }
}

///creation and in-order insertion of a new car in a station
int car_create_inorder_insert(NodePointer station, int battery) {
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

///memory allocation for a new station
void create_station(int distance, int carsNumber){
    if(tree_search(distance) != NULL){
        printf("non aggiunta\n");
        return;
    }

    NodePointer tmp;
    tmp = malloc(sizeof(StationNode));
    tmp->distance = distance;
    tmp->carList = NULL;
    tmp->leftChild = tmp->rightChild = tmp->parent = NULL;
    station_tree_insert(tmp);

    if(carsNumber > 0){
        //adding cars list to newStation
        int i, battery;
        for(i = 0; i < carsNumber; i++){
            scanfTmp = scanf("%d", &battery);
            car_create_inorder_insert(tmp, battery);
        }
    }

    printf("aggiunta\n");
}

///scraps the car with the given battery capacity at the given station (if it exists)
void scrap_car(int distance, int battery){
    NodePointer pointer = tree_search(distance);
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

///prints backwards-planned route (finish is the last station to print, backwards is the sense to print
void print_result_vector(int finish, int backwards){
    int i;

    if(!backwards){
        for(i = 0; i < resultVectorDimension; i++){
            printf("%d ", resultVector[i]);
        }
        printf("%d\n", finish);
    }
    else {
        printf("%d ", finish);

        for(i = resultVectorDimension - 1; i > 0; i--){
            printf("%d ", resultVector[i]);
        }
        printf("%d\n", resultVector[0]);
    }
}

///plans a route (if it exists) between start and finish stations (start < finish)
void plan_route_forwards(int start, int finish){
    const NodePointer startingStation = tree_search(start);

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
    resultVector = malloc(sizeof(int));
    resultVector[resultVectorDimension] = finish;
    resultVectorDimension++;

    const NodePointer arrivingStation = tree_search(finish);
    NodePointer currentArrivingStation = arrivingStation, selectedStationForNextIteration = tree_predecessor(arrivingStation), currentStationDuringIteration = tree_predecessor(arrivingStation);
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


            currentStationDuringIteration = tree_predecessor(currentStationDuringIteration);
            if(currentStationDuringIteration == NULL)
                break;
        }

        currentArrivingStation = selectedStationForNextIteration;
        currentStationDuringIteration = tree_predecessor(selectedStationForNextIteration);
        if(selectedStationForNextIteration->distance < startingStation->distance){
            break;
        }
        else if(selectedStationForNextIteration == startingStation && check){
            done = 1;
            break;
        }
        else if(!check)
            break;
        else if(currentArrivingStation-> distance != start){
            resultVector = realloc(resultVector, (resultVectorDimension + 1) * sizeof(int));
            resultVector[resultVectorDimension] = currentArrivingStation->distance;
            resultVectorDimension++;
        }
    }

    if(done == 0){
        printf("nessun percorso\n");
        //stopList_cleanup();
    }
    else
        //print_route(start, finish);
        print_result_vector(start, 1);
}

///calculates minReachableStation from the given startingStation
NodePointer calculate_minReachableStation(NodePointer startingStation, int finish){ //needs to be fixed
    if(startingStation->carList == NULL)
        return startingStation;

    int minReachableDistanceFromStartingStation = startingStation->distance - startingStation->carList->battery;
    //looking for minReachableStation
    NodePointer minReachableStation = startingStation;
    while(1){
        if(minReachableStation->distance >= minReachableDistanceFromStartingStation){
            if(minReachableStation == tree_minimum(root))
                return tree_minimum(root);
            if(tree_predecessor(minReachableStation) != NULL)
                minReachableStation = tree_predecessor(minReachableStation);
            else if(minReachableStation->distance == finish)
                return tree_successor(minReachableStation);
            //else return minReachableStation;
        }
        else break;
    }

    return tree_successor(minReachableStation);
}

///if the previous stop chosen is not the minimum one based on next stop
void new_route_fixup(int finish){
    NodePointer oneOfThreeStation, twoOfThreeStation, threeOfThreeStation;
    if(resultVectorDimension >= 2) {
        oneOfThreeStation = tree_search(resultVector[resultVectorDimension - 2]);
        twoOfThreeStation = tree_search(resultVector[resultVectorDimension - 1]);
        threeOfThreeStation = tree_search(finish);

        NodePointer currentIterationStation = tree_predecessor(twoOfThreeStation);
        NodePointer minReachableStationFromOneOfThreeStation = calculate_minReachableStation(oneOfThreeStation, finish);
        NodePointer selectedStation = NULL;
        int i = 0;
        while(1){
            currentIterationStation = tree_predecessor(twoOfThreeStation);
            selectedStation = NULL;
            while(1){
                if(currentIterationStation == threeOfThreeStation)
                    break;
                if(currentIterationStation->distance < minReachableStationFromOneOfThreeStation->distance)
                    break;

                if(currentIterationStation->carList != NULL)
                    if(currentIterationStation->distance - currentIterationStation->carList->battery <= threeOfThreeStation->distance)
                        selectedStation = currentIterationStation;

                currentIterationStation = tree_predecessor(currentIterationStation);
            }

            if(selectedStation != NULL)
                if(selectedStation != threeOfThreeStation){
                    resultVector[resultVectorDimension-1-i] = selectedStation->distance;
                    twoOfThreeStation = selectedStation;

                }

            threeOfThreeStation = twoOfThreeStation;
            twoOfThreeStation = oneOfThreeStation;
            if(twoOfThreeStation->distance == resultVector[0])
                break;
            if(resultVectorDimension-2-++i < 0)
                break;
            oneOfThreeStation = tree_search(resultVector[resultVectorDimension - 2 - i]);
            minReachableStationFromOneOfThreeStation = calculate_minReachableStation(oneOfThreeStation, finish);
        }
    }
}

///plans a route (if it exists) between start and finish stations (start > finish)
void plan_route_backwards(int start, int finish){
    const NodePointer startingStation = tree_search(start);  //startingStation in the station at distance == start

    if(startingStation->carList == NULL)
        printf("nessun percorso\n");
    else{
        //looking for minReachableStationFromStartingStation
        NodePointer minReachableStationFromStartingStation = calculate_minReachableStation(startingStation, finish);
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
        NodePointer currentIterationStation = tree_predecessor(startingStation), possibleNextStopStation = minReachableStationFromStartingStation, nextMinReachableStation = NULL;
        int check;
        int done = 0;

        while(1){
            check = 0;
            NodePointer minReachableStationFromMinReachableStation = calculate_minReachableStation(minReachableStationFromStartingStation, finish);

            while(currentIterationStation->distance >= minReachableStationFromStartingStation->distance){
                nextMinReachableStation = calculate_minReachableStation(currentIterationStation, finish);
                if(nextMinReachableStation == currentIterationStation){
                    currentIterationStation = tree_predecessor(currentIterationStation);
                    continue;
                }

                currentIterationStation = tree_predecessor(currentIterationStation);

                if(nextMinReachableStation->distance <= finish){
                    check = 2;
                    possibleNextStopStation = tree_successor(currentIterationStation);
                    if(done == 0){
                        if(resultVector[resultVectorDimension-1] != possibleNextStopStation->distance){
                            resultVector = realloc(resultVector, (resultVectorDimension + 1) * sizeof(int));
                            resultVector[resultVectorDimension] = possibleNextStopStation->distance;
                            resultVectorDimension++;
                            done = 1;
                        }
                    }
                    else{
                        if(resultVector[resultVectorDimension-1] != possibleNextStopStation->distance)
                            resultVector[resultVectorDimension-1] = possibleNextStopStation->distance;
                    }

                }
                else if(currentIterationStation->distance <= finish){
                    check = 2;
                    break;
                }
                else if(nextMinReachableStation->distance <= minReachableStationFromMinReachableStation->distance && check != 2/* || nextMinReachableStation->distance <= finish*/){
                    possibleNextStopStation = tree_successor(currentIterationStation);
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
                return;
            }


            if(check == 2){
                break;
            }

            //possibleNextStopStation is the nextStop
            if(resultVector[resultVectorDimension-1] != possibleNextStopStation->distance){
                resultVector = realloc(resultVector, (resultVectorDimension + 1) * sizeof(int));
                resultVector[resultVectorDimension] = possibleNextStopStation->distance;
                resultVectorDimension++;
            }

            if(possibleNextStopStation->distance <= finish){
                break;
            }

            minReachableStationFromStartingStation = calculate_minReachableStation(possibleNextStopStation, finish);
            if(minReachableStationFromStartingStation == possibleNextStopStation){
                printf("nessun percorso\n");
                return;
            }
            nextMinReachableStation = NULL;
        }

        if(resultVectorDimension >= 2){
            new_route_fixup(finish);
        }
        print_result_vector(finish, 0);
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
                int res = car_create_inorder_insert(tree_search(distance), battery);
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
            tree_delete(distance);
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

            if(resultVector != NULL) {
                free(resultVector);
                resultVector = NULL;
            }
            resultVectorDimension = 0;
        }
    }
}