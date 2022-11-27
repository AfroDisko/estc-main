#ifndef CLI_H
#define CLI_H

typedef enum
{
    WriteCallbackNone,
    WriteCallbackEcho
} WriteCallback;

void cliSetup(void);

#endif
