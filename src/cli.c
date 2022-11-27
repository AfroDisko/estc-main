#include <string.h>
#include <stdlib.h>

#include "app_usbd_cdc_acm.h"

#include "main.h"
#include "leds.h"
#include "cmd.h"
#include "cli.h"

#define BUFFER_SIZE_ECHO 1
#define BUFFER_SIZE_MAIN 32

#define COMMAND_WORD_NUM_MAX 4
#define COMMAND_WORD_LEN_MAX 8

static WriteCallback gWriteCallback = WriteCallbackNone;

static char gBufferEcho[BUFFER_SIZE_ECHO];
static char gBufferMain[BUFFER_SIZE_MAIN];

static char gCommand[COMMAND_WORD_NUM_MAX][COMMAND_WORD_LEN_MAX];

static void usbdHandler(app_usbd_class_inst_t const* p_inst, app_usbd_cdc_acm_user_event_t event);

APP_USBD_CDC_ACM_GLOBAL_DEF(usbdInstance, usbdHandler,
                            CDC_ACM_COMM_INTERFACE, CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN, CDC_ACM_DATA_EPIN, CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_NONE);

static void cliBufferReset(void)
{
    gBufferEcho[0] = '\0';
    gBufferMain[0] = '\0';
    for(uint8_t idx = 0; idx < COMMAND_WORD_NUM_MAX; ++idx)
        gCommand[idx][0] = '\0';
}

void cliSetup(void)
{
    cliBufferReset();
    app_usbd_class_append(app_usbd_cdc_acm_class_inst_get(&usbdInstance));
}

static void cliBufferAppend(void)
{
    if(strlen(gBufferMain) + 1 < BUFFER_SIZE_MAIN)
        strcat(gBufferMain, gBufferEcho);
}

static void cliBufferParse(void)
{
    char*   token = strtok(gBufferMain, " ");
    uint8_t tokenIdx = 0;
    while(token != NULL && tokenIdx < COMMAND_WORD_NUM_MAX)
    {
        strcpy(gCommand[tokenIdx++], token);
        token = strtok(NULL, " ");
    }
}

static void cliExecute(void)
{
    if(strcmp(gCommand[0], gCmdHelp) == 0)
    {
        app_usbd_cdc_acm_write(&usbdInstance, gCmdResponseHelp, sizeof(gCmdResponseHelp));
        return;
    }

    if(strcmp(gCommand[0], gCmdRgb) == 0)
    {
        ColorHSV hsv = rgb2hsv((ColorRGB){atoi(gCommand[1]),
                                          atoi(gCommand[2]),
                                          atoi(gCommand[3])});

        Context* ctx = mainGetCtx();
        ctx->color = hsv;

        ledsSetLED2StateHSV(hsv);

        return;
    }

    if(strcmp(gCommand[0], gCmdHsv) == 0)
    {
        Context* ctx = mainGetCtx();
        (ctx->color).h = atoi(gCommand[1]);
        (ctx->color).s = atoi(gCommand[2]);
        (ctx->color).v = atoi(gCommand[3]);

        ledsSetLED2StateHSV(ctx->color);

        return;
    }

    if(strcmp(gCommand[0], "") != 0)
        app_usbd_cdc_acm_write(&usbdInstance, gCmdResponseUnknownCmd, sizeof(gCmdResponseUnknownCmd));
}

static void cliBufferProcess(void)
{
    cliBufferParse();
    cliExecute();
    cliBufferReset();
}

static void usbdHandler(app_usbd_class_inst_t const* p_inst, app_usbd_cdc_acm_user_event_t event)
{
    switch(event)
    {
    case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        app_usbd_cdc_acm_read(&usbdInstance, gBufferEcho, BUFFER_SIZE_ECHO);
        break;

    case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
        break;

    case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
        // looks like a very bad decision, but will suffice for now
        switch(gWriteCallback)
        {
        case WriteCallbackEcho:
            gWriteCallback = WriteCallbackNone;
            goto returnToEventEcho;
            break;

        default:
            break;
        }

    case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        do
        {
            if(gBufferEcho[0] == '\r' || gBufferEcho[0] == '\n')
            {
                app_usbd_cdc_acm_write(&usbdInstance, "\r\n", 2);
                cliBufferProcess();
            }
            else
            {
                app_usbd_cdc_acm_write(&usbdInstance, gBufferEcho, BUFFER_SIZE_ECHO);
                cliBufferAppend();
            }
            gWriteCallback = WriteCallbackEcho;
            return;
            returnToEventEcho:
        }
        while(app_usbd_cdc_acm_read(&usbdInstance, gBufferEcho, BUFFER_SIZE_ECHO) == NRF_SUCCESS);
        break;

    default:
        break;
    }
}
