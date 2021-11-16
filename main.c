#include <stdio.h>
#define MAXSIZE 1000
#define DATA_NUMBER 3
#define BURST_NUMBER_INDEX 0
#define ARRIVAL_TIME_INDEX 1
#define BURST_LENGTH_INDEX 2

  struct ProcessesData_s {
    int bursts_info [MAXSIZE][DATA_NUMBER];
    int count;
};
typedef struct ProcessesData_s ProcessesData;

void add_process_data (ProcessesData* processData, int arrivalTime, int burstLength){
    processData->bursts_info[processData->count][BURST_NUMBER_INDEX] = processData->count;
    processData->bursts_info[processData->count][ARRIVAL_TIME_INDEX] = arrivalTime;
    processData->bursts_info[processData->count][BURST_LENGTH_INDEX] = burstLength;
    processData->count++;
}
// in use , left time, and we have to increment counter for all in use processes
double turnaround_time_FCFS (int processDataArray [MAXSIZE][DATA_NUMBER], int count){
    return 0.0;
}
double turnaround_time_SJF (int processDataArray [MAXSIZE][DATA_NUMBER], int count){
    return 0.0;
}
double turnaround_time_SRTF (int processDataArray [MAXSIZE][DATA_NUMBER], int count){
    return 0.0;
}
double turnaround_time_RR (int processDataArray [MAXSIZE][DATA_NUMBER], int count, int q){
    return 0.0;
}

int main() {
    ProcessesData prData;
    prData.count = 0;

    add_process_data (&prData, 15, 175);
    add_process_data (&prData, 20, 200);

    printf("count is %d and arrival time %d and length is %d \n", prData.count, prData.bursts_info[0][1], prData.bursts_info[0][2]);
    printf("count is %d and arrival time %d and length is %d \n", prData.count, prData.bursts_info[1][1], prData.bursts_info[1][2]);

    return 0;
}
