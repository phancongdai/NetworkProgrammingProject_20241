#include <gtk/gtk.h>

typedef struct {
    GtkWidget* username;
    GtkWidget* password;
    GtkTextBuffer* client_sockfd;
} login_request;