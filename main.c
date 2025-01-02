#include <gtk/gtk.h>
//#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *buttons[3][3];
    GtkWidget *store_buttons[7];
    int map[3][3];
    int store[7];
} GameState;

GameState game;

void init_game() {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            game.map[r][c] = r + 1;
        }
    }
    
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int r1 = rand() % 3;
            int c1 = rand() % 3;
            int t = game.map[r1][c1];
            game.map[r1][c1] = game.map[r][c];
            game.map[r][c] = t;
        }
    }

    for (int i = 0; i < 7; i++) {
        game.store[i] = 0;
    }
}

int main(void) {
   
}