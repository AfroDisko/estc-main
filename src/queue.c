#include "queue.h"

#define QUEUE_SIZE 100

volatile unsigned int gIdxF = 0;
volatile unsigned int gIdxR = 0;

volatile Event gQueue[QUEUE_SIZE];

void queueShiftEvents(void)
{
    for(unsigned int idx = 0; idx < gIdxR - gIdxF; ++idx)
        gQueue[idx] = gQueue[gIdxF + idx];
    gIdxR -= gIdxF;
    gIdxF = 0;
}

bool queueIsFull(void)
{
    return gIdxR == QUEUE_SIZE;
}

bool queueIsEmpty(void)
{
    return gIdxF == gIdxR;
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
        return DummyEvent;

    return gQueue[gIdxF++];
}
