#include <stdio.h>
#define MAXSIZE 1000
#define DATA_NUMBER 3
#define QUEUE_DATA_NUMBER 4
#define BURST_NUMBER_INDEX 0
#define ARRIVAL_TIME_INDEX 1
#define BURST_LENGTH_INDEX 2
#define TIME_TURNAROUND 3

struct ProcessesData_s
{
    int bursts_info[MAXSIZE][DATA_NUMBER];
    int count;
};
typedef struct ProcessesData_s ProcessesData;

struct ProcessesQueue_s
{
    int bursts_enqueued[MAXSIZE][QUEUE_DATA_NUMBER];
    int count;
};
typedef struct ProcessesQueue_s ProcessesQueue;

void add_process_data(ProcessesData *processData, int arrivalTime, int burstLength)
{
    processData->bursts_info[processData->count][BURST_NUMBER_INDEX] = processData->count;
    processData->bursts_info[processData->count][ARRIVAL_TIME_INDEX] = arrivalTime;
    processData->bursts_info[processData->count][BURST_LENGTH_INDEX] = burstLength;
    processData->count++;
}

/*
 * As we are progressing with processing of the bursts we keep ones with past arrival time here
 * We update turnaround time as they get processed.
*/
void enqueue_process (ProcessesQueue *processesQueue, int arrivalTime, int burstLength){
    processesQueue->bursts_enqueued[processesQueue->count][BURST_NUMBER_INDEX] = processesQueue->count;
    processesQueue->bursts_enqueued[processesQueue->count][ARRIVAL_TIME_INDEX] = arrivalTime;
    processesQueue->bursts_enqueued[processesQueue->count][BURST_LENGTH_INDEX] = burstLength;
    processesQueue->count++;
}

int consume_from_queue (ProcessesQueue *processesQueue, int index){
    if (processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX] > 0) {
        processesQueue->bursts_enqueued[index][TIME_TURNAROUND]++;
        processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX]--;
        return 0;
    }
    return -1;
}
// in use , left time, and we have to increment counter for all in use processes
double turnaround_time_FCFS(int processDataArray[MAXSIZE][DATA_NUMBER], int size)
{
    ProcessesQueue pQueue;
    pQueue.count = 0;
    int sourceCurrentIndex = 0;
    int queueCurrentIndex = 0;
    int timer = 0;
    int done = 0;
    while (!done)
    {
        //first add
        if (timer == processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX])
        {
            enqueue_process(&pQueue,processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX],
                            processDataArray[sourceCurrentIndex][BURST_LENGTH_INDEX]);
            sourceCurrentIndex++;
        }
        //second consume
        if (consume_from_queue(&pQueue,queueCurrentIndex) == -1)
        {
            queueCurrentIndex++;
        }
        if (size > queueCurrentIndex)
            done = 1;
        timer++;
    }
    return 0.0;
}

double turnaround_time_SJF(int processDataArray[MAXSIZE][DATA_NUMBER], int count)
{
    return 0.0;
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
    ProcessesQueue qData;

    prData.count = 0;
    qData.count = 0;

    enqueue_process(&qData, 15, 25);
    enqueue_process(&qData, 10, 2);
    printf("turnaround time is %d and length remaining is %d\n",
           qData.bursts_enqueued[0][TIME_TURNAROUND], qData.bursts_enqueued[0][BURST_LENGTH_INDEX]);
    printf("turnaround time is %d and length remaining is %d\n",
           qData.bursts_enqueued[1][TIME_TURNAROUND], qData.bursts_enqueued[1][BURST_LENGTH_INDEX]);

    consume_from_queue(&qData, 0);
    consume_from_queue(&qData, 0);
    printf("turnaround time is %d and length remaining is %d\n",
           qData.bursts_enqueued[0][TIME_TURNAROUND], qData.bursts_enqueued[0][BURST_LENGTH_INDEX]);
    consume_from_queue(&qData, 1);
    printf("turnaround time is %d and length remaining is %d\n",
           qData.bursts_enqueued[1][TIME_TURNAROUND], qData.bursts_enqueued[1][BURST_LENGTH_INDEX]);

    add_process_data(&prData, 15, 175);
    add_process_data(&prData, 20, 200);

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
            printf("%d %d %d\n", index, arrivalTime, burstTime);
            add_process_data(&prData, arrivalTime, burstTime);
        }
    }

    for (int i = 0; i < prData.count; i++)
    {
        printf("count is %d and arrival time %d and length is %d \n", prData.count, prData.bursts_info[i][1], prData.bursts_info[i][2]);
    }

    return 0;
}