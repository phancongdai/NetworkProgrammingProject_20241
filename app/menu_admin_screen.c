#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

extern GtkWidget* stack;
extern GtkWidget *menu_admin_page;

GtkWidget* create_menu_admin_page(int client_sockfd){
    menu_admin_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *manage_question_db_button = gtk_button_new_with_label("1. Manage question database(Search, Add, Edit, Delete)");
    GtkWidget *manage_exam_db_button = gtk_button_new_with_label("2. Manage exam database(Search, Add, Edit, Delete)");
    GtkWidget *manage_exam_room_button = gtk_button_new_with_label("3. Manage exam room");
    GtkWidget *see_exam_room_result_button = gtk_button_new_with_label("4. See exam room result");
    GtkWidget *see_user_info_button = gtk_button_new_with_label("5. See user information");
    GtkWidget *logout_button = gtk_button_new_with_label("6. Logout");

    char client_sockfd_str[20];
    snprintf(client_sockfd_str, sizeof(client_sockfd_str), "%d", client_sockfd);
    // request->client_sockfd = (GtkWidget*) itoa(client_sockfd);
    // request->client_sockfd = gtk_text_buffer_new(NULL);
    
    gtk_box_pack_start(GTK_BOX(menu_admin_page), manage_question_db_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_admin_page), manage_exam_db_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_admin_page), manage_exam_room_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_admin_page), see_exam_room_result_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_admin_page), see_user_info_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_admin_page), logout_button, FALSE, FALSE, 0);

    return menu_admin_page;
}