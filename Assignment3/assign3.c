/*
Course: CS 3305
Project: Assignment 3
Date: October 26, 2023
*/

/*
Purpose: this C program uses threads to perform a sequence of operations:
   1) Calculate the sum of two integers and store it.
   2) Determine if the sum is even or odd, and calculate their product.
   3) Reverse the product number.
   -> The main process coordinates the threads to ensure correct execution order.
Concepts Implemented: working with pthread() system functions, creating multiple threads for different tasks, identifying how different threads can access shared data,
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Defining input_array for computational steps
int input_array[4];

// Declaring 3 threads
pthread_t thread_1, thread_2, thread_3;

/*
PURPOSE: Performs summation between X and Y, writes the result to input_array
Parameters: pointer to thread_1's TID
Returns: N/A
*/
void *sum(void *thread_id)
{
    int TID = *((int *)thread_id);
    int X = input_array[0];
    int Y = input_array[1];

    // Print message displaying the X and Y values read from the array
    printf("\nthread_1 (TID %d) reads X = %d and Y = %d from input_array[]\n",
           TID, X, Y);

    // Perform summation
    int S = X + Y;

    // Store the sum into input_array
    input_array[2] = S;

    // Prints message showing the sum of X and Y
    printf("\nthread_1 (TID %d) writes X + Y = %d to the input_array[2]\n", TID,
           S);
}

/*
PURPOSE: Identifies whether sum stored in input_array[] is even or odd
Parameters: pointer to thread_2's TID
Returns: N/A
*/
void *even_odd(void *thread_id)
{
    int TID = *((int *)thread_id);

    int S = input_array[2];
    // Prints message displaying value read from input_array[2]
    printf("\nthread_2 (TID %d) reads %d from the input_array[2]\n", TID, S);

    // Check if the sum is an even number
    if (S % 2 == 0)
    {
        printf("\nthread_2 (TID %d) identifies that %d is an even number\n", TID,
               S);
        // Executes if the calculates sum is an odd number
    }
    else
    {
        printf("\nthread_2 (TID %d) identifies that %d is an odd number\n", TID, S);
    }
}

/*
PURPOSE: Multiplies X and Y values from input_array[], writes result to array
Parameters: pointer to thread_2's TID
Returns: N/A
*/
void *multiplication(void *thread_id)
{
    int TID = *((int *)thread_id);

    int X = input_array[0];
    int Y = input_array[1];

    // Calculates the product of X and Y, stores the result in input_array[3]
    int M = X * Y;
    input_array[3] = M;

    // Print message displaying X and Y values read from array, and their product
    printf("\nthread_2 (TID %d) reads X and Y from input_array[], writes X * Y = "
           "%d to input_array[3]\n",
           TID, M);
}

/*
PURPOSE: Reverses the digits of the number stored in M
Parameters: pointer to thread_3's TID
Returns: N/A
*/
void *reverse_num(void *thread_id)
{
    int TID = *((int *)thread_id);

    // Read value stored in M from input_array
    int M = input_array[3];
    int reverse = 0, remainder;

    // Print message indicating value read from input_array[3]
    printf("\nthread_3 (TID %d) reads %d input_array[3]\n", TID, M);

    // Reverses the value stored in M
    while (M != 0)
    {
        remainder = M % 10;                 // Stores the rightmost digit
        reverse = reverse * 10 + remainder; // Add the digit to the reversed number
        M /= 10;                            // Remove the rightmost digit from original number
    }

    // Print message displaying the number before and after reversal
    printf("\nthread_3 (TID %d) reverses the number %d -> %d\n", TID,
           input_array[3], reverse);
}

int main(int argc, char **argv)
{

    // Check if the correct number of command-line arguments is provided
    if (argc != 3)
    {

        // Inform the user of the correct usage of program
        fprintf(stderr, "Usage: %s X Y\n", argv[0]);
        return 1; // Exit with an error code
    }

    // Convert command-line arguments to appropriate types
    int X = atoi(argv[1]);
    int Y = atoi(argv[2]);

    // Ensure that user-defined values stay within bounds
    if (X < 0 || Y < 0)
    {
        X = X * -1;
        Y = Y * -1;
    }

    // Print a message indicating that X and Y values have been received
    printf("\nparent (PID %d) receives X = %d and Y = %d from user\n", getpid(),
           X, Y);

    // Write X and Y to input_array[0] and input_array[1] respectively
    input_array[0] = X;
    input_array[1] = Y;

    // Print message that X and Y have been written to input_array
    printf("\nparent (PID %d) writes X = %d and Y = %d to input_array[]\n",
           getpid(), X, Y);

    // Create thread_1, performs sum function
    pthread_create(&thread_1, NULL, sum, (void *)&thread_1);
    pthread_join(thread_1, NULL);

    // Create thread_2, performs even_odd function
    pthread_create(&thread_2, NULL, even_odd, (void *)&thread_2);
    pthread_join(thread_2, NULL);

    // Thread_2 performs multiplication function
    pthread_create(&thread_2, NULL, multiplication, (void *)&thread_2);
    pthread_join(thread_2, NULL);

    // Create thread_3, performs reverse_num function
    pthread_create(&thread_3, NULL, reverse_num, (void *)&thread_3);
    pthread_join(thread_3, NULL);
}