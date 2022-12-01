#ifndef QUEUE_H
#define QUEUE_H

#include "ledsutils.h"

typedef enum
{
    EventNone,
    EventSwitchPressed,
    EventSwitchPressedContinuous,
    EventSwitchReleased,
    EventCliChangeColorRGB,
    EventCliChangeColorHSV
} EventType;

typedef union
{
    uint8_t  num;
    ColorRGB rgb;
    ColorHSV hsv;
} EventData;

typedef struct
{
    EventType type;
    EventData data;
} Event;

void queueEventEnqueue(Event event);

Event queueEventDequeue(void);

#endif
