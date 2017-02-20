#include <stdio.h>
#include <pthread.h>


int numFiles = 4;

struct Account {
    int number;
    float balance;
};

int main() {

    pthread_t files[numFiles];
    struct Account account[numFiles];
    FILE *pFile;

    for (int i = 0; i < numFiles; i++) {

        // Determine customer file to be opened
        char fileName[10];
        snprintf(fileName, sizeof(fileName), "cust%d.dat", i);

        printf("Opening file: %s\n", fileName);
        pFile = fopen(fileName, "r");

        fscanf (pFile, "%d", &account[i].number);
        printf("Account number: %08d\n", account[i].number);

        fscanf (pFile, "%f", &account[i].balance);
        printf("Account balance: $%.2f\n\n", account[i].balance);
    }

    return 0;
}
