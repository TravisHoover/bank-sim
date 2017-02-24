#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


const int numFiles = 4;
const int numATMs = 3;
int thread_ids[3] = {0, 1, 2};
int done = 0;

struct Account {
    int number;
    float balance;
};
struct Account account[4];

void *transaction(void * arg) {
    int accNumber;
    char tranType;
    float tranAmount;
    unsigned int waitTime;

    FILE *transaction;
    int threadID = *(int *)arg;

    char atmFile[10];
    snprintf(atmFile, sizeof(atmFile), "atm%d.dat", threadID);

    printf("Thread %d opening %s\n", threadID, atmFile);

    transaction = fopen(atmFile, "r");
    for (;;) {

        int t;

        fscanf(transaction, "%d %c %f %d", &accNumber, &tranType, & tranAmount, &waitTime);

        if (feof(transaction)) break;

        /*printf("\nAccount: %08d\n", accNumber);
        printf("Transaction type: %c\n", tranType);
        printf("Transaction amount: %.2f\n", tranAmount);
        printf("Wait time: %d\n", waitTime);*/

        //update account
        for (t = 0; t < numFiles; t++) {            //scan all cust.dat files to find account
            if (accNumber == account[t].number) {
                if (tranType == 'd') {
                    account[t].balance += tranAmount;
                } else if (tranType == 'w') {
                    account[t].balance -= tranAmount;
                } else {
                    printf("Error reading type of transaction.\n");
                }

                //print the current account and it's current balance
                printf("New balance for account %08d: %.2f\n", accNumber, account[t].balance);

                //print overdrawn message if necessary
                if (account[t].balance < 0) {
                    printf("Warning: Account %08d has overdrafted. There will be a penalty of $10\n", accNumber);
                    account[t].balance -= 10;
                }
            }
            //put the transaction into the main program's work queue

            //sleep
            sleep(waitTime);
        }
    }

    done++;

    return NULL;
}

int main() {

    int i, j, k;

    FILE *pFile;
    pthread_t thread[numATMs];

    /*** Read in customer accounts ***/
    for (i = 0; i < numFiles; i++) {
        // Determine customer file to be opened
        char fileName[10];
        snprintf(fileName, sizeof(fileName), "cust%d.dat", i);

        printf("Opening file: %s\n", fileName);
        pFile = fopen(fileName, "r");
        fscanf(pFile, "%d %f", &account[i].number, &account[i].balance);
        //printf("Account number: %08d\n", account[i].number);
        //printf("Account balance: $%.2f\n\n", account[i].balance);
    }
//printf("********** Transactions **********");
    /*** Reopen the files for writing ***/
    /* Not sure why this is needed yet */
    //pFile = fopen(fileName, "w");
    //fprintf(pFile, "%f", account[i].balance);

    /*** ATM implementation ***/
    for (j = 0; j < numATMs; j++) {
        pthread_create(&thread[j], NULL, transaction, &thread_ids[j]);
        //pass one of the atm.dat files into this thread
    }

    for (k = 0; k < numATMs; k++) {
        pthread_join(thread[k], NULL);
    }

    /*** main.c work queue ***/

    if (done == numATMs) {
        printf("\n/***** Final Balances *****/\n");
        for (i = 0; i < numFiles; i++) {
            printf("Account number: %08d\n", account[i].number);
            printf("Account balance: $%.2f\n\n", account[i].balance);
        }
    } else {
        printf("\nError: one of the threads did not complete successfully\n");
    }


    return 0;
}
