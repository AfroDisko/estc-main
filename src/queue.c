#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

#define QUEUE_SIZE UINT8_MAX

static volatile Event gQueue[QUEUE_SIZE];

static volatile uint8_t gIdxF = 0;
static volatile uint8_t gIdxR = 0;

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
    for(uint8_t idx = 0; idx < gIdxR - gIdxF; ++idx)
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
