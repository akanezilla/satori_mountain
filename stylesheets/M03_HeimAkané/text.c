#include "text.h"
#include "mode0.h"
#include "sprites.h"

void initTextbox();
void drawTextbox();
void showTextbox();
void hideTextbox();

int textActivated;

TEXTBOX textbox[TEXTBOXCOUNT];

void initTextbox() {
    for (int i = 0; i < TEXTBOXCOUNT; i++) {
        textbox[i].width = 64;
        textbox[i].height = 64;
        textbox[i].active = 0;
    }
    textbox[0].x = 0;
    textbox[0].y = 96;
    textbox[0].oamIndex = 20;

    textbox[1].x = 64;
    textbox[1].y = 96;
    textbox[1].oamIndex = 21;

    textbox[2].x = 128;
    textbox[2].y = 96;
    textbox[2].oamIndex = 22;

    textbox[3].x = 192;
    textbox[3].y = 96;
    textbox[3].oamIndex = 23;
}

void drawTextbox() {
    for (int i = 0; i < TEXTBOXCOUNT; i++) {
        if (textbox[i].active) {
            shadowOAM[textbox[i].oamIndex].attr0 = ATTR0_Y(textbox[i].y) | ATTR0_REGULAR | ATTR0_SQUARE;
            shadowOAM[textbox[i].oamIndex].attr1 = ATTR1_X(textbox[i].x) | ATTR1_LARGE;
            shadowOAM[textbox[i].oamIndex].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(16, 12) | ATTR2_PRIORITY(1);
        } else {
            shadowOAM[textbox[i].oamIndex].attr0 = ATTR0_HIDE;
        }
    }
}

void showTextbox() {
    for (int i = 0; i < TEXTBOXCOUNT; i++) {
        textbox[i].active = 1;
    }
}

void hideTextbox() {
    for (int i = 0; i < TEXTBOXCOUNT; i++) {
        textbox[i].active = 0;
    }
}
