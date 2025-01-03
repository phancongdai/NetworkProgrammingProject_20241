#include <gtk/gtk.h>
#include "../client/client.h"
#include <string.h>
#include <stdlib.h>
// #include "menu_screen.c"
// #include "menu_admin_screen.c"

#define WIDTH 600
#define HEIGHT 800

typedef struct {
    GtkWidget* username;
    GtkWidget* password;
    GtkTextBuffer* client_sockfd;
} login_request;

extern login_data data;

extern GtkWidget *login_page;
extern GtkWidget *signup_page;
extern GtkWidget* stack;

void show_toast_for_login_screen(GtkWidget *window, const char *message) {
    GtkWidget *toast_label = gtk_label_new(message);
    
    // Center the toast at the bottom of the window (adjust as needed)
    gtk_widget_set_halign(toast_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(toast_label, GTK_ALIGN_END);

    // Add the toast label to the window
    gtk_container_add(GTK_CONTAINER(window), toast_label);

    // Show the toast message
    gtk_widget_show(toast_label);

    // Set the timeout to hide the toast after 3 seconds
    g_timeout_add(3000, (GSourceFunc) gtk_widget_hide, toast_label);
}

// Function to handle the "Login" button click
void on_login_clicked(GtkButton *button, gpointer request) {
    login_request* user_request = (login_request*) request;
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter(user_request->client_sockfd, &start_iter);
    gtk_text_buffer_get_end_iter(user_request->client_sockfd, &end_iter);
    char* username = gtk_entry_get_text(GTK_ENTRY(user_request->username));
    char* password = gtk_entry_get_text(GTK_ENTRY(user_request->password));
    username[strlen(username)] = '\0';
    password[strlen(password)] = '\0';
    data.opcode = 100;
    strcpy(data.username, username);
    strcpy(data.password, password);
    data.username[strlen(data.username)] = '\0';
    data.password[strlen(data.password)] = '\0';

    int client_sockfd = atoi(gtk_text_buffer_get_text(user_request->client_sockfd, &start_iter, &end_iter, FALSE));
    login_server_response response = login(client_sockfd);
    int check = 0;
    if(response.valid == 1) {
        if(response.previlege == 0){
            check = 1;
            // printf("Login successfully!\n");
            show_toast_for_login_screen(login_page, "Login successfully!");
            gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu");

            id = response.user_id;
            data.user_id = id;
            // UIMainAppMenu(client_sockfd);
            // break;
        } else {
            check = 1;
            // printf("Admin, login successfully!\n");
            show_toast_for_login_screen(login_page, "Admin, login successfully!");
            gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu_admin");

            id = response.user_id;
            data.user_id = id;
            // UIMainAppMenuAdmin(client_sockfd);
            // break;
        }
    }
    else if (response.valid == 2){
        // printf("Login failed! Account is being used by another user!\n");
        show_toast_for_login_screen(login_page, "Login failed! Account is being used by another user!");
    }
    else{
        // printf("Login failed! Please retry\n");
        show_toast_for_login_screen(login_page, "Login failed! Please retry");
    }
}

// // Function to handle the "Signup" button click
void on_signup_clicked(GtkButton *button, gpointer request) {
    g_print("Signup Button clicked\n");
    // In a real app, handle the signup process here
}


void on_go_to_homepage_from_login_screen(GtkButton* button, gpointer user_data){
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "homepage");
}

// Function to create the login page
GtkWidget* create_login_page(int client_sockfd) {
    login_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    login_request* request = g_malloc(sizeof(login_request));
    
    GtkWidget *username_label = gtk_label_new("Username:");
    GtkWidget *username_entry = gtk_entry_new();
    GtkWidget *password_label = gtk_label_new("Password:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide password
    
    GtkWidget *login_button = gtk_button_new_with_label("Login");

    request->username = username_entry;
    request->password = password_entry;
    char client_sockfd_str[20];
    snprintf(client_sockfd_str, sizeof(client_sockfd_str), "%d", client_sockfd);
    // request->client_sockfd = (GtkWidget*) itoa(client_sockfd);
    request->client_sockfd = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(request->client_sockfd, client_sockfd_str, -1);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), request);
    
    GtkWidget *go_to_homepage_button = gtk_button_new_with_label("Back to homepage");
    
    g_signal_connect(go_to_homepage_button, "clicked", G_CALLBACK(on_go_to_homepage_from_login_screen), NULL);
    
    gtk_box_pack_start(GTK_BOX(login_page), username_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(login_page), username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(login_page), password_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(login_page), password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(login_page), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(login_page), go_to_homepage_button, FALSE, FALSE, 0);
    
    return login_page;
}

// Function to create the signup page
GtkWidget* create_signup_page(int client_sockfd) {
    signup_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    GtkWidget *signup_username_label = gtk_label_new("Username:");
    GtkWidget *signup_username_entry = gtk_entry_new();
    GtkWidget *signup_password_label = gtk_label_new("Password:");
    GtkWidget *signup_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(signup_password_entry), FALSE); // Hide password

    GtkWidget *signup_email_label = gtk_label_new("Email:");
    GtkWidget *signup_email_entry = gtk_entry_new();
    
    GtkWidget *signup_button = gtk_button_new_with_label("Signup");
    g_signal_connect(signup_button, "clicked", G_CALLBACK(on_signup_clicked), NULL);
    
    GtkWidget *go_to_homepage_button = gtk_button_new_with_label("Go to Login");
    g_signal_connect(go_to_homepage_button, "clicked", G_CALLBACK(on_go_to_homepage_from_login_screen), NULL);
    
    gtk_box_pack_start(GTK_BOX(signup_page), signup_username_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), signup_username_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), signup_password_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), signup_password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), signup_email_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), signup_email_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), signup_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_page), go_to_homepage_button, FALSE, FALSE, 0);
    
    return signup_page;
}
