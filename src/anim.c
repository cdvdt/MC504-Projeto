#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "anim.h"
#include "sprites.h"

pthread_mutex_t mutex;
pthread_mutex_t display;
int oxygen = 0;
int hydrogen = 0;
pthread_barrier_t barrier;
sem_t oxyQueue;
sem_t hydroQueue;
int water = 0;
int building = 0;
int t_oxygen;
int t_hydrogen;
int N;

void bond()
{
    building = 1;
    sleep(1);
}

void *oxygen_thread(void *v)
{
    pthread_mutex_lock(&mutex);

    oxygen += 1;

    sleep(random() % 3 + 1);

    if (hydrogen >= 2)
    {
        pthread_mutex_lock(&display);
        sem_post(&hydroQueue);
        sem_post(&hydroQueue);
        hydrogen -= 2;
        sem_post(&oxyQueue);
        oxygen -= 1;
        pthread_mutex_unlock(&display);
    }
    else
        pthread_mutex_unlock(&mutex);

    sem_wait(&oxyQueue);
    bond(1);

    pthread_barrier_wait(&barrier);

    water++;
    building = 0;

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *hydrogen_thread(void *v)
{
    pthread_mutex_lock(&mutex);

    hydrogen += 1;

    sleep(random() % 3);

    if ((hydrogen >= 2) && (oxygen >= 1))
    {
        pthread_mutex_lock(&display);
        sem_post(&hydroQueue);
        sem_post(&hydroQueue);
        hydrogen -= 2;
        sem_post(&oxyQueue);
        oxygen -= 1;
        pthread_mutex_unlock(&display);
    }
    else
        pthread_mutex_unlock(&mutex);

    sem_wait(&hydroQueue);
    bond(2);

    pthread_barrier_wait(&barrier);
    return NULL;
}

AnimStateData collect_render_data()
{
    AnimStateData data;
    data.building = building;
    data.hydrogen = hydrogen;
    data.N = N;
    data.oxygen = oxygen;
    data.t_hydrogen = t_hydrogen;
    data.t_oxygen = t_oxygen;
    data.water = water;
    return data;
}

void *draw_anim(void *v)
{
    while (1)
    {
        pthread_mutex_lock(&display);
        draw_screen(collect_render_data());
        pthread_mutex_unlock(&display);
        sleep(1);
    }
}

int main(int argv, char **argc)
{
    pthread_t *thr, thr_draw;
    int h = 0, o = 0;
    int i = 0;
    int input;

    if (argv > 1)
    {
        input = strtol(argc[1], NULL, 10);
    }
    srandom(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&display, NULL);
    sem_init(&oxyQueue, 0, 0);
    sem_init(&hydroQueue, 0, 0);
    pthread_barrier_init(&barrier, NULL, 3);

    presentation();
    screen_init();

    N = (get_screen_column() / 6) * 3;
    if ((argv > 1) && (input < N))
        N = input;

    thr = malloc(sizeof(pthread_t) * N);

    pthread_create(&thr_draw, NULL, draw_anim, NULL);

    for (i = 0; i < N; i++)
    {
        if (random() % 3 < 2)
        {
            pthread_create(&thr[i], NULL, hydrogen_thread, NULL);
            h++;
        }
        else
        {
            pthread_create(&thr[i], NULL, oxygen_thread, NULL);
            o++;
        }
    }

    t_hydrogen = h;
    t_oxygen = o;

    getchar();
    show_cursor();
    return 0;
}
