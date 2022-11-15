#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef enum
{
    Dummy,
    SW1PressedSingle,
    SW1PressedDouble,
    SW1Released
} Event;

bool queueIsFull(void);

bool queueIsEmpty(void);

void queueShiftEvents(void);

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
