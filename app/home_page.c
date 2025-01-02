#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

extern GtkWidget *home_page;
extern GtkWidget *stack;

// Function to handle the "Go to Signup" button click
void on_go_to_signup_from_homepage(GtkButton *button, gpointer user_data) {
    // Switch to the signup page when the button is clicked
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "signup");
}

// Function to handle the "Go to Login" button click
void on_go_to_login_from_homepage(GtkButton *button, gpointer user_data) {
    // Switch to the login page when the button is clicked
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
}

void on_exit_app(GtkButton* button, gpointer user_data){
    gtk_main_quit();
}

GtkWidget* create_home_page(int client_sockfd){
    home_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkButton* login_button = gtk_button_new_with_label("1. Login");
    GtkButton* signup_button = gtk_button_new_with_label("2. Signup");
    GtkButton* exit_button = gtk_button_new_with_label("3. Exit");

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_go_to_login_from_homepage), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(on_go_to_signup_from_homepage), NULL);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_app), NULL);

    gtk_box_pack_start(GTK_BOX(home_page), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(home_page), signup_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(home_page), exit_button, FALSE, FALSE, 0);

    return home_page;
}
