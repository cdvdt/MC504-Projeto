#ifndef SPRITES_H
#define SPRITES_H

#include "anim.h"

#define DELTA_C 20
#define DELTA_R 7

void presentation();

int get_screen_column();

void screen_init();

void show_cursor();

void draw_screen(AnimStateData state);

#endif
