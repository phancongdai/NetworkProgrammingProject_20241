#include "main_app.h"

extern GtkWidget *main_window;
extern GtkWidget *stack;

void create_main_window(int client_sockfd) {
    gtk_init(NULL, NULL);  // Initialize GTK

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Online examination system");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    // Create the stack widget and its pages (screens)
    stack = gtk_stack_new();
    gtk_stack_set_homogeneous(GTK_STACK(stack), TRUE);  // All pages will be the same size

    // Create login and signup pages
    GtkWidget* home_page_screen = create_home_page(client_sockfd);
    GtkWidget *login_screen = create_login_page(client_sockfd);
    GtkWidget *signup_screen = create_signup_page(client_sockfd);
    
    // Add the pages to the stack
    gtk_stack_add_named(GTK_STACK(stack), home_page_screen, "homepage");
    gtk_stack_add_named(GTK_STACK(stack), login_screen, "login");
    gtk_stack_add_named(GTK_STACK(stack), signup_screen, "signup");

    // Set the default page
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "homepage");
    
    // Add the stack to the window
    gtk_container_add(GTK_CONTAINER(main_window), stack);
    gtk_widget_show_all(main_window);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Close the application when the window is closed

    gtk_main();
}