#ifndef QUEUE_H
#define QUEUE_H

typedef enum
{
    EventDummy,
    EventSwitchPressedSingle,
    EventSwitchPressedDouble
} Event;

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
