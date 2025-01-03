#include <gtk/gtk.h>

// Callback function for button click event
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkLabel *label = GTK_LABEL(data);  // Get the label widget
    gtk_label_set_text(label, "Button Clicked!");  // Update label text
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *box;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to pack widgets
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create a label
    label = gtk_label_new("Hello, GTK!");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    // Create a button
    button = gtk_button_new_with_label("Click Me");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), label);  // Connect button to callback
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
