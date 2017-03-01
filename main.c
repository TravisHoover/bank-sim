/*
 * Author: Travis Hoover
 * Date: March 1, 2017
 * Course: CSCI 4300
 * Professor: Dr. Pettey
 *
 * This program will simulate a bank, practicing work with concurrent programming
 * by treating each pthread as an individual ATM.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

const int numAccounts = 4;
const int numATMs = 3;
int thread_ids[4] = {0, 1, 2, 3};
volatile int done = 0;
volatile int queueCount = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t threshold = PTHREAD_COND_INITIALIZER;

struct Account {
    int number;
    float balance;
};

struct Queue {
    int atmNum;
    int accNum;
    char transactionType;
    float transactionAmount;
    float balance;
};

struct Account account[4];
struct Queue queue[3];

void * mainQueue(void * idp) {
    while(!done) {
        int q, c;
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&threshold, &mutex);
        for (c = 0; c < 3; c++) {               //for loop for queue
            //print transactions to the appropriate customer file
            for (q = 0; q < numAccounts; q++) {     //for loop for number of cust.dat files
                if (queue[c].accNum == account[q].number) {

                    char fileName[10];
                    snprintf(fileName, sizeof(fileName), "cust%d.dat", q);

                    FILE *write;
                    write = fopen(fileName, "a");

                    fprintf(write, "%d  %c  %.2f  %.2f\n", queue[c].atmNum,
                            queue[c].transactionType,
                            queue[c].transactionAmount,
                            queue[c].balance);
                    queueCount--;
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

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

        fscanf(transaction, "%d %c %f %d", &accNumber, &tranType, &tranAmount, &waitTime);

        if (feof(transaction)) {
            break;
        } else {

            //update account
            for (t = 0; t < numAccounts; t++) {            //scan all cust.dat files to find account
                pthread_mutex_lock(&mutex);
                if (accNumber == account[t].number) {

                    if (tranType == 'd') {
                        account[t].balance += tranAmount;
                    } else if (tranType == 'w') {
                        account[t].balance -= tranAmount;
                    } else {
                        printf("Error reading type of transaction.\n");
                    }

                    //print overdrawn message if necessary
                    if (account[t].balance < 0) {
                        //printf("Warning: Account %08d has become overdraft. There will be a penalty of $10\n", accNumber);
                        account[t].balance -= 10;
                    }

                    //print the current account and it's current balance
                    printf("New balance for account %08d: %.2f after adding %.2f\n", accNumber, account[t].balance,
                           tranAmount);

                    queue[queueCount].atmNum = threadID;
                    queue[queueCount].accNum = accNumber;
                    queue[queueCount].transactionType = tranType;
                    queue[queueCount].transactionAmount = tranAmount;
                    queue[queueCount].balance = account[t].balance;
                    queueCount++;

//                char writeName[10];
//                snprintf(writeName, sizeof(writeName), "cust%d.dat", t);
//                FILE *transWrite;
//                transWrite = fopen(writeName, "a");
//                fprintf(transWrite, "%d %c %.2f %.2f \n", threadID, tranType, tranAmount, account[t].balance);

                    if (queueCount == 3) {
                        printf("queueCount 3\n");
                        pthread_cond_signal(&threshold);
                    }
                    //sleep
                    sleep(waitTime);
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    done++;

    return NULL;
}

int main() {

    int i, j, k;

    FILE *pFile;
    pthread_t thread[numATMs];
    pthread_t main;

    /*** Read in customer accounts ***/
    for (i = 0; i < numAccounts; i++) {
        // Determine customer file to be opened
        char fileName[10];
        snprintf(fileName, sizeof(fileName), "cust%d.dat", i);

        printf("Opening file: %s\n", fileName);
        pFile = fopen(fileName, "r");
        fscanf(pFile, "%d %f", &account[i].number, &account[i].balance);
    }

    /*** ATM implementation ***/
    for (j = 0; j < numATMs; j++) {
        pthread_create(&thread[j], NULL, transaction, &thread_ids[j]);
    }
    pthread_create(&main, NULL, mainQueue, &thread_ids[4]);

    for (k = 0; k < numATMs; k++) {
        pthread_join(thread[k], NULL);
    }
    pthread_join(main,NULL);

    if (done == numATMs) {
        printf("\n/***** Final Balances *****/\n");
        for (i = 0; i < numAccounts; i++) {
            printf("The ending balance for %08d is $%.2f\n\n", account[i].number, account[i].balance);
        }
    } else {
        printf("\nError: one of the threads did not complete successfully\n");
    }

    return 0;
}
