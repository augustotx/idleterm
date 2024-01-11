#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <ncurses.h>
#include <vector>
using namespace std;

int getttysize(int *lines, int *cols)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *lines = w.ws_row;
    *cols = w.ws_col;
    return 0;
}

int getBrightColorPair(const string &color_choice)
{
    if (color_choice == "grey")
    {
        return 8;
    }
    else if (color_choice == "red")
    {
        return 9;
    }
    else if (color_choice == "green")
    {
        return 10;
    }
    else if (color_choice == "yellow")
    {
        return 11;
    }
    else if (color_choice == "blue")
    {
        return 12;
    }
    else if (color_choice == "magenta")
    {
        return 13;
    }
    else if (color_choice == "cyan")
    {
        return 14;
    }
    else if (color_choice == "white")
    {
        return 15;
    }
    else
    {
        return 15;
    }
}

int lines, cols, program_running;

void handle_winch(int sig)
{
    program_running = 0;
}

void program(int speed, string characters, int color)
{
    getttysize(&lines, &cols);
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    start_color();
    int default_bg_color;
    if (use_default_colors() == OK)
    {
        default_bg_color = -1;
    }
    else
    {
        default_bg_color = COLOR_BLACK;
    }
    init_pair(getBrightColorPair("grey"), COLOR_BLACK, default_bg_color);
    init_pair(getBrightColorPair("red"), COLOR_RED, default_bg_color);
    init_pair(getBrightColorPair("green"), COLOR_GREEN, default_bg_color);
    init_pair(getBrightColorPair("yellow"), COLOR_YELLOW, default_bg_color);
    init_pair(getBrightColorPair("blue"), COLOR_BLUE, default_bg_color);
    init_pair(getBrightColorPair("magenta"), COLOR_MAGENTA, default_bg_color);
    init_pair(getBrightColorPair("cyan"), COLOR_CYAN, default_bg_color);
    init_pair(getBrightColorPair("white"), COLOR_WHITE, default_bg_color);

    

    srand(time(0));
    vector<vector<char>> matrix(lines, vector<char>(cols, ' '));
    int i;
    int j;
    char c;

    for (i = lines - 1; i > 0; i--)
    {
        for (j = 0; j < cols; j++)
        {
            matrix[i][j] = ' ';
        }
    }
    
    int key;
    program_running = 1;
    signal(SIGWINCH, handle_winch);
    while ((key = getch()) != 'q')
    {
        if (program_running == 0)
        {
            endwin();
            program(speed, characters, color);
            break;
        }
        getttysize(&lines, &cols);
        matrix.resize(lines, std::vector<char>(cols, ' '));

        switch (key)
        {
        case 'h':
        case KEY_LEFT:
            if (speed >=6)
            {
                speed-=5;
            }
            break;
        case 'l':
        case KEY_RIGHT:
            if (speed <=95)
            {
                speed+=5;
            }
            break;
        case 'j':
        case KEY_DOWN:
            if (color > getBrightColorPair("grey"))
            {
                color--;
            }
            
            break;
        case 'k':
        case KEY_UP:
            if (color < getBrightColorPair("white"))
            {
                color++;
            }
            
            break;
        default:
            // Handle other keys if needed
            break;
        }
        for (i = lines - 1; i > 0; i--)
        {
            for (j = 0; j < cols; j++)
            {
                matrix[i][j] = matrix[i - 1][j];
            }
        }

        for (j = 0; j < cols; j++)
        {

            if (rand() % 10 == 1)
            {
                c = characters[rand() % characters.size()];
                matrix[0][j] = c;
            }
            else
            {
                matrix[0][j] = ' ';
            }
        }

        attron(COLOR_PAIR(color));
        for (i = 0; i < lines; i++)
        {
            for (j = 0; j < cols; j++)
            {
                printw("%c", matrix[i][j]);
            }
        }
        attroff(COLOR_PAIR(color));

        refresh();
        usleep(static_cast<useconds_t>(1000000 / speed));
        clear();
    }

    endwin();
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
    cout << "  speed can be changed with left/right arrow keys\n";
    cout << "OPTIONAL ARGUMENTS\n";
    cout << "  -c      Color: red, green, yellow, blue, magenta [default], cyan, white\n";
    cout << "  -s      Speed: from 1 to 100 [default], in percentage\n";
    cout << "  -l      Character list: e.g. \"!@#$%&*<>()\" [default]";
}

void signalHandler(int signal)
{
    switch (signal)
    {
    case SIGINT:
        cout << "\033[0m";
        endwin();
        cout << "\033[2J\033[1;1H";
        exit(signal);
        break;
    default:
        break;
    }
}