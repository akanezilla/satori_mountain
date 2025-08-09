#include "springTrial.h"
#include "sprites.h"
#include "link.h"
#include "mode0.h"
#include "greatFF.h"
#include "digitalSound.h"
#include "text.h"
#include "mode0.h"
#include "interact.h"
#include "yahaha.h"

void initSpring();
void reInitSpring();
void initPlayerSpring();
void reInitPlayerSpring();
void initBlupees();
void initLotM();
void initStaminaSpring();
void initKorokSpring();
void updateSpring();
void updatePlayerSpring();
void updateBlupees();
void updateLotM();
void updateKorokSpring();
void drawSpring();
void drawPlayerSpring();
void drawBlupees();
void drawLotM();
void drawKorokSpring();

void initStamina();
void reinitStamina();
void updateStamina();
void drawStamina();
inline unsigned char colorAt2(int x, int y);

SPRITE player;
SPRITE blupee1;
SPRITE blupee2;
SPRITE blupee3;
SPRITE lotm;
SPRITE korok;
extern SPRITE staminaHold1;
extern SPRITE staminaHold2;
extern SPRITE staminaHold3;
extern SPRITE stamina1;
extern SPRITE stamina2;
extern SPRITE stamina3;

int countdown1;
int countdown2;
int countdown3;
int countdownKorok;
int inConversation;
extern int hasArmor;
extern int spiritOrbCount;

extern enum DIRECTION {DOWN, RIGHT, UP, LEFT} direction;
extern enum KOROKDIRECTION {KDOWN, KUP} korokDirection;

OBJ_ATTR shadowOAM[128];

void initSpring() {
    REG_DISPCTL = MODE(0) | BG_ENABLE(1) | BG_ENABLE(0) | SPRITE_ENABLE;
    REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(8) | BG_SIZE_SMALL | 3;

    DMANow(3, linkTiles, &CHARBLOCK[4], linkTilesLen / 2);
    DMANow(3, linkPal, SPRITE_PAL, 256);

    hOff = 0;
    vOff = 0;
    countdown1 = 60;
    countdown2 = 150;
    countdown3 = 200;
    countdownKorok = 150;
    inConversation = 0;

    initPlayerSpring();
    initBlupees();
    initLotM();
    initSound();
    initKorokSpring();
    reinitStamina();

    setupSoundInterrupts();
    setupSounds();
    playSoundA(greatFF_data, greatFF_length, 1);

    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);
}

void reInitSpring() {
    reInitPlayerSpring();
    reinitStamina();
    initBlupees();
    initLotM();
    initKorokSpring();
}

void initPlayerSpring() {
    player.x = 24;
    player.y = 24;
    player.width = 16;
    player.height = 32;
    player.xVel = 1;
    player.yVel = 1;
    player.timeUntilNextFrame = 15;
    player.direction = RIGHT;
    player.isAnimating = 0;
    player.currentFrame = 0;
    player.numFrames = 3;
    player.oamIndex = 6;
    player.oldX = player.x;
    player.oldY = player.y;
}

void reInitPlayerSpring() {
    player.x = 64;
    player.y = 96;
    player.width = 16;
    player.height = 32;
    player.xVel = 1;
    player.yVel = 1;
    player.timeUntilNextFrame = 15;
    player.direction = DOWN;
    player.isAnimating = 0;
    player.currentFrame = 0;
    player.numFrames = 3;
    player.oamIndex = 6;
    player.oldX = player.x;
    player.oldY = player.y;
}

void initBlupees() {
    blupee1.x = 48;
    blupee1.y = 144;
    blupee1.width = 16;
    blupee1.height = 16;
    blupee1.xVel = 1;
    blupee1.yVel = 1;
    blupee1.timeUntilNextFrame = 5;
    blupee1.blupeeDirection = DOWN;
    blupee1.isAnimating = 1;
    blupee1.currentFrame = 0;
    blupee1.numFrames = 3;
    blupee1.oamIndex = 7;
    blupee1.active = 1;

    blupee2.x = 176;
    blupee2.y = 72;
    blupee2.width = 16;
    blupee2.height = 16;
    blupee2.xVel = 1;
    blupee2.yVel = 1;
    blupee2.timeUntilNextFrame = 5;
    blupee2.blupeeDirection = DOWN;
    blupee2.isAnimating = 1;
    blupee2.currentFrame = 0;
    blupee2.numFrames = 3;
    blupee2.oamIndex = 8;
    blupee2.active = 1;

    blupee3.x = 136;
    blupee3.y = 192;
    blupee3.width = 16;
    blupee3.height = 16;
    blupee3.xVel = 1;
    blupee3.yVel = 1;
    blupee3.timeUntilNextFrame = 5;
    blupee3.blupeeDirection2 = LEFT;
    blupee3.isAnimating = 1;
    blupee3.currentFrame = 0;
    blupee3.numFrames = 3;
    blupee3.oamIndex = 9;
    blupee3.active = 1;
}

void initLotM() {
    lotm.x = 96;
    lotm.y = 136;
    lotm.width = 40;
    lotm.height = 24;
    lotm.timeUntilNextFrame = 60;
    lotm.isAnimating = 1;
    lotm.currentFrame = 0;
    lotm.numFrames = 2;
    lotm.oamIndex = 25;
}

void initKorokSpring() {
    korok.x = 208;
    korok.y = 24;
    korok.width = 16;
    korok.height = 24;
    korok.xVel = 1;
    korok.yVel = 1;
    korok.timeUntilNextFrame = 20;
    korok.isAnimating = 1;
    korok.currentFrame = 0;
    korok.numFrames = 3;
    korok.oamIndex = 18;
    korok.korokDirection = DOWN;
    korok.active = 1;
}

void updateSpring() {
    updatePlayerSpring();
    updateBlupees();
    updateLotM();
    updateKorokSpring();
}

void updatePlayerSpring() {
    player.isAnimating = 0;
    //centers player on screen
    hOff = player.x - (SCREENWIDTH - player.width) / 2;
    vOff = player.y - (SCREENHEIGHT - player.height) / 2;

    int leftX = player.x;
    int rightX = player.x + player.width - 1;
    int topY = player.y;
    int bottomY = player.y + player.height - 1;

    //movement logic
    if (BUTTON_HELD(BUTTON_UP)) {
        player.isAnimating = 1;
        player.direction = UP;
        if (player.y + player.height < MAPHEIGHT && colorAt2(leftX, topY - player.yVel) && colorAt2(rightX, topY - player.yVel)) {
            player.y -= player.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        player.isAnimating = 1;
        player.direction = DOWN;
        if (player.y + player.height < MAPHEIGHT && colorAt2(leftX, bottomY + player.yVel) && colorAt2(rightX, bottomY + player.yVel)) {
            player.y += player.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_RIGHT) && (player.x + player.width) < MAPWIDTH) {
        player.isAnimating = 1;
        player.direction = RIGHT;
        if ((player.x + player.width) < MAPWIDTH && colorAt2(rightX + player.xVel, topY) && colorAt2(rightX + player.xVel, bottomY)) {
            player.x += player.xVel;
        }
    } 
    if (BUTTON_HELD(BUTTON_LEFT)) {
        player.isAnimating = 1;
        player.direction = LEFT;
        if ((player.x + player.width) < MAPWIDTH && colorAt2(leftX - player.xVel, topY) && colorAt2(leftX - player.xVel, bottomY)) {
            player.x -= player.xVel;
        }
    }
    //restrict player to map
    if (player.x < 0) {
        player.x = 0;
    }
    if (player.y < 0) {
        player.y = 0;
    }

    //restrict camera movement
    if (player.x + player.width > MAPWIDTH) {
        player.x = MAPWIDTH - player.width;
    }
    if (player.y + player.height > MAPHEIGHT) {
        player.y = MAPHEIGHT - player.height;
    }

    //restictions to complex movement
    if (hOff < 0) {
        hOff = 0;
    }
    if (vOff < 0) {
        vOff = 0;
    }
    if (hOff > MAPWIDTH - SCREENWIDTH) {
        hOff = MAPWIDTH - SCREENWIDTH;
    }
    if (vOff > MAPHEIGHT - SCREENHEIGHT) {
        vOff = MAPHEIGHT - SCREENHEIGHT;
    }

    //animation logic
    if (player.isAnimating) {
        --player.timeUntilNextFrame;
        if (player.timeUntilNextFrame == 0) {
            player.currentFrame = (player.currentFrame + 1) % player.numFrames;
            player.timeUntilNextFrame = 15;
        }
    } else {
        player.currentFrame = 1;
    }
    
    //enter overworld
    if (collision(player.x, player.y, player.width, player.height, 16, 0, 32, 24)) {
        reInitGame();
        goToGame();
    }

    //enter spring trial
    if (collision(player.x, player.y, player.width, player.height, 80, 133, 80, 50) && (spiritOrbCount == 3 || (hasArmor && spiritOrbCount >= 1))) {
        initTrial(); 
        goToTrial();
    }
}

void updateBlupees() {
    //walking up and down
    if (blupee1.y < 192 && blupee1.blupeeDirection == BDOWN) {
        blupee1.y++;
    }
    if (blupee1.y > 144 && blupee1.blupeeDirection == BUP) {
        blupee1.y--;
    }

    if (blupee2.y < 120 && blupee2.blupeeDirection == BDOWN) {
        blupee2.y++;
    }
    if (blupee2.y > 72 && blupee2.blupeeDirection == BUP) {
        blupee2.y--;
    }

    if (blupee3.x < 176 && blupee3.blupeeDirection == BLEFT) {
        blupee3.x++;
    }
    if (blupee3.x > 136 && blupee3.blupeeDirection == BRIGHT) {
        blupee3.x--;
    }

    //pause walking at the ends
    if (blupee1.y == 192 && countdown1 > 0) {
        blupee1.isAnimating = 0;
        countdown1--;
    } else if (blupee1.y == 192 && countdown1 == 0) {
        blupee1.isAnimating = 1;
        countdown1 = 60;
        blupee1.blupeeDirection = BUP;
    }
    if (blupee1.y == 144 && countdown1 > 0) {
        blupee1.isAnimating = 0;
        countdown1--;
    } else if (blupee1.y == 144 && countdown1 == 0) {
        blupee1.isAnimating = 1;
        countdown1 = 60;
        blupee1.blupeeDirection = BDOWN;
    }

    if (blupee2.y == 120 && countdown2 > 0) {
        blupee2.isAnimating = 0;
        countdown2--;
    } else if (blupee2.y == 120 && countdown2 == 0) {
        blupee2.isAnimating = 1;
        countdown2 = 150;
        blupee2.blupeeDirection = BUP;
    }
    if (blupee2.y == 72 && countdown2 > 0) {
        blupee2.isAnimating = 0;
        countdown2--;
    } else if (blupee2.y == 72 && countdown2 == 0) {
        blupee2.isAnimating = 1;
        countdown2 = 150;
        blupee2.blupeeDirection = BDOWN;
    }

    if (blupee3.x == 176 && countdown3 > 0) {
        blupee3.isAnimating = 0;
        countdown3--;
    } else if (blupee3.x == 176 && countdown3 == 0) {
        blupee3.isAnimating = 1;
        countdown3 = 200;
        blupee3.blupeeDirection = BRIGHT;
    }
    if (blupee3.x == 136 && countdown3 > 0) {
        blupee3.isAnimating = 0;
        countdown3--;
    } else if (blupee3.x == 136 && countdown3 == 0) {
        blupee3.isAnimating = 1;
        countdown3 = 200;
        blupee3.blupeeDirection = BLEFT;
    }

    //animation logic
    if (blupee1.isAnimating) {
        --blupee1.timeUntilNextFrame;
        if (blupee1.timeUntilNextFrame == 0) {
            blupee1.currentFrame = (blupee1.currentFrame + 1) % blupee1.numFrames;
            blupee1.timeUntilNextFrame = 5;
        }
    } else {
        blupee1.currentFrame = 0;
        blupee1.timeUntilNextFrame = 5;
    }

    if (blupee2.isAnimating) {
        --blupee2.timeUntilNextFrame;
        if (blupee2.timeUntilNextFrame == 0) {
            blupee2.currentFrame = (blupee2.currentFrame + 1) % blupee2.numFrames;
            blupee2.timeUntilNextFrame = 5;
        }
    } else {
        blupee2.currentFrame = 0;
        blupee2.timeUntilNextFrame = 5;
    }

    if (blupee3.isAnimating) {
        --blupee3.timeUntilNextFrame;
        if (blupee3.timeUntilNextFrame == 0) {
            blupee3.currentFrame = (blupee3.currentFrame + 1) % blupee3.numFrames;
            blupee3.timeUntilNextFrame = 10;
        }
    } else {
        blupee3.currentFrame = 0;
        blupee3.timeUntilNextFrame = 10;
    }
}

void updateLotM() {
    //animating logic
    if (lotm.isAnimating) {
        --lotm.timeUntilNextFrame;
        if (lotm.timeUntilNextFrame == 0) {
            lotm.currentFrame = (lotm.currentFrame + 1) % lotm.numFrames;
            lotm.timeUntilNextFrame = 60;
        }
    } else {
        lotm.currentFrame = 1;
        lotm.timeUntilNextFrame = 60;
    }
}

void updateKorokSpring() {
    if (korok.isAnimating) {
        if (korok.korokDirection == DOWN) {
            korok.y += korok.yVel;
            if (korok.y >= 114) {
                korok.korokDirection = UP;
            }
        } else if (korok.korokDirection == UP) {
            korok.y -= korok.yVel;
            if (korok.y <= 24) {
                korok.korokDirection = DOWN;
            }
        }
    }

    //animation logic
    if (korok.isAnimating) {
        --korok.timeUntilNextFrame;
        if (korok.timeUntilNextFrame == 0) {
            korok.currentFrame = (korok.currentFrame + 1) % korok.numFrames;
            korok.timeUntilNextFrame = 20;
        }
    } else {
        korok.currentFrame = 0;
    }

    if (!inConversation) {
        if (korok.y == 114 && countdownKorok > 0) {
            korok.isAnimating = 0;
            countdownKorok--;
        } else if (korok.y == 114 && countdownKorok == 0) {
            korok.isAnimating = 1;
            countdownKorok = 150;
            korok.korokDirection = UP;
        }
        if (korok.y == 24 && countdownKorok > 0) {
            korok.isAnimating = 0;
            countdownKorok--;
        } else if (korok.y == 24 && countdownKorok == 0) {
            korok.isAnimating = 1;
            countdownKorok = 150;
            korok.korokDirection = DOWN;
        }
    }

    if (collision(player.x, player.y, player.width, player.height, korok.x, korok.y, korok.width, korok.height)) {
        if (BUTTON_PRESSED(BUTTON_SELECT)) {
            korok.isAnimating = 0;
            inConversation = 1;
            showTextbox();
            switch(textActivated) {
                case 0:
                    playSoundB(yahaha_data, yahaha_length, 0);
                    eraseText();
                    textToTile("HEY AGAIN!", 513);
                    printArrow();
                    break;
                case 1:
                    eraseText();
                    textToTile("SAVE UP YOUR STAMINA,", 513);
                    textToTile("THEN LEAP INTO THE SPRING", 577);
                    printArrow();
                    break;
                case 2:
                    eraseText();
                    textToTile("TO CATCH THE LORD OF THE", 513);
                    textToTile("MOUNTAIN.", 577);
                    printArrow();
                    break;
                case 3:
                    eraseText();
                    textToTile("YOU'LL HAVE ONLY A LITTLE", 513);
                    textToTile("TIME- STAY CLOSE AND SPAM", 577);
                    printArrow();
                    break;
                case 4:
                    eraseText();    
                    textToTile("SELECT TO CATCH HIM!", 513);
                    printArrow();
                    break;
                case 5:
                    eraseText();
                    textToTile("BEST OF LUCK! TEEHEE!", 513);
                    printButton();
                    break;
            }
            textActivated++;
        }
        if (BUTTON_PRESSED(BUTTON_A)) {
            playSoundB(interact_data, interact_length, 0);
            textActivated = 0;
            korok.isAnimating = 1;
            inConversation = 0;
            hideTextbox(); 
            eraseText();
        }
    }
}

void drawSpring() {
    drawPlayerSpring();
    drawBlupees();
    drawLotM();
    drawKorokSpring();
    drawStamina();
    drawTextbox();
    DMANow(3, shadowOAM, OAM, 128*4);
}

void drawPlayerSpring() {
    if (!hasArmor) {
        shadowOAM[player.oamIndex].attr0 = ATTR0_Y(player.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[player.oamIndex].attr1 = ATTR1_X(player.x - hOff) | ATTR1_MEDIUM;
        shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(1) | ATTR2_TILEID(player.currentFrame * 2, player.direction * 4) | ATTR2_PRIORITY(2);
        REG_BG1HOFF = hOff;
        REG_BG1VOFF = vOff;
    } else {
        shadowOAM[player.oamIndex].attr0 = ATTR0_Y(player.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[player.oamIndex].attr1 = ATTR1_X(player.x - hOff) | ATTR1_MEDIUM;
        shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(7) | ATTR2_TILEID(6 + (player.currentFrame * 2), player.direction * 4) | ATTR2_PRIORITY(2);
        REG_BG1HOFF = hOff;
        REG_BG1VOFF = vOff;
    }
    DMANow(3, shadowOAM, OAM, 128*4);
}

void drawBlupees() {
    int screenY1 = blupee1.y - vOff;
    int screenX1 = blupee1.x - hOff;
    if (screenY1 > SCREENHEIGHT || screenX1 > SCREENWIDTH || screenY1 < -blupee1.height || screenX1 < -blupee1.width || !blupee1.active) {
        shadowOAM[blupee1.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        int blupee1Y = (blupee1.blupeeDirection == BDOWN) ? 0 : 2;
        shadowOAM[blupee1.oamIndex].attr0 = ATTR0_Y(blupee1.y - vOff) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[blupee1.oamIndex].attr1 = ATTR1_X(blupee1.x - hOff) | ATTR1_SMALL;
        shadowOAM[blupee1.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID((13 + (blupee1.currentFrame * 2)), blupee1Y) | ATTR2_PRIORITY(2);
    }

    int screenY2 = blupee2.y - vOff;
    int screenX2 = blupee2.x - hOff;
    if (screenY2 > SCREENHEIGHT || screenX2 > SCREENWIDTH || screenY2 < -blupee2.height || screenX2 < -blupee2.width || !blupee2.active) {
        shadowOAM[blupee2.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        int blupee2Y = (blupee2.blupeeDirection == BDOWN) ? 0 : 2;
        shadowOAM[blupee2.oamIndex].attr0 = ATTR0_Y(blupee2.y - vOff) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[blupee2.oamIndex].attr1 = ATTR1_X(blupee2.x - hOff) | ATTR1_SMALL;
        shadowOAM[blupee2.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID((13 + (blupee2.currentFrame * 2)), blupee2Y) | ATTR2_PRIORITY(2);
    }

    int screenY3 = blupee3.y - vOff;
    int screenX3 = blupee3.x - hOff;
    if (screenY3 > SCREENHEIGHT || screenX3 > SCREENWIDTH || screenY3 < -blupee3.height || screenX3 < -blupee3.width || !blupee3.active) {
        shadowOAM[blupee3.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        int blupee3Y = (blupee3.blupeeDirection == BLEFT) ? 6 : 4;
        shadowOAM[blupee3.oamIndex].attr0 = ATTR0_Y(blupee3.y - vOff) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[blupee3.oamIndex].attr1 = ATTR1_X(blupee3.x - hOff) | ATTR1_SMALL;
        shadowOAM[blupee3.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID((13 + (blupee3.currentFrame * 2)), blupee3Y) | ATTR2_PRIORITY(2);
    }
}

void drawLotM() {
    int screenY = lotm.y - vOff;
    int screenX = lotm.x - hOff;
    if (screenY > SCREENHEIGHT || screenX > SCREENWIDTH || screenY < -lotm.height || screenX < -lotm.width) {
        shadowOAM[lotm.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        shadowOAM[lotm.oamIndex].attr0 = ATTR0_Y(lotm.y - vOff) | ATTR0_REGULAR | ATTR0_WIDE;
        shadowOAM[lotm.oamIndex].attr1 = ATTR1_X(lotm.x - hOff) | ATTR1_LARGE;
        shadowOAM[lotm.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID(14, 20) | ATTR2_PRIORITY(2);
    }
}

void drawKorokSpring() {
    int screenY = korok.y - vOff;
    int screenX = korok.x - hOff;
    //making sure korok doesn't appear twice
    if ((screenY > SCREENHEIGHT || screenX > SCREENWIDTH || screenY < -korok.height || screenX < -korok.width) || !korok.active) {
        shadowOAM[korok.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        int korokY = (korok.korokDirection == DOWN) ? 20 : 24;
        shadowOAM[korok.oamIndex].attr0 = ATTR0_Y(korok.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[korok.oamIndex].attr1 = ATTR1_X(korok.x - hOff) | ATTR1_MEDIUM;
        shadowOAM[korok.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(korok.currentFrame * 2, korokY) | ATTR2_PRIORITY(2);
    }
}

inline unsigned char colorAt2(int x, int y) {
    return ((unsigned char*) collisionSpringBitmap) [OFFSET(x, y, MAPWIDTH)];
}