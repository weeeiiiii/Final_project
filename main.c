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

static GameState game;

static void init_game() {
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

static void app_activate(GtkApplication *app, gpointer user_data) {
    // 創建主窗口
    game.window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(game.window), "sheepNsheep");
    gtk_window_set_default_size(GTK_WINDOW(game.window), WIDTH, HEIGHT);

    // 創建overlay容器
    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(game.window), overlay);

    // 創建並設置背景圖片
    GtkWidget *background = gtk_image_new_from_file("C:/Users/wei/Desktop/sheepppp/gtk/images/background/item.png");
    gtk_widget_set_hexpand(background, TRUE);
    gtk_widget_set_vexpand(background, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), background);

    // 創建主遊戲元素容器並置中
    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), center_box);

    // 創建標題標籤並設置樣式
    GtkWidget *title_label = gtk_label_new(NULL);
    // 使用Pango標記語言設置標題樣式
    const char *markup = "<span font='24' foreground='#006000' weight='bold'>sheepNsheep</span>";
    gtk_label_set_markup(GTK_LABEL(title_label), markup);
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    
    // 可選：添加陰影效果（如果需要的話）
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(title_box), title_label);
    gtk_box_append(GTK_BOX(center_box), title_box);

    // 創建遊戲網格並置中
    game.grid = gtk_grid_new();
    gtk_widget_set_halign(game.grid, GTK_ALIGN_CENTER);
    gtk_grid_set_row_spacing(GTK_GRID(game.grid), INTERVAL);
    gtk_grid_set_column_spacing(GTK_GRID(game.grid), INTERVAL);
    gtk_box_append(GTK_BOX(center_box), game.grid);

    init_game();

    // 創建遊戲按鈕
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            game.buttons[r][c] = gtk_button_new();
            gtk_widget_set_size_request(game.buttons[r][c], TILE_SIZE, TILE_SIZE);
            
            gtk_widget_set_halign(game.buttons[r][c], GTK_ALIGN_CENTER);
            gtk_widget_set_valign(game.buttons[r][c], GTK_ALIGN_CENTER);

            char filename[32];
            sprintf(filename, "C:/Users/wei/Desktop/sheepppp/gtk/images/item%d.png", game.map[r][c]);
            GtkWidget *image = gtk_image_new_from_file(filename);
            gtk_image_set_pixel_size(GTK_IMAGE(image), TILE_SIZE);
            
            gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
            gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
            
            gtk_button_set_child(GTK_BUTTON(game.buttons[r][c]), image);
            
            g_signal_connect(game.buttons[r][c], "clicked",
                           G_CALLBACK(button_clicked),
                           GINT_TO_POINTER(r * 3 + c));
            
            gtk_grid_attach(GTK_GRID(game.grid), game.buttons[r][c], c, r, 1, 1);
        }
    }

    // 創建存儲按鈕容器並置中
    GtkWidget *store_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(store_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(store_box, 10);
    gtk_box_append(GTK_BOX(center_box), store_box);
    
    // 創建存儲按鈕
    for (int i = 0; i < 7; i++) {
        game.store_buttons[i] = gtk_button_new();
        gtk_widget_set_size_request(game.store_buttons[i], TILE_SIZE, TILE_SIZE);
        gtk_widget_set_halign(game.store_buttons[i], GTK_ALIGN_CENTER);
        gtk_widget_set_valign(game.store_buttons[i], GTK_ALIGN_CENTER);
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