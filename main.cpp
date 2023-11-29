#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <iostream>

#include "func.h"

using namespace std;
int main(int argc, char *argv[])
{
    string characters;
    characters = "!@#$%&*<>()";
    int *terminaltype = (int *)malloc(0);

    int *cols = (int *)malloc(0);
    int *lines = (int *)malloc(0);
    *cols = 80;
    *lines = 24;
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    *cols = ts.ts_cols;
    *lines = ts.ts_lines;
    *terminaltype = 0;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    *cols = ts.ws_col;
    *lines = ts.ws_row;
    *terminaltype = 1;
#endif

    double *refreshrate = (double *)malloc(0.0);
    *refreshrate = 60.0;
    if (terminaltype)
    {
        setrefreshrate(refreshrate);
    }
    int speed = 50;
    string color_choice = "";
    for (int i = 1; i < argc; ++i)
    {
        if (string(argv[i]) == "-s" && i + 1 < argc)
        {
            speed = stoi(argv[i + 1]);
            if (speed > 100 || speed < 1)
            {
                printf("Invalid speed (-h for help.)");
                return 1;
            }
        }
        else if (string(argv[i]) == "-c" && i + 1 < argc)
        {
            color_choice = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "-l" && i + 1 < argc)
        {
            characters = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "-h")
        {
            helpmsg();
            return 0;
        }
    }

    if (!color_choice.empty())
    {
        color_choice = getBrightColorCode(color_choice);
    }
    else
    {
        color_choice = getBrightColorCode("magenta");
    }
    if (color_choice == "invalid")
    {
        printf("Invalid color (-h for help.)");
        return 2;
    }

    *refreshrate = (double)speed * (*refreshrate) * 0.01;
    int occupiedcols[*cols];
    int occupiedlines[*lines];
    program(*cols, *lines, *refreshrate, characters, occupiedcols, occupiedlines, color_choice);

    free(lines);
    free(cols);
    free(terminaltype);
    free(refreshrate);
    return 0;
}