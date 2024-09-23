#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "route-records.h"

int main(int argc, char *argv[]){
    int userNum;
    SearchType st;
    RouteRecord* r;
    int totalRecords;
    char userStr[30];
    char userOrg[50];
    char userDest[50];
    char userAirln[50];
    FILE* fileIn = NULL;

    if(argc >= 2){                                      //checks if user inputs at least 2 arguments and if the second argument is the correct file name
        if(strcmp(argv[1], "data.csv") != 0){
            printf("ERROR: Missing file name :(");
            return 1;
        }
    }
    else{
        printf("ERROR: Not enough arguments :(");
        return 1;
    }

    printf("Opening data.csv...\n");

    fileIn = fopen(argv[1], "r");                       //checks if the file has data in it 
    if(fileIn == NULL){
        printf("ERROR: Could not open the file :(");
        return 1;
    }

    r = createRecords(fileIn);                          //creates the array of RouteRecord’s and initializes it

    totalRecords = fillRecords(r, fileIn);              //searches through the data file and gets the data for all unique routes

    printf("Unique routes operated by airlines: %d\n", totalRecords);
    
    userNum = 1;
    while(userNum != 0){
    printMenu();

    scanf("%s", userStr);
    userNum = atoi(userStr);

        while(userNum < 1 || userNum > 5){              //gets a string from the user, converts it to an int, and checks if it is <1 or >5 and loops if true
            printf("Please enter a number from the menu\n", userNum);
            printMenu();
            scanf("%s", userStr);
            userNum = atoi(userStr);
        }

        switch (userNum)
        {
        case 1:                                         //gets an origin and destination code from the user, capitalizes them, and sends them through search records to find all corresp. routes
            printf("Enter origin: ");
            scanf("%s", &userOrg);
            strupr(userOrg);
            printf("Enter destination: ");
            scanf("%s", &userDest);
            strupr(userDest);
            printf("Searching by route...\n");
            st = ROUTE;
            searchRecords(r, totalRecords, userOrg, userDest, st);
            break;
        case 2:                                         //gets an origin code from the user, capitalizes it, and sends it through search records to find all corresp. origins
            printf("Enter origin: ");
            scanf("%s", &userOrg);
            strupr(userOrg);
            printf("Searching by origin...\n");
            st = ORIGIN;
            searchRecords(r, totalRecords, userOrg, userDest, st);
            break;
        case 3:                                         //gets a destinations code from the user, capitalizes it, and sends it through search records to find all corresp. destinations
            printf("Enter destination: ");
            scanf("%s", &userDest);
            strupr(userDest);
            printf("Searching by destination...\n");
            st = DESTINATION;
            searchRecords(r, totalRecords, userDest, userOrg, st);
            break;
        case 4:                                         //gets an airline code from the user, capitalizes it, and sends it through search records to find all corresp. airlines
            printf("Enter airline: ");
            scanf("%s", &userAirln);
            strupr(userAirln);
            printf("Searching by airline...\n");
            st = AIRLINE;
            searchRecords(r, totalRecords, userAirln, userOrg, st);
            break;
        case 5:                                         //quits out of the loop and ends the program
        default:
        userNum = 0;
        printf("Goodbye\n");
            break;
        }
    }

    fclose(fileIn);

    free(r);

    return 0;
}
