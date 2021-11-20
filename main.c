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
void reset_memory(ProcessesQueue *processesQueue)
{
    for (int i = 0; i < MAXSIZE; i++)
    {
        for (int j = 0; j < QUEUE_DATA_NUMBER; j++)
        {
            processesQueue->bursts_enqueued[i][j] = 0;
        }
    }
}
void enqueue_process(ProcessesQueue *processesQueue, const int arrivalTime, const int burstLength)
{
    processesQueue->bursts_enqueued[processesQueue->count][BURST_NUMBER_INDEX] = processesQueue->count;
    processesQueue->bursts_enqueued[processesQueue->count][ARRIVAL_TIME_INDEX] = arrivalTime;
    processesQueue->bursts_enqueued[processesQueue->count][BURST_LENGTH_INDEX] = burstLength;
    processesQueue->count++;
}

int consume_from_queue(ProcessesQueue *processesQueue, int index)
{
    if (processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX] > 0){
        processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX]--;

        if (processesQueue->bursts_enqueued[index][BURST_LENGTH_INDEX] == 0)
        {
            processesQueue->bursts_enqueued[index][TIME_TURNAROUND_INDEX]++;
            processesQueue->finished++;
            return -1;
        }
        else {
            return 0;
        }
    }
    return -1;
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
        if (pQueue.bursts_enqueued[queueCurrentIndex][BURST_LENGTH_INDEX] == 0 && queueCurrentIndex < pQueue.count - 1){
            queueCurrentIndex++;
        }
        //second consume
        if (consume_from_queue(&pQueue, queueCurrentIndex) == -1)
        {
        }
        update_turnaround_time(&pQueue);
        timer++;
    }
    double totalTurnaround = 0.0;
    for (int i = 0; i < pQueue.count; i++)
    {
        //printf("turnaround tine for this burst is %d \n", pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX]);
        totalTurnaround += pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX];
    }
    return totalTurnaround / (double)pQueue.count;
}

double turnaround_time_SJF(int const processDataArray[MAXSIZE][DATA_NUMBER], int const count)
{
    // temp array
    int dataArray[MAXSIZE][DATA_NUMBER];

    for (int i = 0; i < count; i++)
    {
        dataArray[i][BURST_NUMBER_INDEX] = processDataArray[i][BURST_NUMBER_INDEX];
        dataArray[i][ARRIVAL_TIME_INDEX] = processDataArray[i][ARRIVAL_TIME_INDEX];
        dataArray[i][BURST_LENGTH_INDEX] = processDataArray[i][BURST_LENGTH_INDEX];
    }
    printf("SJF\n");
    double turnaroundAvg = 0, turnaroundTotalTime = 0;
    double passedTime = 0;
    float burstTime = 0;
    int min;
    int minIndex = 1;
    for (int i = 0; i < count; i++)
    {
        // execute burst, increase burst time
        burstTime = burstTime + dataArray[i][BURST_LENGTH_INDEX];

        min = dataArray[minIndex][BURST_LENGTH_INDEX];
        /*
            set min burst length to next burst and check
            if any newly arrived burst has smaller burst time than min burst.
            If there exist such burst it swaps the bursts by swapping the data values.
         */
        for (int j = minIndex; j < count; j++)
        {
            if (burstTime >= dataArray[j][ARRIVAL_TIME_INDEX] && dataArray[j][BURST_LENGTH_INDEX] < min)
            {
                min = dataArray[j][BURST_LENGTH_INDEX];

                // Swap the place of the burst by swapping data
                // swap ARRIVAL_TIME_INDEX
                int temp = dataArray[minIndex][ARRIVAL_TIME_INDEX];
                dataArray[minIndex][ARRIVAL_TIME_INDEX] = dataArray[j][ARRIVAL_TIME_INDEX];
                dataArray[j][ARRIVAL_TIME_INDEX] = temp;
                // swap BURST_LENGTH_INDEX
                temp = dataArray[minIndex][BURST_LENGTH_INDEX];
                dataArray[minIndex][BURST_LENGTH_INDEX] = dataArray[j][BURST_LENGTH_INDEX];
                dataArray[j][BURST_LENGTH_INDEX] = temp;
                // swap BURST_NUMBER_INDEX
                temp = dataArray[minIndex][BURST_NUMBER_INDEX];
                dataArray[minIndex][BURST_NUMBER_INDEX] = dataArray[j][BURST_NUMBER_INDEX];
                dataArray[j][BURST_NUMBER_INDEX] = temp;
            }
        }
        minIndex++;
    }
    for (int i = 0; i < count; i++)
    {
        passedTime = passedTime + dataArray[i][BURST_LENGTH_INDEX];
        double turaroundTime = passedTime - dataArray[i][ARRIVAL_TIME_INDEX];
        turnaroundTotalTime = turnaroundTotalTime + turaroundTime;
    }
    turnaroundAvg = turnaroundTotalTime / count;
    return turnaroundAvg;
}

double turnaround_time_SRTF(int const processDataArray[MAXSIZE][DATA_NUMBER], int const count)
{
    // temp array with same data values as processDataArray
    int dataArray[MAXSIZE][DATA_NUMBER];

    for (int i = 0; i < count; i++)
    {
        dataArray[i][BURST_NUMBER_INDEX] = processDataArray[i][BURST_NUMBER_INDEX];
        dataArray[i][ARRIVAL_TIME_INDEX] = processDataArray[i][ARRIVAL_TIME_INDEX];
        dataArray[i][BURST_LENGTH_INDEX] = processDataArray[i][BURST_LENGTH_INDEX];
    }

    printf("SRJF\n");
    double turnaroundAvg = 0, turnaroundTotalTime = 0, turnaroundTime = 0;
    float burstTime = 0;
    int index = 1;
    int complate = 0;
    int passedTime = 0;

    dataArray[count + 1][BURST_LENGTH_INDEX] = MAX_BURST_SIZE + 1; // create a burst with max burst size
    while (complate != count)
    {
        index = count + 1; // set index to max burst

        // pick the burst with minimum burst length from the arrived ones
        for (int i = 0; i < count; i++)
        {
            if (dataArray[i][BURST_LENGTH_INDEX] < dataArray[index][BURST_LENGTH_INDEX] &&
                passedTime >= dataArray[i][ARRIVAL_TIME_INDEX] && dataArray[i][BURST_LENGTH_INDEX] > 0)
            {
                index = i; // set the minimum burst index
            }
        }
        // process only one time unit
        dataArray[index][BURST_LENGTH_INDEX]--;

        // a burst is end it, add to compete list and calculate turnaround time
        if (dataArray[index][BURST_LENGTH_INDEX] == 0)
        {
            complate++;
            // turnaroundTime calculated by passed time - the time it arrived.
            turnaroundTime = passedTime + 1 - dataArray[index][ARRIVAL_TIME_INDEX];
            turnaroundTotalTime += turnaroundTime;
        }
        passedTime++;
    }

    turnaroundAvg = turnaroundTotalTime / count;
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
    int timer = processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX];
    int pTimer = processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX] % q;
    while (size > pQueue.finished) //last element reached but not done
    {
        //first add
        while (timer == processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX] && sourceCurrentIndex < size)
        {
            enqueue_process(&pQueue, processDataArray[sourceCurrentIndex][ARRIVAL_TIME_INDEX],
                            processDataArray[sourceCurrentIndex][BURST_LENGTH_INDEX]);
            sourceCurrentIndex++;
        }
        // go to next job if necessary
        if (pTimer == q
        || pTimer == 0 && pQueue.bursts_enqueued[queueCurrentIndex][BURST_LENGTH_INDEX] == 0)
        {
            int limit = 0; // in case queue is all finished but some tasks can still arrive later
            queueCurrentIndex = (queueCurrentIndex + 1) % pQueue.count;
            while (pQueue.bursts_enqueued[queueCurrentIndex][BURST_LENGTH_INDEX] == 0 && limit < pQueue.count)
            {
                queueCurrentIndex = (queueCurrentIndex + 1) % pQueue.count;
                limit++;
            }
        }
        //then consume
        if (consume_from_queue(&pQueue, queueCurrentIndex) == -1)
            pTimer = 0;

        else
            pTimer++;

        update_turnaround_time(&pQueue);
        timer++;
    }
    double totalTurnaround = 0.0;
    for (int i = 0; i < pQueue.count; i++)
    {
        printf("turnaround tine for this burst is  %d\n", pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX]);
        totalTurnaround += pQueue.bursts_enqueued[i][TIME_TURNAROUND_INDEX];
    }
    return totalTurnaround / (double)pQueue.count;
}

int main()
{
    ProcessesData prData;
    prData.count = 0;

    FILE *fp;
    char *name = "gaps.txt";
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
    //printf("average turnaround for RR<10> is %.2lf\n", turnaround_time_RR(prData.bursts_info, prData.count, 10));
    printf("average turnaround for RR<5> is %.2lf\n", turnaround_time_RR(prData.bursts_info, prData.count, 50));
    printf("average turnaround for FCFS is %.2lf\n", turnaround_time_FCFS(prData.bursts_info, prData.count));

    printf("average turnaround for SJF is %.2lf\n", turnaround_time_SJF(prData.bursts_info, prData.count));
    printf("average turnaround for SRTF is %.2lf\n", turnaround_time_SRTF(prData.bursts_info, prData.count));
    return 0;
}