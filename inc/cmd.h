#ifndef CMD_H
#define CMD_H

static const char gCmdResponseUnknownCmd[] = "Unknown command (enter 'help' for help)\r\n";

static const char gCmdHelp[]               = "help";

static const char gCmdResponseHelp[]       = "Available commands:\r\n"
                                             "help                             -- prints this message\r\n"
                                             "rgb <r> <g> <b>                  -- sets LED2 state according to RGB input (0 <= <i> <= 255)\r\n"
                                             "hsv <h> <s> <v>                  -- sets LED2 state according to HSV input (0 <= <i> <= 255)\r\n"
                                             "color_add_rgb <r> <g> <b> <mark> -- memorizes LED2 state according to RGB input (0 <= <i> <= 255),\r\n"
                                             "                                    <mark> needs to be a char\r\n"
                                             "color_add_cur <mark>             -- memorizes current LED2 state, <mark> needs to be a char\r\n"
                                             "color_set <mark>                 -- sets LED2 state according to prev. memorized state named <mark>,\r\n"
                                             "                                    <mark> needs to be a char\r\n"
                                             "color_del <mark>                 -- deletes LED2 state named <mark>, <mark> needs to be a char\r\n";

static const char gCmdRgb[]                = "rgb";

static const char gCmdHsv[]                = "hsv";

static const char gCmdColorAddRgb[]        = "color_add_rgb";

static const char gCmdColorAddCur[]        = "color_add_cur";

static const char gCmdColorSet[]           = "color_set";

static const char gCmdResponseNoColor[]    = "There is no color mark like that in memory\r\n";

static const char gCmdColorDel[]           = "color_del";

#endif
