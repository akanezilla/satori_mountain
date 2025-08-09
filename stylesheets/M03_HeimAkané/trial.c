#include "trial.h"
#include "sprites.h"
#include "link.h"
#include "mode0.h"
#include "fight.h"
#include "digitalSound.h"
#include "trialCollision.h"

void initTrial();
void initPlayerTrial();
void initLotMTrial();
void initBar();
void initNumbers();
void initHeartsWin();
void initWinWord();
void updateTrial();
void updatePlayerTrial();
void updateLotMTrial();
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

enum DIRECTION {DOWN, RIGHT, UP, LEFT} direction;

SPRITE player;
SPRITE lotm;
SPRITE staminaBar;
SPRITE piece1;
SPRITE piece2;
SPRITE piece3;
SPRITE piece4;
SPRITE piece5;
SPRITE fifteen;
SPRITE fourteen;
SPRITE thirteen;
SPRITE numbers;
SPRITE hearts;
SPRITE hearts2;
SPRITE hearts3;
SPRITE winWord;

int staminaCount;
int alreadyAnimated;
extern int hasArmor;

void initTrial() {
    REG_DISPCTL = MODE(0) | BG_ENABLE(1) | SPRITE_ENABLE;
    REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(8) | BG_SIZE_SMALL;

    DMANow(3, linkTiles, &CHARBLOCK[4], linkTilesLen / 2);
    DMANow(3, linkPal, SPRITE_PAL, 256);

    hOff = 0;
    vOff = 0;
    staminaCount = 0;
    alreadyAnimated = 0;

    initSound();
    setupSoundInterrupts();
    setupSounds();
    playSoundA(fight_data, fight_length, 1);

    initPlayerTrial();
    initLotMTrial();
    initBar();
    initNumbers();

    hideSprites();
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);
}

void initPlayerTrial() {
    player.x = 16;
    player.y = 88;
    player.width = 16;
    player.height = 32;
    player.xVel = 1;
    player.yVel = 1;
    player.timeUntilNextFrame = 15;
    player.direction = DOWN;
    player.isAnimating = 0;
    player.currentFrame = 0;
    player.numFrames = 3;
    player.oamIndex = 0;
    player.oldX = player.x;
    player.oldY = player.y;
    player.active = 1;
}

void initLotMTrial() {
    lotm.x = 96;
    lotm.y = 136;
    lotm.width = 40;
    lotm.height = 24;
    lotm.timeUntilNextFrame = 15;
    lotm.isAnimating = 1;
    lotm.currentFrame = 0;
    lotm.numFrames = 3;
    lotm.oamIndex = 1;
    lotm.direction = RIGHT;
}

void initBar() {
    staminaBar.x = 16;
    staminaBar.y = 2;
    staminaBar.width = 24;
    staminaBar.height = 24;
    staminaBar.oamIndex = 7;
    staminaBar.active = 1;

    piece1.x = 2;
    piece1.y = 17;
    piece1.width = 16;
    piece1.height = 16;
    piece1.oamIndex = 2;
    piece1.active = 0;

    piece2.x = 2;
    piece2.y = 27;
    piece2.width = 16;
    piece2.height = 16;
    piece2.oamIndex = 3;
    piece2.active = 0;

    piece3.x = 9;
    piece3.y = 16;
    piece3.width = 16;
    piece3.height = 16;
    piece3.oamIndex = 4;
    piece3.active = 0;

    piece4.x = 9;
    piece4.y = 30;
    piece4.width = 16;
    piece4.height = 16;
    piece4.oamIndex = 5;
    piece4.active = 0;
    
    piece5.x = 18;
    piece5.y = 21;
    piece5.width = 16;
    piece5.height = 16;
    piece5.oamIndex = 6;
    piece5.active = 0;
}

void initNumbers() {
    numbers.x = 208;
    numbers.y = 0;
    numbers.width = 16;
    numbers.height = 16;
    numbers.active = 1;
    numbers.oamIndex = 8;
    numbers.isAnimating = 1;
    numbers.numFrames = 15;
    numbers.currentFrame = 0;
    numbers.timeUntilNextFrame = 60;
}

void updateTrial() {
    updatePlayerTrial();
    updateLotMTrial();
    updateBar();
    updateNumbers();
    //updateHearts();
}

void updatePlayerTrial() {
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
        if (player.y + player.height < MAPHEIGHT && colorAt3(leftX, topY - player.yVel) && colorAt3(rightX, topY - player.yVel)) {
            player.y -= player.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        player.isAnimating = 1;
        player.direction = DOWN;
        if (player.y + player.height < MAPHEIGHT && colorAt3(leftX, bottomY + player.yVel) && colorAt3(rightX, bottomY + player.yVel)) {
            player.y += player.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_RIGHT) && (player.x + player.width) < MAPWIDTH) {
        player.isAnimating = 1;
        player.direction = RIGHT;
        if ((player.x + player.width) < MAPWIDTH && colorAt3(rightX + player.xVel, topY) && colorAt3(rightX + player.xVel, bottomY)) {
            player.x += player.xVel;
        }
    } 
    if (BUTTON_HELD(BUTTON_LEFT)) {
        player.isAnimating = 1;
        player.direction = LEFT;
        if ((player.x + player.width) < MAPWIDTH && colorAt3(leftX - player.xVel, topY) && colorAt3(leftX - player.xVel, bottomY)) {
            player.x -= player.xVel;
        }
    }
    //increment stamina if colliding with lotm
    if (BUTTON_PRESSED(BUTTON_SELECT) && collision(player.x, player.y, player.width, player.height, lotm.x, lotm.y, lotm.width, lotm.height)) {
        staminaCount++;
    }

    //modify sprite pal if collision with lotm happens
    if (collision(player.x, player.y, player.width, player.height, lotm.x, lotm.y, lotm.width, lotm.height)) {
        SPRITE_PAL[139] = YELLOW;
    } else {
        SPRITE_PAL[139] = BLACK;
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
}

void updateLotMTrial() {
    //seting bounds for the circle
    if (lotm.direction == RIGHT && lotm.x < 184) {
        lotm.x++;
    } else if (lotm.direction == DOWN && lotm.y < 176) {
        lotm.y++;
    } else if (lotm.direction == LEFT && lotm.x > 8) {
        lotm.x--;
    } else if (lotm.direction == UP && lotm.y > 88) {
        lotm.y--;
    }

    //changing directions
    if (lotm.y == 88 && lotm.direction == UP) {
        lotm.direction = RIGHT;
    } else if (lotm.x == 184 && lotm.direction == RIGHT) {
        lotm.direction = DOWN;
        lotm.numFrames = 2;
    } else if (lotm.y == 176 && lotm.direction == DOWN) {
        lotm.direction = LEFT;
    } else if (lotm.x == 8 && lotm.direction == LEFT) {
        lotm.direction = UP;
        lotm.numFrames = 2;
    }

    //animating logic
    if (lotm.isAnimating) {
        --lotm.timeUntilNextFrame;
        if (lotm.timeUntilNextFrame == 0) {
            lotm.currentFrame = (lotm.currentFrame + 1) % lotm.numFrames;
            lotm.timeUntilNextFrame = 25;
        }
    } else {
        lotm.currentFrame = 0;
    }
}

void updateBar() {
    if (staminaCount == 10 && !piece2.active) {
        piece2.active = 1;
        hearts.isAnimating = 1;
        hearts.active = 1;
    } else if (staminaCount == 20 && !piece4.active) {
        piece4.active = 1;
        hearts.isAnimating = 1;
        hearts.active = 1;
    } else if (staminaCount == 30 && !piece5.active) {
        piece5.active = 1;
        hearts.isAnimating = 1;
        hearts.active = 1;
    } else if (staminaCount == 40 && !piece3.active) {
        piece3.active = 1;
        hearts.isAnimating = 1;
        hearts.active = 1;
    } else if (staminaCount == 50 && !piece1.active) {
        piece1.active = 1;
        hearts.isAnimating = 1;
        hearts.active = 1;
        goToWin();
    }
}

void updateNumbers() {
    if (numbers.isAnimating) {
        --numbers.timeUntilNextFrame;
        if (numbers.timeUntilNextFrame == 0) {
            numbers.currentFrame = (numbers.currentFrame + 1) % numbers.numFrames;
            numbers.timeUntilNextFrame = 60;
            alreadyAnimated++;
        }
    } else {
        numbers.currentFrame = 0;
    }

    if (numbers.currentFrame == 0 && (alreadyAnimated > 0)) {
        numbers.isAnimating = 0;
        numbers.active = 0;
        if (staminaCount >= 50) {
            goToWin();
        } else {
            goToLose();
        }
    }
}

void drawTrial() {
    drawPlayerTrial();
    drawLotMTrial();
    drawBar();
    drawNumbers();
}

void drawPlayerTrial() {
    if (player.active && !hasArmor) {
        shadowOAM[player.oamIndex].attr0 = ATTR0_Y(player.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[player.oamIndex].attr1 = ATTR1_X(player.x - hOff) | ATTR1_MEDIUM;
        shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(1) | ATTR2_TILEID(player.currentFrame * 2, player.direction * 4);
        REG_BG1HOFF = hOff;
        REG_BG1VOFF = vOff;
    } else if (player.active && hasArmor) {
        shadowOAM[player.oamIndex].attr0 = ATTR0_Y(player.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[player.oamIndex].attr1 = ATTR1_X(player.x - hOff) | ATTR1_MEDIUM;
        shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(7) | ATTR2_TILEID(6 + (player.currentFrame * 2), player.direction * 4);
        REG_BG1HOFF = hOff;
        REG_BG1VOFF = vOff;
    } 
    else {
        shadowOAM[player.oamIndex].attr0 = ATTR0_HIDE; 
    }
    DMANow(3, shadowOAM, OAM, 128*4);
}

void drawLotMTrial() {
    int screenY = lotm.y - vOff;
    int screenX = lotm.x - hOff;
    if (screenY > SCREENHEIGHT || screenX > SCREENWIDTH || screenY < -lotm.height || screenX < -lotm.width) {
        shadowOAM[lotm.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        if (lotm.direction == LEFT) {
            shadowOAM[lotm.oamIndex].attr0 = ATTR0_Y(lotm.y - vOff) | ATTR0_REGULAR | ATTR0_WIDE;
            shadowOAM[lotm.oamIndex].attr1 = ATTR1_X(lotm.x - hOff) | ATTR1_LARGE;
            shadowOAM[lotm.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID((6  + (lotm.currentFrame * 8)), 20);
        } else if (lotm.direction == RIGHT) {
            shadowOAM[lotm.oamIndex].attr0 = ATTR0_Y(lotm.y - vOff) | ATTR0_REGULAR | ATTR0_WIDE;
            shadowOAM[lotm.oamIndex].attr1 = ATTR1_X(lotm.x - hOff) | ATTR1_LARGE;
            shadowOAM[lotm.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID((6  + (lotm.currentFrame * 8)), 24);
        } else if (lotm.direction == DOWN) {
            shadowOAM[lotm.oamIndex].attr0 = ATTR0_Y(lotm.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
            shadowOAM[lotm.oamIndex].attr1 = ATTR1_X(lotm.x - hOff) | ATTR1_MEDIUM;
            shadowOAM[lotm.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID(12, 12 + (lotm.currentFrame * 4));
        } else if (lotm.direction == UP) {
            shadowOAM[lotm.oamIndex].attr0 = ATTR0_Y(lotm.y - vOff) | ATTR0_REGULAR | ATTR0_TALL;
            shadowOAM[lotm.oamIndex].attr1 = ATTR1_X(lotm.x - hOff) | ATTR1_MEDIUM;
            shadowOAM[lotm.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID(14, 12 + (lotm.currentFrame * 4));
        }
    }
}

void drawBar() {
    //bar
    if (staminaBar.active) {
        shadowOAM[staminaBar.oamIndex].attr0 = ATTR0_Y(staminaBar.y) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[staminaBar.oamIndex].attr1 = ATTR1_X(staminaBar.x) | ATTR1_MEDIUM;
        shadowOAM[staminaBar.oamIndex].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(19, 0);
    } else {
        shadowOAM[staminaBar.oamIndex].attr0 = ATTR0_HIDE;
    }

    //pieces
    if (piece1.active) {
        shadowOAM[piece1.oamIndex].attr0 = ATTR0_Y(piece1.x) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[piece1.oamIndex].attr1 = ATTR1_X(piece1.y) | ATTR1_SMALL;
        shadowOAM[piece1.oamIndex].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(19, 4);
    } else {
        shadowOAM[piece1.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (piece2.active) {
        shadowOAM[piece2.oamIndex].attr0 = ATTR0_Y(piece2.x) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[piece2.oamIndex].attr1 = ATTR1_X(piece2.y) | ATTR1_SMALL;
        shadowOAM[piece2.oamIndex].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(21, 4);
    } else {
        shadowOAM[piece2.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (piece3.active) {
        shadowOAM[piece3.oamIndex].attr0 = ATTR0_Y(piece3.x) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[piece3.oamIndex].attr1 = ATTR1_X(piece3.y) | ATTR1_SMALL;
        shadowOAM[piece3.oamIndex].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(23, 4);
    } else {
        shadowOAM[piece3.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (piece4.active) {
        shadowOAM[piece4.oamIndex].attr0 = ATTR0_Y(piece4.x) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[piece4.oamIndex].attr1 = ATTR1_X(piece4.y) | ATTR1_SMALL;
        shadowOAM[piece4.oamIndex].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(25, 4);
    } else {
        shadowOAM[piece4.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (piece5.active) {
        shadowOAM[piece5.oamIndex].attr0 = ATTR0_Y(piece5.x) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[piece5.oamIndex].attr1 = ATTR1_X(piece5.y) | ATTR1_SMALL;
        shadowOAM[piece5.oamIndex].attr2 = ATTR2_PALROW(3) | ATTR2_TILEID(27, 4);
    } else {
        shadowOAM[piece5.oamIndex].attr0 = ATTR0_HIDE;
    }
}

void drawNumbers() {
    int numbersY = (numbers.currentFrame < 8 ? 8 : 10);
    if (numbers.active) {
        shadowOAM[numbers.oamIndex].attr0 = ATTR0_Y(numbers.y) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[numbers.oamIndex].attr1 = ATTR1_X(numbers.x) | ATTR1_SMALL;
        shadowOAM[numbers.oamIndex].attr2 = ATTR2_PALROW(5) | ATTR2_TILEID(12 + ((numbers.currentFrame % 8) * 2), numbersY);
    } else {
        shadowOAM[numbers.oamIndex].attr0 = ATTR0_HIDE;
    }
}

void initHeartsWin() {
    hearts.x = lotm.x + lotm.width;
    hearts.y = lotm.y - 8;
    hearts.width = 8;
    hearts.height = 8;
    hearts.numFrames = 4;
    hearts.isAnimating = 1;
    hearts.currentFrame = 1;
    hearts.timeUntilNextFrame = 10;
    hearts.active = 1;
    hearts.oamIndex = 9;

    hearts2.x = lotm.x + lotm.width + 6;
    hearts2.y = lotm.y - 10;
    hearts2.width = 8;
    hearts2.height = 8;
    hearts2.numFrames = 4;
    hearts2.isAnimating = 1;
    hearts2.currentFrame = 2;
    hearts2.timeUntilNextFrame = 10;
    hearts2.active = 1;
    hearts2.oamIndex = 10;

    hearts3.x = lotm.x + lotm.width + 3;
    hearts3.y = lotm.y - 12;
    hearts3.width = 8;
    hearts3.height = 8;
    hearts3.numFrames = 4;
    hearts3.isAnimating = 1;
    hearts3.currentFrame = 4;
    hearts3.timeUntilNextFrame = 10;
    hearts3.active = 1;
    hearts3.oamIndex = 11;
}

void updateHeartsWin() {
    hearts.x = lotm.x + lotm.width;
    hearts.y = lotm.y - 8;

    hearts2.x = lotm.x + lotm.width + 8;
    hearts2.y = lotm.y - 10;

    hearts3.x = lotm.x + lotm.width + 2;
    hearts3.y = lotm.y - 15;

    if (hearts.isAnimating) {
        --hearts.timeUntilNextFrame;
        if (hearts.timeUntilNextFrame == 0) {
            hearts.currentFrame = (hearts.currentFrame + 1) % hearts.numFrames;
            hearts.timeUntilNextFrame = 10;
        }
    } else {
        hearts.currentFrame = 1;
    }

    if (hearts2.isAnimating) {
        --hearts2.timeUntilNextFrame;
        if (hearts2.timeUntilNextFrame == 0) {
            hearts2.currentFrame = (hearts2.currentFrame + 1) % hearts2.numFrames;
            hearts2.timeUntilNextFrame = 10;
        }
    } else {
        hearts2.currentFrame = 2;
    }

    if (hearts3.isAnimating) {
        --hearts3.timeUntilNextFrame;
        if (hearts3.timeUntilNextFrame == 0) {
            hearts3.currentFrame = (hearts3.currentFrame + 1) % hearts3.numFrames;
            hearts3.timeUntilNextFrame = 10;
        }
    } else {
        hearts3.currentFrame = 4;
    }
}

void drawHearts() {
    if (hearts.active) {
        shadowOAM[hearts.oamIndex].attr0 = ATTR0_Y(hearts.y) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[hearts.oamIndex].attr1 = ATTR1_X(hearts.x) | ATTR1_TINY;
        shadowOAM[hearts.oamIndex].attr2 = ATTR2_PALROW(11) | ATTR2_TILEID(30 , 20 + hearts.currentFrame);
    } else {
        shadowOAM[hearts.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (hearts2.active) {
        shadowOAM[hearts2.oamIndex].attr0 = ATTR0_Y(hearts2.y) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[hearts2.oamIndex].attr1 = ATTR1_X(hearts2.x) | ATTR1_TINY;
        shadowOAM[hearts2.oamIndex].attr2 = ATTR2_PALROW(11) | ATTR2_TILEID(30 , 20 + hearts2.currentFrame);
    } else {
        shadowOAM[hearts2.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (hearts3.active) {
        shadowOAM[hearts3.oamIndex].attr0 = ATTR0_Y(hearts3.y) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[hearts3.oamIndex].attr1 = ATTR1_X(hearts3.x) | ATTR1_TINY;
        shadowOAM[hearts3.oamIndex].attr2 = ATTR2_PALROW(11) | ATTR2_TILEID(30 , 20 + hearts3.currentFrame);
    } else {
        shadowOAM[hearts3.oamIndex].attr0 = ATTR0_HIDE;
    }
}

void initWinWord() {
    winWord.x = 104;
    winWord.y = 30;
    winWord.width = 32;
    winWord.height = 32;
    winWord.active = 1;
    winWord.oamIndex = 12;
}

void drawWinWord() {
    if (winWord.active) {
        shadowOAM[winWord.oamIndex].attr0 = ATTR0_Y(winWord.y) | ATTR0_REGULAR | ATTR0_SQUARE;
        shadowOAM[winWord.oamIndex].attr1 = ATTR1_X(winWord.x) | ATTR1_MEDIUM;
        shadowOAM[winWord.oamIndex].attr2 = ATTR2_PALROW(11) | ATTR2_TILEID(28 , 8);
    } else {
        shadowOAM[winWord.oamIndex].attr0 = ATTR0_HIDE;
    }
}

inline unsigned char colorAt3(int x, int y) {
    return ((unsigned char*) trialCollisionBitmap) [OFFSET(x, y, MAPWIDTH)];
}