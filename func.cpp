#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int setrefreshrate(double *current_rate)
{
    Display *display = XOpenDisplay(NULL);
    Window default_root_window = XDefaultRootWindow(display);

    XRRScreenResources *screen_resources = XRRGetScreenResources(display, default_root_window);

    RRMode active_mode_id = 0;
    for (int i = 0; i < screen_resources->ncrtc; ++i)
    {
        XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(display, screen_resources, screen_resources->crtcs[i]);
        // If None, then is not displaying the screen contents
        if (crtc_info->mode != None)
        {
            active_mode_id = crtc_info->mode;
        }
    }

    double active_rate = 0;
    for (int i = 0; i < screen_resources->nmode; ++i)
    {
        XRRModeInfo mode_info = screen_resources->modes[i];
        if (mode_info.id == active_mode_id)
        {
            active_rate = (double)mode_info.dotClock / ((double)mode_info.hTotal * (double)mode_info.vTotal);
        }
    }

    *current_rate = active_rate;
    return 0;
}

void program(int cols, int lines, double refreshRate, string characters, int *occupiedcols, int *occupiedlines, string color)
{
    char matrix[lines][cols];
    int i;
    int j;
    char c;

    for (i = 0; i < lines; i++)
    {
        for (j = 0; j < cols; j++)
        {
            matrix[i][j] = ' ';
        }
    }
    while (true)
    {

        for (i = lines - 1; i > 0; i--)
        {
            for (j = 0; j < cols; j++)
            {
                matrix[i][j] = matrix[i - 1][j];
            }
        }

        for (j = 0; j < cols; j++)
        {
            occupiedlines[0] = 1;
            occupiedcols[j] = 0;

            if (rand() % 10 == 1)
            {
                c = characters[rand() % characters.size()];
                matrix[0][j] = c;
                occupiedcols[j] = 1;
            }
            else
            {
                matrix[0][j] = ' ';
            }
        }

        cout << color;
        for (i = 0; i < lines; i++)
        {
            for (j = 0; j < cols; j++)
            {
                cout << matrix[i][j];
            }
            cout << endl;
        }
        cout << "\033[0m";

        usleep(static_cast<useconds_t>(1000000 / refreshRate));
        system("clear");
    }
}

string getBrightColorCode(const string &color_choice)
{
    string code;

    if (color_choice == "grey")
    {
        code = "\033[38;5;8m";
    }
    else if (color_choice == "red")
    {
        code = "\033[38;5;9m";
    }
    else if (color_choice == "green")
    {
        code = "\033[38;5;10m";
    }
    else if (color_choice == "yellow")
    {
        code = "\033[38;5;11m";
    }
    else if (color_choice == "blue")
    {
        code = "\033[38;5;12m";
    }
    else if (color_choice == "magenta")
    {
        code = "\033[38;5;13m";
    }
    else if (color_choice == "cyan")
    {
        code = "\033[38;5;14m";
    }
    else if (color_choice == "white")
    {
        code = "\033[38;5;15m";
    }
    else
    {
        // Default to bright white
        code = "invalid";
    }

    return code;
}

void helpmsg()
{
    cout << "USAGE\n";
    cout << "  idleterm [-c COLOR] [-s SPEED]\n";
    cout << "OPTIONAL ARGUMENTS\n";
    cout << "  -c      Color: red, green, yellow, blue, magenta [default], cyan, white\n";
    cout << "  -s      Speed: from 1 to 100 [default], in percentage\n";
}