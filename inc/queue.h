#ifndef QUEUE_H
#define QUEUE_H

typedef enum
{
    EventDummy,
    EventSwitchPressedContin,
    EventSwitchPressedSingle,
    EventSwitchPressedDouble,
    EventSwitchReleased
} Event;

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
