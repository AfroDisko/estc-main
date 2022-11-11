#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdbool.h>

typedef enum
{
    DummyEvent,
    SW1PressedSingle,
    SW1PressedDouble,
    SW1Released
} Event;

void shiftEvents(void);

bool eventQueueIsFull(void);

bool eventQueueIsEmpty(void);

void eventQueueEventEnqueue(Event event);

Event eventQueueEventDequeue(void);

#endif
