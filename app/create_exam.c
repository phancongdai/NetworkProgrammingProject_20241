#include <gtk/gtk.h>

extern GtkWidget *stack;

void on_create_random_exam_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý tạo đề thi ngẫu nhiên
}

void on_create_from_questions_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý tạo đề thi từ câu hỏi đã chọn
}

void on_back_from_create_exam_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "manage_exam_page");
}

GtkWidget* create_create_exam() {
  GtkWidget *create_exam_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget *create_random_exam_button = gtk_button_new_with_label("1. Create a random exam");
  GtkWidget *create_from_questions_button = gtk_button_new_with_label("2. Create exam from chosen question");
  GtkWidget *back_button = gtk_button_new_with_label("3. Back");

  g_signal_connect(create_random_exam_button, "clicked", G_CALLBACK(on_create_random_exam_clicked), NULL);
  g_signal_connect(create_from_questions_button, "clicked", G_CALLBACK(on_create_from_questions_clicked), NULL);
  g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_from_create_exam_clicked), NULL);

  gtk_box_pack_start(GTK_BOX(create_exam_menu), create_random_exam_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(create_exam_menu), create_from_questions_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(create_exam_menu), back_button, FALSE, FALSE, 0);

  return create_exam_menu;
}