#ifndef CMD_H
#define CMD_H

static const char gCmdResponseUnknownCmd[] = "Unknown command (enter 'help' for help)\r\n";

static const char gCmdHelp[]               = "help";

static const char gCmdResponseHelp[]       = "Available commands:\r\n"
                                             "help            -- prints this message\r\n"
                                             "rgb <r> <g> <b> -- sets LED2 state according to RGB input (0 <= <i> <= 255)\r\n"
                                             "hsv <h> <s> <v> -- sets LED2 state according to HSV input (0 <= <i> <= 255)\r\n";

static const char gCmdRgb[]                = "rgb";

static const char gCmdHsv[]                = "hsv";

#endif
