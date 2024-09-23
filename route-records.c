#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "route-records.h"

RouteRecord* createRecords(FILE* fileIn){
    int i;
    int k;
    int month;
    int count = 0;
    int passengers;
    char airline[3];
    char buffer[50];
    RouteRecord* arr;
    char new_Dest[4];
    char category[11];
    char orig_Dest[4];

    fgets(buffer, 50, fileIn);
    while(fgets(buffer, 50, fileIn) != NULL){                                                                                      //counts how many flights paths were taken
        count += 1;
    }

    arr = (RouteRecord*)malloc(sizeof(RouteRecord) * count);                                                                       //allocates memory

    for(i = 0; i < count; i++){                                                                                                    //sets the number of passengers for every month in all struct objects to 0
        for(k = 0; k < 6; k++){
            arr[i].passenger_Cnt[k] = 0;  
        }
    }

    rewind(fileIn);

    return arr;
}

int fillRecords(RouteRecord* r, FILE* fileIn){
    int i;
    int month;
    int FAR_idx;
    int passengers;
    int lineNumber;
    char airline[3];
    char buffer[50];
    char new_Dest[4];
    int totalRecords;
    char orig_Dest[4];
    char category[11];

    lineNumber = 0;
    totalRecords = 0;
    fgets(buffer, 50, fileIn);
    while(fgets(buffer, 50, fileIn) != NULL){                                                                                       //parses the current line's origin, destination, airline, and passenger count
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%[^,],%d", &month, orig_Dest, new_Dest, airline, category, &passengers);    
        FAR_idx = findAirlineRoute(r, totalRecords, orig_Dest, new_Dest, airline, lineNumber-1);

        if(FAR_idx == -1){                                                                                   //checks if it is our first line or if findAirlineRoute returned -1, if so then it adds this line to the total records and goes to the next line
            strcpy(r[totalRecords].origin_Code, orig_Dest);
            strcpy(r[totalRecords].dest_Code, new_Dest);
            strcpy(r[totalRecords].airln_Code, airline);
            for(i = 1; i <= 6; i++){
                if(i == month){
                    r[totalRecords].passenger_Cnt[i-1] += passengers;                                                               //iterates through the r objects passenger_cnt elements and adds the amount of passengers in the element that is equal to the month for that record
                }
            }
            lineNumber += 1;
            totalRecords += 1;
        }

        else{                                                                                                                       //if neither previous case is true, it then means the current line is a duplicate, and this line's passenger amount is added to the same month index of the original line.  
            r[FAR_idx].passenger_Cnt[month-1] = passengers;                                                                         //it then goes to the next line without adding a duplicate record
            lineNumber += 1;
        }
    }
    return totalRecords;
}

int findAirlineRoute(RouteRecord* r, int length, const char* origin, const char* destination, const char* airline, int curIdx){      //checks if the origin, destination, and airline of a single object is identical to a previous line and returns the original line's index
    if(curIdx == -1){
        return -1;                                                                                                                   //returns -1 if it doesn't find a duplicate
    }
    else{
        if((strcmp(origin, r[curIdx].origin_Code) == 0) && (strcmp(destination, r[curIdx].dest_Code) == 0) && (strcmp(airline, r[curIdx].airln_Code) == 0)){
            return curIdx;                                                                                                           //returns the index of the original line if the curr line is a duplicate
        }
        else{
            return findAirlineRoute(r, length, origin, destination, airline, curIdx-1);
        }
    }
}

void searchRecords(RouteRecord* r, int length, const char* key1, const char* key2, SearchType st){
    int i;
    int n;
    int count = 0;
    int totalPassengers = 0;
    int passengerCount[] = {0, 0, 0, 0, 0, 0};

    switch (st)
    {
    case 0:                                                                                                                            //checks all routes that correspond with the user's inputs and returns the stats
        for(i = 0; i < length; i++){
            if((strcmp(r[i].origin_Code, key1) == 0) && (strcmp(r[i].dest_Code, key2) == 0)){
                printf("%s (%s-%s) ", r[i].airln_Code, r[i].origin_Code, r[i].dest_Code);
                for(n = 0; n < 6; n++){
                    passengerCount[n] += r[i].passenger_Cnt[n];
                }
                count += 1;
            }
        }
        printf("\n%d matches were found\n\n", count);

        break;
    case 1:                                                                                                                            //checks all origins that correspond with the user's inputs and returns the stats
        for(i = 0; i < length; i++){
            if(strcmp(r[i].origin_Code, key1) == 0){
                printf("%s (%s-%s) ", r[i].airln_Code, r[i].origin_Code, r[i].dest_Code);
                for(n = 0; n < 6; n++){
                    passengerCount[n] += r[i].passenger_Cnt[n];
                }
                count += 1;
            }
        }
        printf("\n%d matches were found\n\n", count);

        break;
    case 2:                                                                                                                            //checks all destinations that correspond with the user's inputs and returns the stats
        for(i = 0; i < length; i++){
            if(strcmp(r[i].dest_Code, key1) == 0){
                printf("%s (%s-%s) ", r[i].airln_Code, r[i].origin_Code, r[i].dest_Code);
                for(n = 0; n < 6; n++){
                    passengerCount[n] += r[i].passenger_Cnt[n];
                }
                count += 1;
            }
        }
        printf("\n%d matches were found\n\n", count);

        break;
    case 3:                                                                                                                            //checks all airlines that correspond with the user's inputs and returns the stats
        for(i = 0; i < length; i++){
            if(strcmp(r[i].airln_Code, key1) == 0){
                printf("%s (%s-%s) ", r[i].airln_Code, r[i].origin_Code, r[i].dest_Code);
                for(n = 0; n < 6; n++){
                    passengerCount[n] += r[i].passenger_Cnt[n];
                }
                count += 1;
            }
        }
        printf("\n%d matches were found\n\n", count);

        break;
    }

    printf("Statistics\n");

    for(i = 0; i < 6; i++){                                                                                                            //iterates through passengercount's elements and adds them together
        totalPassengers += passengerCount[i];
    }
    printf("Total Passengers:\t%d\n", totalPassengers);

    for(i = 1; i <= 6; i++){                                                                                                           //iterates through passengercount and prints out the number of passengers for each month
        printf("Total Passengers in Month %d:\t%d\n", i, passengerCount[i-1]);
    }

    printf("\nAverage Passengers per Month:\t%d\n", totalPassengers/6);
}

void printMenu(){
    printf( "\n\n######### Airline Route Records Database MENU #########\n" );
    printf( "1. Search by Route\n" );
    printf( "2. Search by Origin Airport\n" );
    printf( "3. Search by Destination Airport\n" );
    printf( "4. Search by Airline\n" );
    printf( "5. Quit\n" );
    printf( "Enter your selection: " );
}