#ifndef __SPRITES_H__
#define __SPRITES_H__

#include<stdio.h>
#include <sys/ioctl.h>
#include "anim.h"

#define DELTA_C 20
#define DELTA_R 7

int get_screen_row();

int get_screen_column();

void move_cursor (int l, int c);

void hide_cursor();

void show_cursor();

void clear_screen();

//int draw_water (int l, int c);

//int draw_atom();

void screen_init();

//int number_of_atoms();

//int is_possible();

void presentation ();

void draw_screen ();

#endif
