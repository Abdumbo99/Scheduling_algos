#include <stdio.h>
#define MAXSIZE 1000
#define DATA_NUMBER 3
#define BURST_NUMBER_INDEX 0
#define ARRIVAL_TIME_INDEX 1
#define BURST_LENGTH_INDEX 2

struct ProcessesData_s
{
    int bursts_info[MAXSIZE][DATA_NUMBER];
    int count;
};
typedef struct ProcessesData_s ProcessesData;

void add_process_data(ProcessesData *processData, int arrivalTime, int burstLength)
{
    processData->bursts_info[processData->count][BURST_NUMBER_INDEX] = processData->count;
    processData->bursts_info[processData->count][ARRIVAL_TIME_INDEX] = arrivalTime;
    processData->bursts_info[processData->count][BURST_LENGTH_INDEX] = burstLength;
    processData->count++;
}
// in use , left time, and we have to increment counter for all in use processes
double turnaround_time_FCFS(int processDataArray[MAXSIZE][DATA_NUMBER], int count)
{
    return 0.0;
}
double turnaround_time_SJF(int processDataArray[MAXSIZE][DATA_NUMBER], int count)
{
    printf("\n\n========SJF=======\n\n");
    double waitingAvg = 0, turnaroundAvg = 0, turnaroundTotalTime = 0, waitingTotalSum = 0;
    int waitTime[count];
    float burstTime = 0;
    int min;
    int minIndex = 1;
    for (int i = 0; i < count; i++)
    {
        burstTime = burstTime + processDataArray[i][2];

        min = processDataArray[minIndex][BURST_LENGTH_INDEX];
        for (int j = minIndex; j < count; j++)
        {
            if (burstTime >= processDataArray[j][ARRIVAL_TIME_INDEX] && processDataArray[j][BURST_LENGTH_INDEX] < min)
            {
                min = processDataArray[j][BURST_LENGTH_INDEX];
                // Swap the order of two burst
                int temp = processDataArray[minIndex][ARRIVAL_TIME_INDEX];
                processDataArray[minIndex][ARRIVAL_TIME_INDEX] = processDataArray[j][ARRIVAL_TIME_INDEX];
                processDataArray[j][ARRIVAL_TIME_INDEX] = temp;

                temp = processDataArray[minIndex][BURST_LENGTH_INDEX];
                processDataArray[minIndex][BURST_LENGTH_INDEX] = processDataArray[j][BURST_LENGTH_INDEX];
                processDataArray[j][BURST_LENGTH_INDEX] = temp;
            }
        }
        minIndex++;
    }
    for (int i = 0; i < count; i++)
    {
        printf("index is %d and arrival time %d and length is %d \n", count, processDataArray[i][ARRIVAL_TIME_INDEX], processDataArray[i][BURST_LENGTH_INDEX]);
        turnaroundTotalTime = turnaroundTotalTime + processDataArray[i][BURST_LENGTH_INDEX];
        turnaroundTotalTime = turnaroundTotalTime - processDataArray[i][ARRIVAL_TIME_INDEX];
    }
    turnaroundAvg = turnaroundTotalTime / count;
    printf("Avg turnaround: %fl\n", turnaroundAvg);
    return turnaroundAvg;
}
double turnaround_time_SRTF(int processDataArray[MAXSIZE][DATA_NUMBER], int count)
{
    return 0.0;
}
double turnaround_time_RR(int processDataArray[MAXSIZE][DATA_NUMBER], int count, int q)
{
    return 0.0;
}

int main()
{
    ProcessesData prData;
    prData.count = 0;

    FILE *fp;
    char *name = "input.txt";
    printf("file name %s \n", name);

    fp = fopen(name, "r");
    if (fp != NULL)
    {
        int index;
        int arrivalTime;
        int burstTime;
        while (fscanf(fp, "%d %d %d", &index, &arrivalTime, &burstTime) != EOF)
        {
            add_process_data(&prData, arrivalTime, burstTime);
        }
    }

    for (int i = 0; i < prData.count; i++)
    {
        printf("count is %d and arrival time %d and length is %d \n", prData.count, prData.bursts_info[i][1], prData.bursts_info[i][2]);
    }

    turnaround_time_SJF(prData.bursts_info, prData.count);

    return 0;
}