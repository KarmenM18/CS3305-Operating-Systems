
/*
Course: CS 3305
Project: Assignment 4
Date: November 8, 2023
*/

/*
Purpose: implements the Shortest Job First (SJF) CPU Scheduling Algorithm to process multiple test cases from an input file. It reads process information from the file, schedules them according to SJF, and outputs detailed scheduling results.
Concepts Implemented: file parsing & data handling, turnaround & wait time calculation, dynamic process management, CPU scheduling algorithms
*/

#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
struct Process
{
    char name[10];
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
};

// Function to perform SJF scheduling
void sjf_scheduling(struct Process processes[], int num_processes,
                    int test_case);

// Comparison function for qsort to sort processes by burst time
int compare_burst_time(const void *a, const void *b);

int main()
{
    FILE *file = fopen("sjf_input.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Buffer to store test case content
    char line[100];
    int num_test_cases = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        num_test_cases++;

        int num_processes;
        struct Process *processes;

        // Read processes from the line
        sscanf(line, "%*s %*d %*d%n", &num_processes);

        // Allocate memory for an array of processes
        processes =
            (struct Process *)malloc(num_processes * sizeof(struct Process));

        // Parse the input line to fill the array of processes
        for (int i = 0; i < num_processes; i++)
        {
            sscanf(line, "%s %d %d", processes[i].name, &processes[i].arrival_time,
                   &processes[i].burst_time);
        }

        // Call SJF scheduling algorithm
        sjf_scheduling(processes, num_processes, num_test_cases);

        // Free allocated memory
        free(processes);
    }

    fclose(file);
    return 0;
}

/*
PURPOSE: algorithm that determines the priority of proceses based on SJD scheduling
  */
void sjf_scheduling(struct Process processes[], int num_processes,
                    int test_case)
{
    // Sort processes based on burst time
    qsort(processes, num_processes, sizeof(struct Process), compare_burst_time);

    // Initialize waiting time and turnaround time for the first process
    processes[0].waiting_time = 0;
    processes[0].turnaround_time = processes[0].burst_time;

    // Calculate waiting time and turnaround time for the rest of the processes
    for (int i = 1; i < num_processes; i++)
    {
        processes[i].waiting_time = processes[i - 1].turnaround_time;
        processes[i].turnaround_time =
            processes[i].waiting_time + processes[i].burst_time;
    }

    // Print the scheduling details
    printf("Test case #%d:", test_case);
    for (int i = 0; i < num_processes; i++)
    {
        printf(" %s %d %d", processes[i].name, processes[i].arrival_time,
               processes[i].burst_time);
    }
    printf("\n");

    printf("Number of Processes: %d\n", num_processes / 2);
    printf("Process Scheduling Started:\n");

    for (int i = 0; i < num_processes; i++)
    {
        printf("CPU Time %d: [%s Arrived] %s [%d/%d]\n", processes[i].waiting_time,
               processes[i].name, processes[i].name, processes[i].turnaround_time,
               processes[i].burst_time);
    }

    // Calculate average turnaround time and average waiting time
    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    for (int i = 0; i < num_processes; i++)
    {
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
    }

    double avg_turnaround_time = (double)total_turnaround_time / num_processes;
    double avg_waiting_time = (double)total_waiting_time / num_processes;

    // Print average turnaround time and average waiting time
    printf("Process scheduling completed with Avg Turnaround Time: %.2f, Avg "
           "Waiting Time: %.2f\n",
           avg_turnaround_time, avg_waiting_time);
}

/*
PURPOSE: determines which processes have the shortest burst time
  */
int compare_burst_time(const void *a, const void *b)
{
    return ((struct Process *)a)->burst_time - ((struct Process *)b)->burst_time;
}