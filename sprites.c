#include "sprites.h"

/*int N;
int oxygen;
int hydrogen;
int water;
int t_oxygen;
int t_hydrogen;*/
int cols;
int rows;

int get_screen_row() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_row;
}

int get_screen_column() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_col;
}


void move_cursor (int l, int c) {
	printf("\x1B[%d;%dH", l, c);
}

void hide_cursor() {
	printf("\x1B[?25l");
}

void show_cursor() {
	printf("\x1B[?25h");
}

void clear_screen() {
	printf("\x1B[2J");
}

int draw_water (int l, int c) {
    move_cursor (l, c+2);
    printf("O");
    move_cursor (l+1, c+1);
    printf("/ \\");
    move_cursor (l+2, c);
    printf("H   H\n");

    return 1;
}

int draw_atom() {
    int i, j, used[N][2];
    int h = hydrogen, o = oxygen;
    int l, c;

    used[0][0] = 0;
    used[0][1] = 0;

    move_cursor(1, 1); printf("reator");

    for (i = 0; i < (oxygen + hydrogen); i++){
        c = random() % (cols - DELTA_C) + 1;
        l = random() % (rows - DELTA_R) + 2;
        for (j = 0; j < i; j++) {
            if (c == used[j][0] && l == used[j][1]){
                j = 0;
                c = random() % (cols - DELTA_C) + 1;
                l = random() % (rows - DELTA_R) + 1;
            }
        }
        //printf("c%dcl%dl\n", c,l);
        used[i][0] = c;
        used[i][1] = l;
        move_cursor(l, c);


        if (o > 0) {
            o--;
            printf("O\n");
        }
        else{
            h--;
            printf("H\n");
        }
    }
    return 1;
}

void screen_init() {
    rows = get_screen_row();
    cols = get_screen_column();
    hide_cursor();
}

int number_of_atoms() {
    return hydrogen + oxygen + 3 * water;
}

int is_possible() {
    if (hydrogen >= 2 && oxygen >=1) return 1;
    else return 0;
}

void presentation () {
    //clear_screen();
    printf("\
O problema apresentado é o Building H2O, do livro The Little Book on Semaphores.\n\
É possível definir o número de átomos gerados através da linha de comando.\n\
USAGE: anim <numero de atomos> \n\n\
Caso a quantidade de átomos não seja definida, o programa produzirá o máximo.\n\
A quantidade de atomos gerados é limitada pelo o tamanho do terminal.\n\
\n\
(pressione enter tecla para continuar...)\n");

    getchar();
}

void draw_screen () {
    int i;
    clear_screen();
    draw_atom();
    move_cursor(rows - DELTA_R + 2, 1); printf("reservatorio");
    move_cursor(1, cols - DELTA_C + 2); printf("H total: %d\n", t_hydrogen);
    move_cursor(2, cols - DELTA_C + 2); printf("O total: %d\n", t_oxygen);
    move_cursor(4, cols - DELTA_C + 2); printf("H disponivel: %d\n", hydrogen);
    move_cursor(5, cols - DELTA_C + 2); printf("O disponivel: %d\n", oxygen);
    move_cursor(6, cols - DELTA_C + 2); printf("H2O formados: %d\n", water);
    if (building == 1) {move_cursor(8, cols - DELTA_C + 2); printf("construindo...\n");}
    if (number_of_atoms() == N && is_possible() == 0){
        move_cursor(8, cols - DELTA_C + 2); printf("nao e possivel\n");
        move_cursor(9, cols - DELTA_C + 2); printf("construir mais\n");
        move_cursor(10, cols - DELTA_C + 2); printf("moleculas...\n");
    }

    move_cursor(rows-1, 1); printf("(pressione enter tecla para encerrar...)\n");
    for (i = 0; i < water; i++) draw_water(rows - DELTA_R + 3, 6*i + 1);
}
