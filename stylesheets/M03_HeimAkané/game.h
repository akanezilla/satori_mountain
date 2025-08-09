#ifndef GAME_H
#define GAME_H

#include "sprites.h"

void initGame();
void reInitGame();
void initPlayer();
void reInitPlayer();
void initKorok1();
void initKorok2();
void initSpiritOrb();
void initChest();
void initArmor();
void initMarker();
void initStamina();
void reinitStamina();
void initTree();
void updateGame();
void updatePlayer();
void updateKorok1();
void updateKorok2();
void updateSpiritOrb();
void updateChests();
void updateArmor();
void updateMarker();
void updateStamina();
void drawGame();
void drawPlayer();
void drawKorok1();
void drawKorok2();
void drawSpiritOrb();
void drawChests();
void drawArmor();
void drawMarker();
void drawStamina();
void drawTree();
void hideTrees();

extern enum DIRECTION {DOWN, RIGHT, UP, LEFT} direction;
enum KOROKDIRECTION {KDOWN, KUP} korokDirection;

int chestOpened;
extern int hasArmor;
extern int spiritOrbCount;
int cooldown;
int visible;

#define MAPHEIGHT 512
#define MAPWIDTH 512

SPRITE player;
SPRITE korok1;
SPRITE korok2;
SPRITE spiritOrb1;
SPRITE spiritOrb2;
SPRITE spiritOrb3;
SPRITE chest;
SPRITE armor;
SPRITE marker;
SPRITE staminaHold1;
SPRITE staminaHold2;
SPRITE staminaHold3;
SPRITE stamina1;
SPRITE stamina2;
SPRITE stamina3;

extern int vOff;
extern int hOff;

#endif