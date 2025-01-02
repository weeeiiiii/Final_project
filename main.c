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

static void button_clicked(GtkButton *button, gpointer data) {
    int pos = GPOINTER_TO_INT(data);
    int row = pos / 3;
    int col = pos % 3;

    if (game.map[row][col] == 0) return;

    for (int i = 0; i < 7; i++) {
        if (game.store[i] == 0) {
            game.store[i] = game.map[row][col];
            char filename[32];
            sprintf(filename, "C:/Users/wei/Desktop/sheepppp/gtk/images/item%d.png", game.store[i]);
            GtkWidget *image = gtk_image_new_from_file(filename);
            gtk_button_set_child(GTK_BUTTON(game.store_buttons[i]), image);
            break;
        }
    }

    int count = 0;
    for (int i = 0; i < 7; i++) {
        if (game.store[i] == game.map[row][col]) count++;
    }

    if (count == 3) {
        for (int i = 0; i < 7; i++) {
            if (game.store[i] == game.map[row][col]) {
                game.store[i] = 0;
                gtk_button_set_child(GTK_BUTTON(game.store_buttons[i]), NULL);
            }
        }
    }

    game.map[row][col] = 0;
    gtk_button_set_child(GTK_BUTTON(game.buttons[row][col]), NULL);
}


int main(void) {
   
}