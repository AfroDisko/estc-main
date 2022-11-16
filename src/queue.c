#include <stdbool.h>

#include "queue.h"

#define QUEUE_SIZE 100

volatile Event gQueue[QUEUE_SIZE];

volatile unsigned int gIdxF = 0;
volatile unsigned int gIdxR = 0;

bool queueIsFull(void)
{
    return gIdxR == QUEUE_SIZE;
}

bool queueIsEmpty(void)
{
    return gIdxF == gIdxR;
}

void queueShift(void)
{
    for(unsigned int idx = 0; idx < gIdxR - gIdxF; ++idx)
        gQueue[idx] = gQueue[gIdxF + idx];
    gIdxR -= gIdxF;
    gIdxF -= gIdxF;
}

void queueEventEnqueue(Event event)
{
    if(queueIsFull())
        queueShift();

    if(queueIsFull())
        return;

    gQueue[gIdxR++] = event;
}

Event queueEventDequeue(void)
{
    if(queueIsEmpty())
    {
        queueShift();
        return EventDummy;
    }

    return gQueue[gIdxF++];
}
