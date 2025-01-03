#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <gtk/gtk.h>
#include "login_screen.c"
#include "home_page.c"
#include "menu_screen.c"
#include "menu_admin_screen.c"

// Declare global variables for the UI components
GtkWidget *main_window;
GtkWidget *stack;
GtkWidget *home_page;
GtkWidget *login_page;
GtkWidget *signup_page;
GtkWidget *menu_page;
GtkWidget *menu_admin_page;

void create_main_window(int client_sockfd);

#endif