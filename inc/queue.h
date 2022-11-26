#ifndef QUEUE_H
#define QUEUE_H

typedef enum
{
    EventDummy,
    EventSwitchPressedSingle,
    EventSwitchPressedDouble,
    EventSwitchPressedContin,
    EventSwitchReleased
} Event;

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
