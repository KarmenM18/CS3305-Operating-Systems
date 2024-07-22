
/*
Course: CS 3305
Project: Assignment 1
Date: Sept 29, 2023
*/

/*
Purpose: Developed a C program that calculates the sums of natural numbers from 1 to N and from 1 to M using two separate processes.
Concepts Implemented: process creation control using C in Linux environment, using system functions such as fork(), wait(), excel()
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Function to execute an external program with an argument
void runExternal(char* programName, char* argument) {
    printf("calling an external program [%s]\n", programName);
    execl(programName, programName, argument, NULL); // Execute the external program
    perror("execl() failed"); // Print an error message if execl() fails
    exit(1); // Exit the child process with an error code
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 4) {
        fprintf(stderr, "Usage: %s N M S\n", argv[0]);
        return 1; // Exit with an error code
    }

    // Convert command-line arguments to appropriate types
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    char *S = argv[3];

    // Print a message indicating the start of the parent process
    printf("parent (PID %d): process started\n", getpid());

    // Create child_1
    pid_t child_1_pid = fork();
    if (child_1_pid == 0) { // Child process (child_1)
        // Print a message indicating the start of child_1
        printf("child_1 (PID %d): process started from parent (PID %d)\n", getpid(), getppid());

        // Create child_1.1
        pid_t child_1_1_pid = fork();
        if (child_1_1_pid == 0) { // Child process (child_1.1)
            // Print a message indicating the start of child_1.1
            printf("child_1.1 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());

            // Execute external_program1.out with N as an argument
            runExternal("./external_program1.out", argv[1]);
        } else { // Parent process (child_1)
            int status;
            waitpid(child_1_1_pid, &status, 0); // Wait for child_1.1 to complete

            // Print a message indicating the completion of child_1.1
            printf("child_1 (PID %d): completed child_1.1\n", getpid());

            // Create child_1.2
            pid_t child_1_2_pid = fork();
            if (child_1_2_pid == 0) { // Child process (child_1.2)
                // Print a message indicating the start of child_1.2
                printf("child_1.2 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());

                // Execute external_program1.out with M as an argument
                runExternal("./external_program1.out", argv[2]);
            } else { // Parent process (child_1)
                wait(NULL); // Wait for any child process to complete

                // Print a message indicating the completion of child_1.2
                printf("child_1 (PID %d): completed child_1.2\n", getpid());
                exit(0); // Exit the child_1 process
            }
        }
    } else { // Parent process (main program)
        int status;
        waitpid(child_1_pid, &status, 0); // Wait for child_1 to complete

        // Print messages indicating the successful fork and waiting for child_1
        printf("parent (PID %d): fork successful for child_1 (PID %d)\n", getpid(), child_1_pid);
        printf("parent (PID %d): waiting for child_1 (PID %d) to complete\n", getpid(), child_1_pid);

        // Create child_2
        pid_t child_2_pid = fork();
        if (child_2_pid == 0) { // Child process (child_2)
            // Print a message indicating the start of child_2
            printf("child_2 (PID %d): process started from parent (PID %d)\n", getpid(), getppid());

            // Execute external_program2.out with S as an argument
            runExternal("./external_program2.out", S);
        } else { // Parent process (main program)
            wait(NULL); // Wait for any child process to complete

            // Print a message indicating the completion of the parent process
            printf("parent (PID %d): completed parent\n", getpid());
        }
    }

    return 0;
}
