#ifndef _SCREEN_H__
#define _SCREEN_H__

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "screen.h"

void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void setEcho(int enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty); // Get current settings

    if (!enable) {
        tty.c_lflag &= ~ECHO;      // Turn off ECHO bit
    } else {
        tty.c_lflag |= ECHO;       // Turn on ECHO bit
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &tty); // Apply settings immediately
}

void enableRawMode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    
    // ICANON turns off buffered line editing
    // ECHO turns off the display of the typed character
    tty.c_lflag &= ~(ICANON | ECHO);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void hideCursor() {
    printf("\033[?25l");
}

void showCursor() {
    printf("\033[?25h");
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void disableRawMode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void clearScreen() {
    printf("\033[2J\033[H");
}

void drawCharAt(int x, int y, char c, COLOURS color) {
    printf("\033[%d;%dH\033[%dm%c\033[0m", y, x, color, c);
    
    fflush(stdout);
}

#endif
