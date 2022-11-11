#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef enum
{
    DummyEvent,
    SW1PressedSingle,
    SW1PressedDouble,
    SW1Released
} Event;

void queueShiftEvents(void);

bool queueIsFull(void);

bool queueIsEmpty(void);

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
