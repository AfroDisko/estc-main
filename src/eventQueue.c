#include "eventQueue.h"

#define EVENT_QUEUE_SIZE 100

volatile unsigned int gEventIdxF = 0;
volatile unsigned int gEventIdxR = 0;

volatile Event gEventQueue[EVENT_QUEUE_SIZE];

void shiftEvents(void)
{
    for(unsigned int idx = 0; idx < gEventIdxR - gEventIdxF; ++idx)
        gEventQueue[idx] = gEventQueue[gEventIdxF + idx];
    gEventIdxR -= gEventIdxF;
    gEventIdxF = 0;
}

bool eventQueueIsFull(void)
{
    return gEventIdxR == EVENT_QUEUE_SIZE;
}

bool eventQueueIsEmpty(void)
{
    return gEventIdxF == gEventIdxR;
}

void eventQueueEventEnqueue(Event event)
{
    if(eventQueueIsFull())
        shiftEvents();

    if(eventQueueIsFull())
        return;
    
    gEventQueue[gEventIdxR++] = event;
}

Event eventQueueEventDequeue(void)
{
    if(eventQueueIsEmpty())
        return DummyEvent;

    return gEventQueue[gEventIdxF++];
}
