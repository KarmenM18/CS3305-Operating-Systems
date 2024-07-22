/*
Course: CS 3305
Project: Assignment 2
Date: October 11, 2023
*/

/*
Purpose:  calculate the sum of N natural numbers using M processes
Concepts Implemented: process creation and control, piping for bi-directional communication between parent and child processes,
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int summation(int start, int end)
{
    int sum = 0;
    if (start < end)
    {
        sum = ((end * (end + 1)) - (start * (start - 1))) / 2;
    }
    return sum;
}

int ith_part_start(int i, int N, int M)
{
    int part_size = N / M;
    int start = i * part_size;
    return start;
}
int ith_part_end(int i, int N, int M)
{
    int part_size = N / M;
    int end = (i < M - 1) ? ((i + 1) * part_size - 1) : N;
    return end;
}

int main(int argc, char **argv)
{
    int port[2];
    int totalSum = 0;

    // Check if there was an error in pipe creation
    if (pipe(port) < 0)
    {
        perror("pipe error");
        exit(1);
    }

    // Check if the correct number of command-line arguments is provided
    if (argc != 3)
    {
        // Inform the user of the correct usage of program
        fprintf(stderr, "Usage: %s N M\n", argv[0]);
        return 1; // Exit with an error code
    }

    // Convert command-line arguments to appropriate types
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    // Ensure that user-defined values stay within bounds
    if (N > 100 || M > 10)
    {
        N = 100;
        M = 10;
    }

    // Print a message indicating the start of the parent process
    printf("\nparent (PID %d): process started\n", getpid());

    // Create child_1
    printf("parent (PID %d): forking child_1\n", getpid());
    pid_t child_1_pid = fork();

    // Enter child_1 process
    if (child_1_pid == 0)
    {
        // Print a message indicating successful creation of child_1
        printf("parent (PID %d): fork successful for child_1 (PID %d)\n", getppid(),
               getpid());
        printf("parent (PID %d): waiting for child_1 (PID %d) to complete\n",
               getppid(), getpid());

        // Creating sub-child processes to calculate partial sums
        char childNames[M][8 + M];
        printf("child_1 (PID %d): process started from parent(PID %d)\n", getpid(),
               getppid());

        for (int i = 0; i < M; i++)
        {
            sprintf(childNames[i], "child_1.%d", i + 1);
            // Print a message indicating that sub-children have been forked
            printf("child_1 (PID %d): forking %s\n", getpid(), childNames[i]);
        }

        for (int i = 0; i < M; i++)
        {
            pid_t pid = fork();

            // Catches unsuccessful fork
            if (pid == -1)
            {
                fprintf(stderr, "Error: fork() failed\n");
                exit(1);
            }
            // Execute new child sub-process
            else if (pid == 0)
            {

                // Print a message that child 1.i has forked successfully
                printf("%s (PID %d): fork() successful\n", childNames[i], getpid());
                int start = ith_part_start(i, N, M);
                int end = ith_part_end(i, N, M);
                int partial_sum = summation(start, end);

                // Writes the partial sum calculated in each iteration to the pipe
                ssize_t sumwrite = write(port[1], &partial_sum, sizeof(partial_sum));
                // Handle errors writing to pipe
                if (sumwrite == -1)
                {
                    perror("write");
                    exit(1);
                }

                // Print a message indicating the partial sum
                printf("%s (PID %d): partial sum: [%d - %d] = %d\n", childNames[i],
                       getpid(), start, end, partial_sum);
            }

            // Execute child_1
            else
            {
                wait(NULL);
                for (int i = 0; i < M; i++)
                {
                    int childSum;
                    ssize_t sumread = read(port[0], &childSum, sizeof(childSum));
                    // Handle errors with reading from pipe
                    if (sumread == -1)
                    {
                        perror("write");
                        exit(1);
                    }
                    // Aggregates all partial sums to calculate total sum of all processes
                    totalSum += childSum;
                }

                // Print a message indicating the total sum
                printf("child_1 (PID %d): total sum = %d\n", getpid(), totalSum);
                // Print a message indicating that child_1 is completed
                printf("child_1 (PID %d): child_1 completed\n", getpid());
            }
        }
    }
    else
    { // Enter parent process
        int status;
        waitpid(child_1_pid, &status, 0); // wait for child_1 to complete
        printf("parent (PID %d): parent completed\n", getpid());
    }

    return 0;
}