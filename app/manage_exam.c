#include <gtk/gtk.h>

extern GtkWidget *stack;

void on_search_exam_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý tìm kiếm đề thi
}

void on_create_exam_clicked(GtkButton *button, gpointer user_data) {
  // Chuyển đến trang tạo đề thi
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "create_exam_page");
}

void on_edit_exam_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý sửa đề thi
}

void on_back_from_manage_exam_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu_admin_page");
}

GtkWidget* create_manage_exam() {
  GtkWidget *manage_exam_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget *search_exam_button = gtk_button_new_with_label("1. Search Exam");
  GtkWidget *create_exam_button = gtk_button_new_with_label("2. Create new Exam");
  GtkWidget *edit_exam_button = gtk_button_new_with_label("3. Edit Existing Exam");
  GtkWidget *back_button = gtk_button_new_with_label("4. Back");

  g_signal_connect(search_exam_button, "clicked", G_CALLBACK(on_search_exam_clicked), NULL);
  g_signal_connect(create_exam_button, "clicked", G_CALLBACK(on_create_exam_clicked), NULL);
  g_signal_connect(edit_exam_button, "clicked", G_CALLBACK(on_edit_exam_clicked), NULL);
  g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_from_manage_exam_clicked), NULL);

  gtk_box_pack_start(GTK_BOX(manage_exam_menu), search_exam_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(manage_exam_menu), create_exam_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(manage_exam_menu), edit_exam_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(manage_exam_menu), back_button, FALSE, FALSE, 0);

  return manage_exam_menu;
}