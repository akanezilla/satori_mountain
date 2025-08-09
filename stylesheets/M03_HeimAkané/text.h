#ifndef TEXT_H
#define TEXT_H

#include "gba.h"

void initTextbox();
void drawTextbox();
void showTextbox();
void hideTextbox();

extern int textActivated;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int active;
    u8 oamIndex;
} TEXTBOX;

#define TEXTBOXCOUNT 4

TEXTBOX textbox[TEXTBOXCOUNT];

#endif