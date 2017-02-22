#include <stdio.h>
#include <pthread.h>


int numFiles = 4;
const int numATMs = 3;
int thread_ids[numATMs] = {0, 1, 2};

struct Account {
    int number;
    float balance;
};

void *transaction(void * arg) {
    int accNumber;
    char tranType;
    float tranAmount;
    int waitTime;

    FILE *transaction;

    transaction = fopen("atm0.dat", "r");
    for (;;) {

        fscanf(transaction, "%d %c %f %d", &accNumber, &tranType, & tranAmount, &waitTime);

        if (feof(transaction)) break;

        printf("\nAccount: %08d\n", accNumber);
        printf("Transaction type: %c\n", tranType);
        printf("Transaction amount: %.2f\n", tranAmount);
        printf("Wait time: %d\n", waitTime);
    }

    return NULL;
}

int main() {

    int i, j, k;
    struct Account account[numFiles];
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
        printf("Account number: %08d\n", account[i].number);
        printf("Account balance: $%.2f\n\n", account[i].balance);
    }
printf("****************************************************");
    /*** Reopen the files for writing ***/
    /* Not sure why this is needed yet */
    //pFile = fopen(fileName, "w");
    //fprintf(pFile, "%f", account[i].balance);

    /*** ATM implementation ***/
    for (j = 0; j < numATMs; j++) {
        pthread_create(&thread[j], NULL, transaction, &thread_ids[j]);
        //pass one of the atm.dat files into this thread
        //pthread_join(thread[j], NULL);
    }

    for (k = 0; k < numATMs; k++) {
        pthread_join(thread[k], NULL);
    }

    /*** main.c work queue ***/


    return 0;
}
