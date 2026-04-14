#ifndef SCREEN_H
#define SCREEN_H

typedef enum COLOURS_ENUM {
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    RESET = 0,
    BG_BLUE = 44
} COLOURS;

void sleep_ms(int milliseconds);
void setEcho(int enable);
void clearScreen();
void drawCharAt(int x,int y,char c ,COLOURS colorCode);
int kbhit();
void enableRawMode();
void disableRawMode();
void hideCursor();
void showCursor();
void drawBorders();

#endif
