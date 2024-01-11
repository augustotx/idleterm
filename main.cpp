#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <ncurses.h>
#include "func.h"

using namespace std;
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.UTF-8");
    string characters = "!@#$%&*<>()";
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
    string color_ANSI;
    int color_pair_int;
    if (!color_choice.empty())
    {
        color_ANSI = getBrightColorCode(color_choice);
    }
    else
    {
        color_ANSI = getBrightColorCode("magenta");
        color_choice = "magenta";
    }
    if (color_ANSI == "invalid")
    {
        printf("Invalid color (-h for help.)");
        return 2;
    }
    color_pair_int = getBrightColorPair(color_choice);
    speed *= 2;
    int *program_running = (int *)malloc(sizeof(int));
    signal(SIGINT, signalHandler);
    program(speed, characters, color_pair_int);
    free(program_running);
    return 0;
}