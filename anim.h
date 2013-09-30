#ifndef __ANIN_H__
#define __ANIM_H__

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sprites.h"

/*pthread_mutex_t mutex;
pthread_mutex_t display;*/
int oxygen;
int hydrogen;
/*pthread_barrier_t barrier;
sem_t oxyQueue;
sem_t hydroQueue;*/
int water;
int building ;
int t_oxygen;
int t_hydrogen;
int N;

void bond(int i);
void* oxygen_thread (void *v);
void* hydrogen_thread (void *v);
void* draw_anim(void *v);

#endif
