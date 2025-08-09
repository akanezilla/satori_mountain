#ifndef TRIAL_H
#define TRIAL_H

#include "sprites.h"

void initTrial();
void initPlayerTrial();
void initLotMTrial();
void initBar();
void initNumbers();
void initHeartsWin();
void initWinWord();
void updateTrial();
void updatePlayerTrial();
void updateLotM();
void updateBar();
void updateNumbers();
void updateHeartsWin();
void drawTrial();
void drawPlayerTrial();
void drawLotMTrial();
void drawBar();
void drawNumbers();
void drawHearts();
void drawWinWord();

inline unsigned char colorAt3(int x, int y);

SPRITE player;
SPRITE lotm;
SPRITE staminaBar;
SPRITE piece1;
SPRITE piece2;
SPRITE piece3;
SPRITE piece4;
SPRITE piece5;
SPRITE numbers;
SPRITE hearts;
SPRITE hearts2;
SPRITE hearts3;
SPRITE winWord;

int staminaCount;

OBJ_ATTR shadowOAM[128];

#define MAPHEIGHT 256
#define MAPWIDTH 256

//color
#define BLACK   RGB(0, 0, 0)
#define YELLOW    RGB(31, 31, 15)

extern int vOff;
extern int hOff;

#endif
