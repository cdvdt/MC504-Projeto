#include "anim.h"

pthread_mutex_t mutex;
pthread_mutex_t display;
int oxygen = 0;
int hydrogen = 0;
pthread_barrier_t barrier;
sem_t oxyQueue;
sem_t hydroQueue;
int water = 0;
int building = 0;
/*int t_oxygen;
int t_hydrogen;
int N;*/

void bond(int i){
    building = 1; sleep (1);
    /*if (i == 1) printf("ligando oxigenio\n");
    else printf("ligando hidrogenio\n");*/
}

void* oxygen_thread (void *v) {
    pthread_mutex_lock(&mutex); //mutex.wait();

    oxygen += 1;

    //printf("O disponivel: %d\n", oxygen);

    sleep(random() % 3 + 1);

    if (hydrogen >= 2) {
        pthread_mutex_lock(&display);
        sem_post(&hydroQueue);
        sem_post(&hydroQueue); //hydroQueue.signal(2)
        hydrogen -= 2;
        sem_post(&oxyQueue); //oxyQueue.signal()
        oxygen -= 1;
        pthread_mutex_unlock(&display);
    }
    else
        pthread_mutex_unlock(&mutex); //mutex.signal();

    sem_wait(&oxyQueue); //oxyQueue.wait()
    bond(1);

    pthread_barrier_wait(&barrier); //barrier.wait()

    water++;
    building = 0;
    //printf("%d moleculas passaram pela barreira\n", water);

    pthread_mutex_unlock(&mutex); //mutex.signal()
    return NULL;
}

void* hydrogen_thread (void *v) {
    pthread_mutex_lock(&mutex); //mutex.wait()

    hydrogen += 1;

    //printf("H disponivel: %d\n", hydrogen);

    sleep(random() % 3);

    if ((hydrogen >= 2) && (oxygen >= 1)) {
            pthread_mutex_lock(&display);
            sem_post(&hydroQueue);
            sem_post(&hydroQueue); //hydroQueue.signal(2)
            hydrogen -= 2;
            sem_post(&oxyQueue); //oxyQueue.signal()
            oxygen -= 1;
            pthread_mutex_unlock(&display);
    }
    else
            pthread_mutex_unlock(&mutex); //mutex.signal()

    sem_wait(&hydroQueue); //hydroQueue.wait()
    bond(2);

    pthread_barrier_wait(&barrier); //barrier.wait()
    return NULL;
}

void* draw_anim(void *v) {
    while (1) {
        pthread_mutex_lock(&display);
        draw_screen();
        //if ((hydrogen + oxygen + 3 * water) == 3 * N) exit(0);
        pthread_mutex_unlock(&display);
        sleep(1);
    }
}

int main(int argv, char **argc) {
    pthread_t *thr, thr_draw;
    int h = 0, o = 0;
    int i = 0;
    int input;

    if (argv > 1) {
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
    if ((argv > 1) && (input < N)) N = input;
    
    thr = malloc(sizeof(pthread_t) * N);

    pthread_create(&thr_draw, NULL, draw_anim, NULL);

    for (i = 0; i < N; i++) {
        if (random() % 3 < 2) {
            pthread_create(&thr[i], NULL, hydrogen_thread, NULL);
            h++;
        }
        else {
            pthread_create(&thr[i], NULL, oxygen_thread, NULL);
            o++;
        }
    }

    t_hydrogen = h;
    t_oxygen = o;
    //printf("%d hidrogenio(s), %d oxigenio(s)\n", h, o);

    getchar();
    show_cursor();
    return 0;
    //pthread_exit(NULL);
}
