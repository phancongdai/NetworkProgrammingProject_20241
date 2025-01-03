#include <gtk/gtk.h>

// Giả định các biến này được khai báo ở nơi khác (ví dụ: file header)
extern GtkWidget *stack;

// Các hàm callback cho các nút (cần được định nghĩa ở nơi khác)
void on_create_room_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý tạo phòng thi
  // ...
  // Ví dụ: Chuyển đến trang tạo phòng thi
  // gtk_stack_set_visible_child_name(GTK_STACK(stack), "create_room_page");
}

void on_delete_room_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý xóa phòng thi
  // ...
}

void on_show_rooms_clicked(GtkButton *button, gpointer user_data) {
  // Xử lý hiển thị danh sách phòng thi
  // ...
}

void on_back_to_main_menu_clicked(GtkButton *button, gpointer user_data) {
  // Quay lại menu chính
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "menu_admin_page");
}

GtkWidget* create_manage_room_menu() {
  GtkWidget *manage_room_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget *create_room_button = gtk_button_new_with_label("1. Create a room");
  GtkWidget *delete_room_button = gtk_button_new_with_label("2. Delete a room");
  GtkWidget *show_rooms_button = gtk_button_new_with_label("3. Show your rooms");
  GtkWidget *back_button = gtk_button_new_with_label("4. Main application system!");

  g_signal_connect(create_room_button, "clicked", G_CALLBACK(on_create_room_clicked), NULL);
  g_signal_connect(delete_room_button, "clicked", G_CALLBACK(on_delete_room_clicked), NULL);
  g_signal_connect(show_rooms_button, "clicked", G_CALLBACK(on_show_rooms_clicked), NULL);
  g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_to_main_menu_clicked), NULL);

  gtk_box_pack_start(GTK_BOX(manage_room_menu), create_room_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(manage_room_menu), delete_room_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(manage_room_menu), show_rooms_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(manage_room_menu), back_button, FALSE, FALSE, 0);

  return manage_room_menu;
}