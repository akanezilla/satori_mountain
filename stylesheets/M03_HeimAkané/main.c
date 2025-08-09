#include "gba.h"
#include "mode0.h"
#include "sprites.h"
#include "print.h"
#include "game.h"
#include "startTiles.h"
#include "start.h"
#include "instructions.h"
#include "instructionsPal.h"
#include "instructions2.h"
#include "zelda.h"
#include "zeldaMap.h"
#include "win.h"
#include "winPal.h"
#include "lose.h"
#include "losePal.h"
#include "spring.h"
#include "springMap.h"
#include "springTrial.h"
#include "trialMap.h"
#include "trial.h"
#include "mode4.h"
#include "digitalSound.h"
#include "startMusic.h"
#include "game.h"
#include "pause.h"
#include "pausePal.h"
#include "loseSong.h"
#include "clouds.h"
#include "cloudTiles.h"
#include "grass.h"
#include "winSong.h"
#include "link.h"

void initialize();

void goToStart();
void start();
void goToInstructions();
void instructions();
void goToInstructions2();
void instructions2();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();
void goToSpring();
void spring();
void goToTrial();
void trial();
void winInit();
void winUpdate();
void winDraw();

enum STATE {START, INSTRUCTIONS, INSTRUCTIONS2, GAME, PAUSE, WIN, LOSE, SPRING, TRIAL} state;

unsigned short buttons;
unsigned short oldButtons;

enum STATE prevState;

int hOff;
int vOff;
int hOff2;

int main() {
    mgba_open();
    initialize();

    while(1) {
        oldButtons = buttons;
        buttons = REG_BUTTONS;

        switch(state) {
            case START:
                start();
                break;
            case INSTRUCTIONS:
                instructions();
                break;
            case INSTRUCTIONS2:
                instructions2();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
            case SPRING:
                spring();
                break;
            case TRIAL:
                trial();
                break;
        }
    }
    return 0;
}

void initialize() {
    //set mode 4, background, and backbuffer
    mgba_open();
    REG_DISPCTL = MODE(4) | BG_ENABLE(2) | DISP_BACKBUFFER;
    initSound();

    goToStart();
}

void goToStart() {
    REG_DISPCTL = MODE(4) | BG_ENABLE(2) | DISP_BACKBUFFER;
    DMANow(3, startPal, BG_PALETTE, 256);
    drawFullscreenImage4(startBitmap);
    setupSoundInterrupts();
    setupSounds();
    playSoundA(startMusic_data, startMusic_length, 1);

    waitForVBlank();
    flipPages();

    drawFullscreenImage4(startBitmap);
    waitForVBlank();
    flipPages(); 

    state = START;
}

void start() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToInstructions();
    }
}

void goToInstructions() {
    DMANow(3, instructions2Pal, BG_PALETTE, 256);
    drawFullscreenImage4(instructions2Bitmap);
    waitForVBlank();
    flipPages();

    drawFullscreenImage4(instructions2Bitmap);
    waitForVBlank();
    flipPages();

    state = INSTRUCTIONS;
}

void instructions() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToInstructions2();
    }
}

void goToInstructions2() {
    DMANow(3, instructionsPal, BG_PALETTE, 256);
    drawFullscreenImage4(instructionsBitmap);
    waitForVBlank();
    flipPages();

    drawFullscreenImage4(instructionsBitmap);
    waitForVBlank();
    flipPages();

    state = INSTRUCTIONS2;
}

void instructions2() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        initGame();
        goToGame();
    }
}

void goToGame() {
    DMANow(3, zeldaTiles, &CHARBLOCK[0], zeldaTilesLen / 2);
    DMANow(3, zeldaMapMap, &SCREENBLOCK[8], zeldaMapLen / 2);
    DMANow(3, zeldaPal, BG_PALETTE, 256);

    blupee1.active = 0;
    blupee2.active = 0;
    blupee3.active = 0;
    hideSprites();

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    prevState = state;
    state = GAME;
}

void game() {
    updateGame();
    drawGame();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
}

void goToPause() {
    //make sure screen isnt affected by complex movement
    hOff = 0;
    vOff = 0;
    REG_BG1HOFF = 0;
    REG_BG1VOFF = 0;

    DMANow(3, pausePalTiles, &CHARBLOCK[0], pausePalTilesLen / 2);
    DMANow(3, pauseMap, &SCREENBLOCK[8], pauseLen / 2);
    DMANow(3, pausePalPal, BG_PALETTE, pausePalPalLen / 2);

    pauseSounds();
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    prevState = state;
    state = PAUSE;
}

void pause() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START) && prevState == GAME) {
        unpauseSounds();
        goToGame();
    }
    if (BUTTON_PRESSED(BUTTON_START) && prevState == SPRING) {
        unpauseSounds();
        goToSpring();
    }
    if (BUTTON_PRESSED(BUTTON_START) && prevState == TRIAL) {
        unpauseSounds();
        goToTrial();
    }
}

void goToWin() {
    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | BG_ENABLE(1) | SPRITE_ENABLE;
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | 1;
    REG_BG1CNT = BG_SCREENBLOCK(25) | BG_CHARBLOCK(2) | 0;

    DMANow(3, cloudsTiles, &CHARBLOCK[0], cloudsTilesLen / 2);
    DMANow(3, cloudsTiles, &CHARBLOCK[2], cloudsTilesLen / 2);
    DMANow(3, cloudTilesMap, &SCREENBLOCK[24], cloudTilesMapLen / 2);
    DMANow(3, grassMap, &SCREENBLOCK[25], grassLen / 2);
    DMANow(3, cloudsPal, BG_PALETTE, cloudsPalLen / 2);

    hOff = 0;
    hOff2 = 0;
    vOff = 0;

    //player.active = 0;
    setupSounds();
    playSoundA(winSong_data, winSong_length, 1);
    hideSprites();
    numbers.active = 0;
    piece1.active = 0;
    piece2.active = 0;
    piece3.active = 0;
    piece4.active = 0;
    piece5.active = 0;
    numbers.active = 0;
    staminaBar.active = 0;
    DMANow(3, shadowOAM, OAM, 128*4);

    winInit();
    state = WIN;
}

void win() {
    winUpdate();
    winDraw();
    DMANow(3, shadowOAM, OAM, 128*4);
    hOff += 1;
    hOff2 += 2;
    REG_BG0HOFF = hOff;
    REG_BG1HOFF = hOff2;
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToLose() {
    //make sure screen isnt affected by complex movement
    hOff = 0;
    vOff = 0;
    REG_BG1HOFF = 0;
    REG_BG1VOFF = 0;

    DMANow(3, losePalTiles, &CHARBLOCK[0], losePalTilesLen / 2);
    DMANow(3, loseMap, &SCREENBLOCK[8], loseLen / 2);
    DMANow(3, losePalPal, BG_PALETTE, losePalPalLen / 2);

    player.active = 0;
    setupSounds();
    playSoundA(loseSong_data, loseSong_length, 1);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128*4);

    state = LOSE;
}

void lose() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToSpring() {
    DMANow(3, springTiles, &CHARBLOCK[0], springTilesLen / 2);
    DMANow(3, springMapMap, &SCREENBLOCK[8], springMapLen / 2);
    DMANow(3, springPal, BG_PALETTE, springPalLen / 2);


    hideSprites();
    
    DMANow(3, shadowOAM, OAM, 128*4);

    prevState = state;
    state = SPRING;
}

void spring() {
    updateSpring();
    drawSpring();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
}

void goToTrial() {
    DMANow(3, springTiles, &CHARBLOCK[0], springTilesLen / 2);
    DMANow(3, trialMapMap, &SCREENBLOCK[8], trialMapLen / 2);
    DMANow(3, springPal, BG_PALETTE, springPalLen / 2);

    //hide sprites
    blupee1.active = 0;
    blupee2.active = 0;
    blupee3.active = 0;
    korok.active = 0;
    hideSprites();

    DMANow(3, shadowOAM, OAM, 128*4);

    state = TRIAL;
}

void trial() {
    updateTrial();
    drawTrial();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
}

void winInit() {
    initHeartsWin();
    initWinWord();

    player.x = 104;
    player.y = 125;
    player.width = 16;
    player.height = 24;
    player.oamIndex = 0;
    player.isAnimating = 1;
    player.numFrames = 3;
    player.direction = RIGHT;
    player.timeUntilNextFrame = 5;
    player.active = 1;
    player.currentFrame = 0;

    lotm.x = 69;
    lotm.y = 125;
    lotm.width = 42;
    lotm.height = 32;
    lotm.oamIndex = 1;
    lotm.isAnimating = 1;
    lotm.numFrames = 3;
    lotm.direction = RIGHT;
    lotm.timeUntilNextFrame = 5;
    lotm.active = 1;
    lotm.currentFrame = 0;

    DMANow(3, linkTiles, &CHARBLOCK[4], linkTilesLen / 2);
    DMANow(3, linkPal, SPRITE_PAL, 256);
}

void winUpdate() {
    if (player.isAnimating) {
        --player.timeUntilNextFrame;
        if (player.timeUntilNextFrame == 0) {
            player.currentFrame = (player.currentFrame + 1) % player.numFrames;
            player.timeUntilNextFrame = 5;
        }
    } else {
        player.currentFrame = 1;
        player.timeUntilNextFrame = 5;
    }

    if (lotm.isAnimating) {
        --lotm.timeUntilNextFrame;
        if (lotm.timeUntilNextFrame == 0) {
            lotm.currentFrame = (lotm.currentFrame + 1) % lotm.numFrames;
            lotm.timeUntilNextFrame = 5;
        }
    } else {
        lotm.currentFrame = 0;
    }
    updateHeartsWin();
}

void winDraw() {
    drawHearts();
    drawWinWord();
    
    if (player.active && !hasArmor) {
        shadowOAM[player.oamIndex].attr0 = ATTR0_Y(player.y) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[player.oamIndex].attr1 = ATTR1_X(player.x) | ATTR1_MEDIUM;
        shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(1) | ATTR2_TILEID(player.currentFrame * 2, 4) | ATTR2_PRIORITY(0);
    } else if (player.active && hasArmor) {
        shadowOAM[player.oamIndex].attr0 = ATTR0_Y(player.y) | ATTR0_REGULAR | ATTR0_TALL;
        shadowOAM[player.oamIndex].attr1 = ATTR1_X(player.x) | ATTR1_MEDIUM;
        shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(7) | ATTR2_TILEID(6 + (player.currentFrame * 2), 4)  | ATTR2_PRIORITY(0);
    } else {
        shadowOAM[player.oamIndex].attr0 = ATTR0_HIDE;
    }

    if (lotm.active) {
        shadowOAM[lotm.oamIndex].attr0 = ATTR0_Y(lotm.y) | ATTR0_REGULAR | ATTR0_WIDE;
            shadowOAM[lotm.oamIndex].attr1 = ATTR1_X(lotm.x) | ATTR1_LARGE;
            shadowOAM[lotm.oamIndex].attr2 = ATTR2_PALROW(8) | ATTR2_TILEID((6  + (lotm.currentFrame * 8)), 24) | ATTR2_PRIORITY(0);
    } else {
        shadowOAM[lotm.oamIndex].attr0 = ATTR0_HIDE;
    }
}