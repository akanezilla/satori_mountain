#include "mode0.h"

void eraseText();
void textToTile(char string[], int offset);

void eraseText() {
    //sets each tile to an empty tile
    for (int i = 0; i < 1024; i++) {
        SCREENBLOCK[12].tilemap[i] = TILEMAP_ENTRY_TILEID(0) | TILEMAP_ENTRY_PALROW(0);
    }
}

void textToTile(char string[], int offset) {
    int i = 0;
    //prints each letter of the string
    while (string[i] != '\0') {
        SCREENBLOCK[12].tilemap[i + offset] = TILEMAP_ENTRY_TILEID(string[i]) | TILEMAP_ENTRY_PALROW(0);
        i++;
    }
}

void printArrow() {
    SCREENBLOCK[12].tilemap[604] = TILEMAP_ENTRY_TILEID(92) | TILEMAP_ENTRY_PALROW(0);
}

void printButton() {
    SCREENBLOCK[12].tilemap[604] = TILEMAP_ENTRY_TILEID(93) | TILEMAP_ENTRY_PALROW(0);
}