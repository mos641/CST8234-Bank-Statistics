/******************************************************************************************
*Filename:           main.c
*Version:            1
*Course:             C Language CST8234
*Assignment Name:    Transaction Processing Program using Random Access Files
*Purpose:            Maintains bank account statistics for 100 accounts
******************************************************************************************/

#include "header.h"

/******************************************************************************************
*Function name:              main
*Purpose:                    Main function for the employee database program calls other functions and loops through menu
*Function In parameters:     None
*Function Out parameters:    An integer for function success
*Version:                    1
*Author:                     mos
******************************************************************************************/
int main(void)
{
    int menuChoice = -1;

    /* set values of text file and array */
    setup();

    /* loop through case structure */
    while (menuChoice != 5)
    {
        menuChoice = printMenu();
        switch (menuChoice)
        {
        case 1:
            /* write formatted to text file */
            textFile();
            break;
        case 2:
            /* update an account */
            updateRecord();
            break;
        case 3:
            /* create an account */
            newRecord();
            break;
        case 4:
            /* delete an account */
            deleteRecord();
            break;
        case 5:
            /* exit program */
            printf("\nGood Bye.\n");
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

/******************************************************************************************
*Function name:              setup
*Purpose:                    Creates or resets existing file with 100 default accounts
*Function In parameters:     None
*Function Out parameters:    None
*Version:                    1
*Author:                     mos
******************************************************************************************/
void setup()
{
    FILE *fptr;
    int i = 0;
    struct clientInfo randomClient = {0, "", "", 0.0}; /*create clientInfo with default information*/

    /* set the file to blank records */
    if ((fptr = fopen("credit.dat", "wb")) == NULL)
    {
        printf("File could not be opened");
    }
    else
    {
        /*output 100 random blank records to file*/
        for (i = 0; i < 100; i++)
        {
            fwrite(&randomClient, sizeof(struct clientInfo), 1, fptr);
        }
        fclose(fptr);
    }
}

/******************************************************************************************
*Function name:              menu
*Purpose:                    Prints the menu for the functions and validates menu input
*Function In parameters:     None
*Function Out parameters:    The validated input for the menu
*Version:                    1
*Authors:                    mos
******************************************************************************************/
int printMenu()
{
    char input[3];
    int menuChoice = -1;

    while (menuChoice == -1)
    {
        printf("\nPlease choose one of the following actions:\n1- Output file of Accounts\n2- Update an Account\n3- Create a new Account\n4- Delete an account\n5- Quit\nPlease your menu choice: ");

        /* read input */
        fgets(input, 5, stdin);
        input[strcspn(input, "\n")] = 0;

        /* check if it is an int, then range */
        menuChoice = atoi(input);
        if (menuChoice == 0)
        {
            if (menuChoice == 0 && input[0] == '0' && input[1] == '\0')
            {
                /* input was 0 */
                printf("Option %d is not a valid choice. Options range from 1 to 5. Try again.\n", menuChoice);
                menuChoice = -1;
            }
            else
            {
                /* input wasnt an int */
                printf("Enter a valid whole number from 1 to 5. Try again.\n");
                menuChoice = -1;
            }
        }
        else if (menuChoice < 0 || menuChoice > 5)
        {
            /* if out of range */
            printf("Option %d is not a valid choice. Options range from 1 to 5. Try again.\n", menuChoice);
            menuChoice = -1;
        }
    }
    return menuChoice;
}

/******************************************************************************************
*Function name:              textFile
*Purpose:                    Prints a formatted output of non-default accounts to a text file
*Function In parameters:     None
*Function Out parameters:    None
*Version:                    1
*Author:                     mos
******************************************************************************************/
void textFile()
{
    FILE *fp;
    FILE *writePointer;
    struct clientInfo client = {0, "", "", 0.0}; /*create clientInfo with default information*/

    int i = 0;

    fp = fopen("credit.dat", "a+");
    if (fp == NULL)
    {
        printf("Error in opening file");
    }

    /* pointer to file, create if does not exist */
    writePointer = fopen("./accounts.txt", "w+");
    if (writePointer == NULL)
    {
        printf("File could not be opened\n");
    }

    /* write header */
    fprintf(writePointer, "%-15s %-21s %-21s %-21s\n", "accNo.", "lastName", "firstName", "accBalance");

    /* loop through dat file */
    while (fread(&client, sizeof(client), 1, fp) == 1)
    {
        /* if it is not default value, print to text file */
        if (client.AccNo != 0)
        {
            fprintf(writePointer, "%-15d %-21s %-21s %-21.2f\n", client.AccNo, client.lastName, client.firstName, client.accBalance);
        }
        i++;
    }

    /* close file */
    fclose(writePointer);
}

/******************************************************************************************
*Function name:              updateRecord
*Purpose:                    Asks for an existing account and updates the balance
*Function In parameters:     None
*Function Out parameters:    None
*Version:                    1
*Author:                     mos
******************************************************************************************/
void updateRecord()
{
    char input[20];
    int number = -1;
    char fName[20];
    char lName[20];
    double balance = -1.0;
    char menuOption[10];
    int success;
    struct clientInfo client = {0, "", "", 0.0};

    /* loop to validate acc number */
    while (number == -1)
    {
        printf("Enter Bank Account to Update (1-100): ");
        /* read input */
        fgets(input, 20, stdin);
        input[strcspn(input, "\n")] = 0;
        /* validate */
        number = validateAcc(input);
    }

    /* check if accno position is filled */
    if (findAcc(number, &client) == 1)
    {
        printf("Account %d does not exist.\n", number);
        return;
    }
    else
    {

        /* loop for balance */
        while (balance == -1)
        {
            printf("Enter charge (+) or payment (-): ");
            /* read input */
            fgets(input, 20, stdin);
            input[strcspn(input, "\n")] = 0;
            /* validate */
            balance = validateBal(input);
        }
        client.accBalance = client.accBalance + balance;

        /* update file */
        replaceRecord(number, client);
    }
}

/******************************************************************************************
*Function name:              newRecord
*Purpose:                    Asks for the details of and creates a new account
*Function In parameters:     None
*Function Out parameters:    None
*Version:                    1
*Author:                     mos
******************************************************************************************/
void newRecord()
{
    char input[20];
    int number = -1;
    char fName[20];
    char lName[20];
    double balance = -1.0;
    struct clientInfo client = {0, "", "", 0.0}; /*create clientInfo with default information*/

    /* loop to validate acc number */
    while (number == -1)
    {
        printf("Enter Account Number (1-100): ");
        /* read input */
        fgets(input, 20, stdin);
        input[strcspn(input, "\n")] = 0;
        /* validate */
        number = validateAcc(input);
    }

    /* check if accno position is empty */
    if (findAcc(number, &client) == 0)
    {
        printf("Account #%d already exists.\n", number);
        return;
    }
    else
    {
        /* add the rest of the information */

        /* loop for last name */
        lName[0] = '\0';
        while (lName[0] == '\0')
        {
            printf("Enter Account Holder's Last Name: ");
            /* store in variable, remove newline */
            fgets(lName, 20, stdin);
            lName[strcspn(lName, "\n")] = 0;
            /* validate */
            validateName(lName);
        }

        /* loop for first name */
        fName[0] = '\0';
        while (fName[0] == '\0')
        {
            printf("Enter Account Holder's First Name: ");
            /* store in variable, remove newline */
            fgets(fName, 20, stdin);
            fName[strcspn(fName, "\n")] = 0;
            /* validate */
            validateName(fName);
        }

        /* loop for balance */
        while (balance == -1)
        {
            printf("Enter Account Balance: ");
            /* read input */
            fgets(input, 20, stdin);
            input[strcspn(input, "\n")] = 0;
            /* validate */
            balance = validateBal(input);
        }

        /* add to a struct */
        client.AccNo = number;
        client.accBalance = balance;
        strcpy(client.lastName, lName);
        strcpy(client.firstName, fName);

        /* update file */
        replaceRecord(number, client);
    }
}

/******************************************************************************************
*Function name:              deleteAccount
*Purpose:                    Asks for the account number and deletes an existing account
*Function In parameters:     None
*Function Out parameters:    None
*Version:                    1
*Author:                     mos
******************************************************************************************/
void deleteRecord()
{
    char input[20];
    int number = -1;
    struct clientInfo client = {0, "", "", 0.0};

    /* loop to validate acc number */
    while (number == -1)
    {
        printf("Enter Account Number (1-100): ");
        /* read input */
        fgets(input, 20, stdin);
        input[strcspn(input, "\n")] = 0;
        /* validate */
        number = validateAcc(input);
    }

    /* check if accno position is empty */
    if (findAcc(number, &client) != 0)
    {
        printf("Account #%d does not exist.\n", number);
        return;
    }
    else
    {
        /* otherwise set default values and replace the selected account */
        client.AccNo = 0;
        client.accBalance = 0.0;
        strcpy(client.firstName, "");
        strcpy(client.lastName, "");
        replaceRecord(number, client);
    }
}

/******************************************************************************************
*Function name:              validateAcc
*Purpose:                    Takes in an account number and validates it is in range and an int
*Function In parameters:     A string of input to validate
*Function Out parameters:    Integer of the validated account number
*Version:                    1
*Author:                     mos
******************************************************************************************/
int validateAcc(char *input)
{
    int number;

    /* check if it is an int, then range */
    number = atoi(input);
    if (number == 0)
    {
        if (number == 0 && input[0] == '0' && input[1] == '\0')
        {
            /* input was 0 */
            printf("Cannot be 0. Try again.\n");
            number = -1;
        }
        else
        {
            /* input wasnt an int */
            printf("Input whole numbers only. try again.\n");
            number = -1;
        }
    }
    else if (number < 0)
    {
        /* if it is negative */
        printf("Must be a positive number. Try again.\n");
        number = -1;
    }
    else if (number > 100)
    {
        /* if it is too large */
        printf("Input is too large. Try again.\n");
        number = -1;
    }

    return number;
}

/******************************************************************************************
*Function name:              validateName
*Purpose:                    Validates a name does not have any invalid characters
*Function In parameters:     The string of input to validate
*Function Out parameters:    None
*Version:                    1
*Author:                     mos
******************************************************************************************/
void validateName(char *input)
{
    char tempChar = input[0];
    int i = 0;

    /* confirm it does not begin with dash, apostrophe */
    if (tempChar == 96 || tempChar == 45 || tempChar == 39)
    {
        printf("Can not begin with \"%c\", try again.\n", tempChar);
        i = strlen(input);
        input[0] = '\0';
    }
    /* verify there are no invalid characters in the whole string */
    while (i < strlen(input))
    {
        if (tempChar != 39 && tempChar != 45 && tempChar != 96 && !(tempChar >= 65 && tempChar <= 90) && !(tempChar >= 97 && tempChar <= 122))
        {
            printf("Invalid character \"%c\" entered, try again.\n", tempChar);
            input[0] = '\0';
            i = strlen(input);
        }
        i++;
        tempChar = input[i];
    }
}

/******************************************************************************************
*Function name:              validateBal
*Purpose:                    Validates an input is a correct format for account balance
*Function In parameters:     A string input to check and validate
*Function Out parameters:    A validated double number for balance
*Version:                    1
*Author:                     mos
******************************************************************************************/
double validateBal(char *input)
{
    double number;

    /* check if it is a float */
    number = atof(input);
    if (number == 0)
    {
        if ((input[0] == '0' && input[1] == '\0') || (input[1] == '0' && (input[0] == '-' || input[0] == '+') && input[2] == '\0'))
        {
            /* input was 0 so do nothing */
        }
        else
        {
            /* input wasnt an float */
            printf("Input a valid number. try again.\n");
            number = -1;
        }
    }

    return number;
}

/******************************************************************************************
*Function name:              findAcc
*Purpose:                    Looks for an account based on account number input
*Function In parameters:     The account number to look for, a pointer to client struct
*Function Out parameters:    1 or 0 for found or not
*Version:                    1
*Authors:                    mos
******************************************************************************************/
int findAcc(int accNo, struct clientInfo *client)
{
    FILE *fp;
    int i = 0;

    /* open the file in pointer*/
    fp = fopen("credit.dat", "rb");
    if (fp == NULL)
    {
        printf("Error in opening file");
    }

    /* go to provided position or account number */
    fseek(fp, (accNo - 1) * sizeof(struct clientInfo), SEEK_SET);
    fread(client, sizeof(struct clientInfo), 1, fp);

    /* check if equal */
    if ((*client).AccNo == 0)
    {
        /* not found */
        fclose(fp);
        return 1;
    }
    else
    {
        /* print record and return 0 for found */
        printf("%d   %s   %s   %.2f\n", (*client).AccNo, (*client).lastName, (*client).firstName, (*client).accBalance);
        fclose(fp);
        return 0;
    }
}

/******************************************************************************************
*Function name:              replaceRecord
*Purpose:                    Replaces a record at a provided position with a provided client
*Function In parameters:     The position to replace, the information replacing with
*Function Out parameters:    None
*Version:                    1
*Authors:                    mos
******************************************************************************************/
void replaceRecord(int accNo, struct clientInfo client)
{
    struct clientInfo tempClient;
    FILE *fp;
    int i = 0;

    /* open the file in pointer*/
    fp = fopen("credit.dat", "r+b");
    if (fp == NULL)
    {
        printf("Error in opening file");
        exit(1);
    }

    /* go to provided position or account number */
    fseek(fp, (accNo - 1) * sizeof(struct clientInfo), SEEK_SET);
    fwrite(&client, sizeof(struct clientInfo), 1, fp);

    /* close file */
    fclose(fp);
}
