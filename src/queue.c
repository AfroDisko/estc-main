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

void queueShiftEvents(void)
{
    for(unsigned int idx = 0; idx < gIdxR - gIdxF; ++idx)
        gQueue[idx] = gQueue[gIdxF + idx];
    gIdxR -= gIdxF;
    gIdxF = 0;
}

void queueEventEnqueue(Event event)
{
    if(queueIsFull())
        queueShiftEvents();

    if(queueIsFull())
        return;

    gQueue[gIdxR++] = event;
}

Event queueEventDequeue(void)
{
    if(queueIsEmpty())
        return Dummy;

    return gQueue[gIdxF++];
}
