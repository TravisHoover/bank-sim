#include <stdio.h>
#include <pthread.h>


int numFiles = 4;
int numATMs = 3;

struct Account {
    int number;
    float balance;
    char tranType;
    float tranAmount;
    int waitTime;
};

void *transaction(void *arg) {
    struct Account *input = arg;
    int accNumber = input->number;
    char tranType = input->tranType;
    float tranAmount = input->tranAmount;
    int waitTime = input->waitTime;

    FILE *transaction;

    transaction = fopen("atm0.dat", "r");
    fscanf(transaction, "%d", &accNumber);
    fscanf(transaction, "%c", &tranType);
    fscanf(transaction, "%f", &tranAmount);
    fscanf(transaction, "%d", &waitTime);
    printf("\nAccount: %08d\n", accNumber);
    printf("Transaction type: %c\n", tranType);
    printf("Transaction amount: %.2f\n", tranAmount);
    printf("Wait time: %d\n\n", waitTime);

    return NULL;
}

int main() {

    int i, j;
    struct Account account[numFiles];
    FILE *pFile;
    //pthread_t thread[numATMs];

    for (i = 0; i < numFiles; i++) {

        // Determine customer file to be opened
        char fileName[10];
        snprintf(fileName, sizeof(fileName), "cust%d.dat", i);

        printf("Opening file: %s\n", fileName);
        pFile = fopen(fileName, "r");
        fscanf(pFile, "%d", &account[i].number);
        printf("Account number: %08d\n", account[i].number);
        fscanf(pFile, "%f", &account[i].balance);
        printf("Account balance: $%.2f\n\n", account[i].balance);
    }

    /* Not sure why this is needed yet */
    //pFile = fopen(fileName, "w");
    //fprintf(pFile, "%f", account[i].balance);

    /*** ATM implementation ***/
    for (j = 0; j < numATMs; j++) {
        //pthread_create(&thread[j], NULL, &transaction, &account[j]);
        //pass one of the atm.dat files into this thread
        //pthread_join(thread[j], NULL);
    }

    /*** main.c work queue ***/



    return 0;
}
