#include <gtk/gtk.h>
//#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 500
#define HEIGHT 1000
#define INTERVAL 48
#define TILE_SIZE 48

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

static GameState game;

void app_activate(GtkApplication *app, gpointer user_data) {
    game.window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(game.window), "Match-3 Game");
    gtk_window_set_default_size(GTK_WINDOW(game.window), WIDTH, HEIGHT);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(game.window), box);

    game.grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(game.grid), INTERVAL);
    gtk_grid_set_column_spacing(GTK_GRID(game.grid), INTERVAL);
    gtk_box_append(GTK_BOX(box), game.grid);

    init_game();

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            game.buttons[r][c] = gtk_button_new();
            gtk_widget_set_size_request(game.buttons[r][c], TILE_SIZE, TILE_SIZE);
            
            char filename[32];
            sprintf(filename, "C:/Users/wei/Desktop/sheepppp/gtk/images/item%d.png", game.map[r][c]);
            GtkWidget *image = gtk_image_new_from_file(filename);
            gtk_image_set_pixel_size(GTK_IMAGE(image), TILE_SIZE);
            gtk_button_set_child(GTK_BUTTON(game.buttons[r][c]), image);
            
            g_signal_connect(game.buttons[r][c], "clicked", 
                           G_CALLBACK(button_clicked), 
                           GINT_TO_POINTER(r * 3 + c));
            
            gtk_grid_attach(GTK_GRID(game.grid), game.buttons[r][c], c, r, 1, 1);
        }
    }

    GtkWidget *store_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(box), store_box);

    for (int i = 0; i < 7; i++) {
        game.store_buttons[i] = gtk_button_new();
        gtk_widget_set_size_request(game.store_buttons[i], TILE_SIZE, TILE_SIZE);
        gtk_box_append(GTK_BOX(store_box), game.store_buttons[i]);
    }

    gtk_window_present(GTK_WINDOW(game.window));
}


int main(void) {
   GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    return status;
}