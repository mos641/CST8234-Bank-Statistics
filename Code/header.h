/******************************************************************************************
*Assignment Name:    Transaction Processing Program using Random Access Files
*Course:             C Language CST8234
*Source Files:       main.c header.h
******************************************************************************************/

#ifndef header_h
#define header_h

/* needed libraries */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* account structure*/
struct clientInfo
{
    int AccNo;
    char lastName[20];
    char firstName[20];
    double accBalance;
};

/* function declarations */
void setup();
int printMenu();
void textFile();
void updateRecord();
void newRecord();
void deleteRecord();
int validateAcc(char *input);
void validateName(char *input);
double validateBal(char *input);
int findAcc(int accNo, struct clientInfo *client);
void replaceRecord(int accNo, struct clientInfo client);

#endif
