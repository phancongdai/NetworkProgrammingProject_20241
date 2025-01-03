#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#define MAX_QUERY_LEN 3000

extern GtkWidget* stack;
extern GtkWidget *menu_page;
extern login_data data;

void on_logout_from_menu_clicked(GtkButton *button){
    MYSQL *delete_conn;

    char *server = "localhost";
    char *user = "root";
    char *password = "123456"; 
    char *database = "network_db_01";

    delete_conn = mysql_init(NULL);

    if (!mysql_real_connect(delete_conn, server, user, password, database, 0, NULL, 0)) {
        exit(1);
    }

    char query[MAX_QUERY_LEN];
    strcpy(query, "DELETE FROM `Logged_user` WHERE username = \'");
    strcat(query, data.username);
    strcat(query, "\';");

    if (mysql_query(delete_conn, query)) {
        exit(1);
    }

    mysql_close(delete_conn);

    gtk_stack_set_visible_child_name(GTK_STACK(stack), "homepage");
}

GtkWidget* create_menu_page(int client_sockfd){
    menu_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *take_practice_exam_button = gtk_button_new_with_label("1. Take an practice exam");
    GtkWidget *take_exam_button = gtk_button_new_with_label("2. Take an exam");
    GtkWidget *see_exam_history_button = gtk_button_new_with_label("3. See examination history");
    GtkWidget *see_user_info_button = gtk_button_new_with_label("4. See user information");
    GtkWidget *chat_with_AI_button = gtk_button_new_with_label("5. Chat with AI");
    GtkWidget *logout_button = gtk_button_new_with_label("6. Logout");

    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_from_menu_clicked), NULL);

    char client_sockfd_str[20];
    snprintf(client_sockfd_str, sizeof(client_sockfd_str), "%d", client_sockfd);
    // request->client_sockfd = (GtkWidget*) itoa(client_sockfd);
    // request->client_sockfd = gtk_text_buffer_new(NULL);
    
    gtk_box_pack_start(GTK_BOX(menu_page), take_practice_exam_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_page), take_exam_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_page), see_exam_history_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_page), see_user_info_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_page), chat_with_AI_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_page), logout_button, FALSE, FALSE, 0);

    return menu_page;
}