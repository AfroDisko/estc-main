#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef enum
{
    EventDummy,
    EventSwitchPressedSingle,
    EventSwitchPressedDouble,
    EventSwitchPressedTriple,
    EventLEDBlinkCompleted
} Event;

// bool queueIsFull(void);

// bool queueIsEmpty(void);

// void queueShiftEvents(void);

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
