
/*
Course: CS 3305
Project: Assignment 5
Date: December 1, 2023
*/

/*
Purpose: uses mutual exclusion to synchronize multiple threads performing transactions on shared bank accounts, ensuring atomic operations and preventing negative balances.
Concepts Implemented: mutual exclusion, semaphones, critical sections, thread synchronization
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Define the maximum number of accounts and clients (arbitrary amount)
#define MAX_ACCOUNTS 10
#define MAX_CLIENTS 10

// Define the structure for the bank account
typedef struct
{
    int balance;
    pthread_mutex_t lock;
} BankAccount;

// Define the structure for the client transaction
typedef struct
{
    int accountIndex;
    int amount;
    char transactionType;
} ClientTransaction;

// Global variables
int numAccounts = 0;
int numClients = 0;
BankAccount accounts[MAX_ACCOUNTS];
ClientTransaction transactions[MAX_CLIENTS];

// Function to process a client's transactions
void *processClient(void *arg)
{
    int clientIndex = *(int *)arg;

    for (int i = 0; i < numClients; ++i)
    {
        if (i == clientIndex)
        {
            int accountIndex = transactions[i].accountIndex;
            int amount = transactions[i].amount;
            char transactionType = transactions[i].transactionType;

            pthread_mutex_lock(&accounts[accountIndex - 1].lock);

            if (transactionType == 'd')
            {
                accounts[accountIndex - 1].balance += amount;
            }
            else if (transactionType == 'w')
            {
                if (accounts[accountIndex - 1].balance >= amount)
                {
                    accounts[accountIndex - 1].balance -= amount;
                }
            }

            pthread_mutex_unlock(&accounts[accountIndex - 1].lock);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    // Open the input file
    FILE *file = fopen("assignment_5_input.txt", "r");
    if (!file)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Process the input file to determine the number of accounts
    while (fscanf(file, "A%d balance %d", &numAccounts,
                  &accounts[numAccounts - 1].balance) == 2)
    {
        // Initialize the mutex lock for each account
        pthread_mutex_init(&accounts[numAccounts - 1].lock, NULL);
    }

    // Reset file pointer to read transactions
    fseek(file, 0, SEEK_SET);

    // Skip lines indicating the number of accounts and clients
    fscanf(file, "No. of Accounts: %*d");
    fscanf(file, "No. of Clients: %*d");

    // Read client transactions with one action
    while (fscanf(file, "C%d %c A%d %d", &transactions[numClients].accountIndex,
                  &transactions[numClients].transactionType,
                  &transactions[numClients].accountIndex,
                  &transactions[numClients].amount) == 4)
    {
        numClients++;
    }

    // Read client transactions with two actions
    while (fscanf(file, "C%d %c A%d %d %p A%e %d",
                  &transactions[numClients].accountIndex,
                  &transactions[numClients].transactionType,
                  &transactions[numClients].accountIndex,
                  &transactions[numClients].amount,
                  &transactions[numClients].transactionType,
                  &transactions[numClients].accountIndex,
                  &transactions[numClients].amount) == 7)
    {
        numClients++;
    }

    // Close the input file
    fclose(file);

    // Create and join threads for processing client transactions
    pthread_t threads[MAX_CLIENTS];
    int clientIndices[MAX_CLIENTS];

    for (int i = 0; i < numClients; ++i)
    {
        clientIndices[i] = i;
        if (pthread_create(&threads[i], NULL, processClient, &clientIndices[i]) !=
            0)
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numClients; ++i)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error joining thread");
            exit(EXIT_FAILURE);
        }
    }

    // Print the final account balances
    printf("No. of Accounts: %d\n", numAccounts);
    printf("No. of Clients: %d\n", numClients);
    for (int i = 0; i < numAccounts; ++i)
    {
        printf("A%d balance %d\n", i + 1, accounts[i].balance);
    }

    // Destroy mutex locks for each account
    for (int i = 0; i < numAccounts; ++i)
    {
        pthread_mutex_destroy(&accounts[i].lock);
    }

    return 0;
}