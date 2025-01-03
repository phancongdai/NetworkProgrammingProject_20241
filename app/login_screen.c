#include <gtk/gtk.h>
#include "login_screen.h"
#include "../client/client.h"
#include <string.h>
#include <stdlib.h>
// #include "../client/client.c"

#define WIDTH 600
#define HEIGHT 800

// Declare global variables for the UI components
GtkWidget *main_window;
GtkWidget *stack;
GtkWidget *home_page;
GtkWidget *login_page;
GtkWidget *signup_page;

// login_request request;

// Function to handle the "Login" button click
void on_login_clicked(GtkButton *button, gpointer request) {
    login_request* user_request = (login_request*) request;
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter(user_request->client_sockfd, &start_iter);
    gtk_text_buffer_get_end_iter(user_request->client_sockfd, &end_iter);
    char* username = gtk_entry_get_text(GTK_ENTRY(user_request->username));
    char* password = gtk_entry_get_text(GTK_ENTRY(user_request->password));
    // char* client_sockfd = gtk_entry_get_text(GTK_ENTRY(user_request->client_sockfd));
    // In a real app, validate credentials and perform login actions here
    g_print("User name: %s\n", username);
    g_print("Password: %s\n", password);
    g_print("Sockfd: %s\n", gtk_text_buffer_get_text(user_request->client_sockfd, &start_iter, &end_iter, FALSE));
    username[strlen(username) - 1] = '\0';
    password[strlen(password) - 1] = '\0';
    int client_sockfd = atoi(gtk_text_buffer_get_text(user_request->client_sockfd, &start_iter, &end_iter, FALSE));
    login_server_response response = login(client_sockfd);
    int check = 0;
    if(response.valid == 1) {
        if(response.previlege == 0){
            check = 1;
            printf("Login successfully!\n");
            id = response.user_id;
            data.user_id = id;
            UIMainAppMenu(client_sockfd);
            break;
        } else {
            check = 1;
            printf("Admin, login successfully!\n");
            id = response.user_id;
            data.user_id = id;
            UIMainAppMenuAdmin(client_sockfd);
            break;
        }
    }
    else if (response.valid == 2){
        printf("Login failed! Account is being used by another user!\n");
    }
    else{
        printf("Login failed! Please retry\n");
    }
}

// void on_login_clicked(GtkButton *button, gpointer user_data) {
//     login_request* user_request = (login_request*) user_data;
//     GtkTextIter start_iter, end_iter;
//     gtk_text_buffer_get_start_iter(user_request->client_sockfd, &start_iter);
//     gtk_text_buffer_get_end_iter(user_request->client_sockfd, &end_iter);
//     char* username = gtk_entry_get_text(GTK_ENTRY(user_request->username));
//     char* password = gtk_entry_get_text(GTK_ENTRY(user_request->password));
//     int client_sockfd = atoi(gtk_text_buffer_get_text(user_request->client_sockfd, &start_iter, &end_iter, FALSE));

//     // Set the username and password in the global data structure
//     strcpy(data.username, username);
//     strcpy(data.password, password);
//     data.opcode = 100; // Set the opcode for login

//     // Call the login function to send data to the server
//     login_server_response response = login(client_sockfd);

//     // Check the response from the server
//     if (response.valid == 1) {
//         if (response.previlege == 0) {
//             printf("Login successfully!\n");
//             data.user_id = response.user_id;
//             // Hide the login window
//             gtk_widget_hide(main_window);
//             gtk_main_quit();
//             // Continue with the main application logic for regular users
//             // UIMainAppMenu(client_sockfd);
//         } else {
//             printf("Admin, login successfully!\n");
//             data.user_id = response.user_id;
//             // Hide the login window
//             gtk_widget_hide(main_window);
//             gtk_main_quit();
//             // Continue with the main application logic for admin users
//             // UIMainAppMenuAdmin(client_sockfd);
//         }
//     } else if (response.valid == 2) {
//         printf("Login failed! Account is being used by another user!\n");
//         // You may want to display an error message on the login screen
//     } else {
//         printf("Login failed! Please retry\n");
//         // You may want to display an error message on the login screen
//     }
// }

// void on_login_clicked(GtkButton *button, gpointer user_data) {
//     login_request* user_request = (login_request*) user_data;
//     GtkTextIter start_iter, end_iter;
//     gtk_text_buffer_get_start_iter(user_request->client_sockfd, &start_iter);
//     gtk_text_buffer_get_end_iter(user_request->client_sockfd, &end_iter);
//     char* username = gtk_entry_get_text(GTK_ENTRY(user_request->username));
//     char* password = gtk_entry_get_text(GTK_ENTRY(user_request->password));
//     int client_sockfd = atoi(gtk_text_buffer_get_text(user_request->client_sockfd, &start_iter, &end_iter, FALSE));

//     // Set the username and password in the global data structure
//     strcpy(data.username, username);
//     strcpy(data.password, password);
//     data.opcode = 100; // Set the opcode for login

//     // Call the login function to send data to the server
//     login_server_response response = login(client_sockfd);

//     // Check the response from the server
//     if (response.valid == 1) {
//         if (response.previlege == 0) {
//             printf("Login successfully!\n");
//             data.user_id = response.user_id;
//             // Hide the login window
//             gtk_widget_hide(main_window);
//             // Continue with the main application logic for regular users
//             UIMainAppMenu(client_sockfd);
//         } else {
//             printf("Admin, login successfully!\n");
//             data.user_id = response.user_id;
//             // Hide the login window
//             gtk_widget_hide(main_window);
//             // Continue with the main application logic for admin users
//             UIMainAppMenuAdmin(client_sockfd);
//         }
//     } else if (response.valid == 2) {
//         printf("Login failed! Account is being used by another user!\n");
//         // You may want to display an error message on the login screen
//     } else {
//         printf("Login failed! Please retry\n");
//         // You may want to display an error message on the login screen
//     }
// }

// Function to handle the "Signup" button click
void on_signup_clicked(GtkButton *button, gpointer request) {
    g_print("Signup Button clicked\n");
    // In a real app, handle the signup process here
}

// Function to handle the "Go to Signup" button click
void on_go_to_signup(GtkButton *button, gpointer user_data) {
    // Switch to the signup page when the button is clicked
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "signup");
}

// Function to handle the "Go to Login" button click
void on_go_to_login(GtkButton *button, gpointer user_data) {
    // Switch to the login page when the button is clicked
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
}

void on_go_to_homepage(GtkButton* button, gpointer user_data){
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "homepage");
}

void on_exit_app(GtkButton* button, gpointer user_data){
    gtk_main_quit();
}

GtkWidget* create_home_page(int client_sockfd){
    home_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkButton* login_button = gtk_button_new_with_label("1. Login");
    GtkButton* signup_button = gtk_button_new_with_label("2. Signup");
    GtkButton* exit_button = gtk_button_new_with_label("3. Exit");

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_go_to_login), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(on_go_to_signup), NULL);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_app), NULL);

    gtk_box_pack_start(GTK_BOX(home_page), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(home_page), signup_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(home_page), exit_button, FALSE, FALSE, 0);

    return home_page;
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
    
    g_signal_connect(go_to_homepage_button, "clicked", G_CALLBACK(on_go_to_homepage), NULL);
    
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
    g_signal_connect(go_to_homepage_button, "clicked", G_CALLBACK(on_go_to_homepage), NULL);
    
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

// Function to create the main window and set up the stack
void create_main_window(int client_sockfd) {
    //gtk_init(NULL, NULL);  // Initialize GTK

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

    //gtk_main();
}