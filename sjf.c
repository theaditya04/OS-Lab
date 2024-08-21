#include <stdio.h>

#define MAX 10

// Function to calculate the waiting time and turnaround time
void findWaitingTime(int processes[], int n, int bt[], int wt[]) {
    // Initial waiting time for the first process is 0
    wt[0] = 0;

    // Calculate waiting time for each process
    for (int i = 1; i < n ; i++) {
        wt[i] = 0;
        for (int j = 0; j < i ; j++)
            wt[i] += bt[j];
    }
}

// Function to calculate turnaround time
void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];
}

// Function to find the average time
void findavgTime(int processes[], int n, int bt[]) {
    int wt[MAX], tat[MAX];
    float total_wt = 0, total_tat = 0;

    // Calculate waiting time
    findWaitingTime(processes, n, bt, wt);

    // Calculate turnaround time
    findTurnAroundTime(processes, n, bt, wt, tat);

    // Display process details and average times
    printf("Process\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("%d\t%d\t\t%d\t\t%d\n", processes[i], bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
}

// Function to sort processes by burst time
void sortProcessesByBurstTime(int processes[], int n, int bt[]) {
    int temp;
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (bt[i] > bt[j]) {
                // Swap burst times
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;
                
                // Swap process IDs
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

int main() {
    int processes[MAX], burst_time[MAX];
    int n;

    // Input the number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Input process IDs and burst times
    printf("Enter process IDs and burst times:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d %d", &processes[i], &burst_time[i]);
    }

    // Sort processes by burst time
    sortProcessesByBurstTime(processes, n, burst_time);

    // Calculate and display average waiting and turnaround times
    findavgTime(processes, n, burst_time);

    return 0;
}

