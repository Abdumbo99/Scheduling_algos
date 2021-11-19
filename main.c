#include <stdio.h>
#define MAXSIZE 1000
#define MAX_BURST_SIZE 400
#define MIN_BURST_SIZE 5

#define DATA_NUMBER 3       //2nd dimension of bursts
#define QUEUE_DATA_NUMBER 4 //2nd dimension of queue
//indices
#define BURST_NUMBER_INDEX 0
#define ARRIVAL_TIME_INDEX 1
#define BURST_LENGTH_INDEX 2
#define TIME_TURNAROUND_INDEX 3

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
    int finished;
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
void reset_memory(ProcessesQueue *processesQueue){
    for (int i =0; i < MAXSIZE; i++){
        for (int j = 0; j < QUEUE_DATA_NUMBER; j++) {
            processesQueue->bursts_enqueued[i][j] = 0;
        }
    }
}
void enqueue_process(ProcessesQueue *processesQueue,const int arrivalTime ,const int burstLength)
{
    processesQueue->bursts_enqueued[processesQueue->count][BURST_NUMBER_INDEX] = processesQueue->count;
    processesQueue->bursts_enqueued[processesQueue->count][ARRIVAL_TIME_INDEX] = arrivalTime;
    processesQueue->bursts_enqueued[processesQueue->count][BURST_LENGTH_INDEX] = burstLength;
    processesQueue->count++;

}

int consume_from_queue(ProcessesQueue *processesQueue, int index)
{
    processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX]--;
    if (processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX] == 0)
    {
        processesQueue->bursts_enqueued[index][TIME_TURNAROUND_INDEX]++;
        processesQueue->finished++;
        return -1;
    }
    return 0;
}

void update_turnaround_time(ProcessesQueue *processesQueue)
{
    for (int i = 0; i < processesQueue->count; i++)
    {
        if (processesQueue->bursts_enqueued[i][BURST_LENGTH_INDEX] > 0)
        {
            processesQueue->bursts_enqueued[i][TIME_TURNAROUND_INDEX]++;
        }
    }
}
// in use , left time, and we have to increment counter for all in use processes
double turnaround_time_FCFS(const int processDataArray[MAXSIZE][DATA_NUMBER], const int size)
{
    printf("FCFS\n ");

    ProcessesQueue pQueue;
    reset_memory(&pQueue);

    pQueue.count = 0;
    pQueue.finished = 0;
    int sourceCurrentIndex = 0;
    int queueCurrentIndex = 0;
    int timer = 0;
    while (size > pQueue.finished)
    {
        //first add
        while (timer == processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX] && sourceCurrentIndex < size)
        {
            enqueue_process(&pQueue, processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX],
                            processDataArray[sourceCurrentIndex][BURST_LENGTH_INDEX]);
            sourceCurrentIndex++;
        }
        //second consume
        if (consume_from_queue(&pQueue, queueCurrentIndex) == -1)
        {
            queueCurrentIndex++;
        }
        update_turnaround_time(&pQueue);
        timer++;
    }
    double totalTurnaround = 0.0;
    for (int i = 0; i < pQueue.count; i++){
        printf("turnaround tine for this burst is %d \n", pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX] );
        totalTurnaround += pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX];
    }
    return totalTurnaround / (double) pQueue.count;
}

double turnaround_time_SJF(int processDataArray[MAXSIZE][DATA_NUMBER], int count)
{
    printf("\n\n========SJF=======\n\n");
    double turnaroundAvg = 0, turnaroundTotalTime = 0;
    double passedTime = 0;
    float burstTime = 0;
    int min;
    int minIndex = 1;
    for (int i = 0; i < count; i++)
    {
        burstTime = burstTime + processDataArray[i][BURST_LENGTH_INDEX];

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

                temp = processDataArray[minIndex][BURST_NUMBER_INDEX];
                processDataArray[minIndex][BURST_NUMBER_INDEX] = processDataArray[j][BURST_NUMBER_INDEX];
                processDataArray[j][BURST_NUMBER_INDEX] = temp;
            }
        }
        minIndex++;
    }
    for (int i = 0; i < count; i++)
    {
        printf("burst index is %d and arrival time %d and length is %d \n", processDataArray[i][BURST_NUMBER_INDEX],
               processDataArray[i][ARRIVAL_TIME_INDEX], processDataArray[i][BURST_LENGTH_INDEX]);
        passedTime = passedTime + processDataArray[i][BURST_LENGTH_INDEX];
        double turaroundTime = passedTime - processDataArray[i][ARRIVAL_TIME_INDEX];
        turnaroundTotalTime = turnaroundTotalTime + turaroundTime;
    }
    turnaroundAvg = turnaroundTotalTime / count;
    printf("Avg turnaround: %lf\n", turnaroundAvg);
    return turnaroundAvg;
}

double turnaround_time_SRTF(int processDataArray[MAXSIZE][DATA_NUMBER], int count)
{
    printf("\n\n========SRJF=======\n\n");
    double turnaroundAvg = 0, turnaroundTotalTime = 0, turnaroundTime = 0;
    float burstTime = 0;
    int index = 1;
    int complate = 0;
    int passedTime = 0;

    processDataArray[count + 1][BURST_LENGTH_INDEX] = MAX_BURST_SIZE; // create a burst with max burst size
    while (complate != count)
    {
        index = count + 1; // set index to max burst

        // pick the burst with minin burst lenght from the arrivied ones
        for (int i = 0; i < count; i++)
        {
            if (processDataArray[i][BURST_LENGTH_INDEX] < processDataArray[index][BURST_LENGTH_INDEX] &&
            passedTime >= processDataArray[i][ARRIVAL_TIME_INDEX] && processDataArray[i][BURST_LENGTH_INDEX] > 0)
            {
                index = i;
            }
        }
        // process only one time unit
        processDataArray[index][BURST_LENGTH_INDEX]--;

        // a burst is end it add to compete list and calculate turnaround time
        if (processDataArray[index][BURST_LENGTH_INDEX] == 0)
        {
            complate++;
            turnaroundTime = passedTime + 1 - processDataArray[index][ARRIVAL_TIME_INDEX];
            turnaroundTotalTime += turnaroundTime;
        }
        passedTime++;
    }

    turnaroundAvg = turnaroundTotalTime / count;
    printf("Avg turnaround: %lf\n", turnaroundAvg);
    return turnaroundAvg;
}

double turnaround_time_RR(const int processDataArray[MAXSIZE][DATA_NUMBER], const int size, int q)
{
    printf("RR\n");

    ProcessesQueue pQueue;
    reset_memory(&pQueue);
    pQueue.count = 0;
    pQueue.finished = 0;
    int sourceCurrentIndex = 0;
    int queueCurrentIndex = 0;
    int processCurrentIndex = 0;
    int timer = 0;
    int pTimer = 0;
    while (size > pQueue.finished) //last element reached but not done
    {
        //first add
        while (timer == processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX] && sourceCurrentIndex < size)
        {
            enqueue_process(&pQueue, processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX],
                            processDataArray[sourceCurrentIndex][BURST_LENGTH_INDEX]);
            printf("added %d at %d and %d\n",sourceCurrentIndex, timer, pTimer );

            sourceCurrentIndex++;
        }
        if (pTimer == q ) // go to next job
        {
            printf("switched %d at %d and %d\n",queueCurrentIndex, timer, pTimer );
            printf("switched %d \n", pQueue.bursts_enqueued[queueCurrentIndex][BURST_LENGTH_INDEX]);

            queueCurrentIndex = (queueCurrentIndex + 1) % pQueue.count;
            while  (pQueue.bursts_enqueued[queueCurrentIndex][BURST_LENGTH_INDEX] == 0)
            {
                queueCurrentIndex = (queueCurrentIndex + 1) % pQueue.count;
            }
            pTimer = 0;

        }
        //second consume
        if (consume_from_queue(&pQueue, queueCurrentIndex) == -1)
        {
            printf("consumed %d at %d and %d\n",queueCurrentIndex, timer, pTimer );
            pTimer = 0;
            queueCurrentIndex = (queueCurrentIndex + 1) % pQueue.count;
            int limit = 0;
            while  (pQueue.bursts_enqueued[queueCurrentIndex][BURST_LENGTH_INDEX] == 0 && limit < pQueue.count)
            {
                limit++;
                queueCurrentIndex = (queueCurrentIndex + 1) % pQueue.count;
            }
        }
        else
        {
            pTimer++;
        }
        update_turnaround_time(&pQueue);
        timer++;
    }
    double totalTurnaround = 0.0;
    for (int i = 0; i < pQueue.count; i++){
        printf("turnaround tine for this burst is  %d\n", pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX] );
        totalTurnaround += pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX];
    }

    return totalTurnaround / (double) pQueue.count;
}

int main()
{
    ProcessesData prData;
    prData.count = 0;

    FILE *fp;
    char *name = "calc.txt";
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

   ProcessesQueue qData;
    qData.count = 0;
  printf("average turnaround for RR is %.2lf\n", turnaround_time_RR(prData.bursts_info, prData.count,10));
  printf("average turnaround for RR is %.2lf\n", turnaround_time_RR(prData.bursts_info, prData.count,5));
  printf("average turnaround for FCFS is %.2lf\n", turnaround_time_FCFS(prData.bursts_info, prData.count));

    return 0;
}