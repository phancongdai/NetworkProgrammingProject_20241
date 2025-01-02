#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <gtk/gtk.h>
#include "login_screen.c"
#include "home_page.c"

// Declare global variables for the UI components
GtkWidget *main_window;
GtkWidget *stack;
GtkWidget *home_page;
GtkWidget *login_page;
GtkWidget *signup_page;

void create_main_window(int client_sockfd);

#endif