#include <gtk/gtk.h>

// Cấu trúc để lưu trữ dữ liệu người dùng
typedef struct {
  gchar *username;
  gchar *email;
  gchar *phone_number;
  gchar *image_path;
} UserData;

// Hàm tạo GtkWidget chứa thông tin người dùng
GtkWidget * create_user_information(UserData *data) {
  GtkWidget *grid;
  GtkWidget *user_image;
  GtkWidget *username_label;
  GtkWidget *email_label;
  GtkWidget *phone_label;

  // Tạo lưới để sắp xếp các widget
  grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

  // Tạo ảnh người dùng
  user_image = gtk_image_new_from_file(data->image_path);
    if (user_image == NULL) {
        fprintf(stderr, "Lỗi: Không thể tải ảnh từ: %s\n", data->image_path);
    }
  gtk_widget_set_halign(user_image, GTK_ALIGN_CENTER);

  // Tạo nhãn tên người dùng
  username_label = gtk_label_new(data->username);
  gtk_label_set_xalign(GTK_LABEL(username_label), 0);

  // Tạo nhãn email
  email_label = gtk_label_new(data->email);
  gtk_label_set_xalign(GTK_LABEL(email_label), 0);

  // Tạo nhãn số điện thoại
  phone_label = gtk_label_new(data->phone_number);
  gtk_label_set_xalign(GTK_LABEL(phone_label), 0);

  // Thêm các widget vào lưới
  gtk_grid_attach(GTK_GRID(grid), user_image, 0, 0, 1, 3); // Ảnh ở cột 0, hàng 0-2
  gtk_grid_attach(GTK_GRID(grid), username_label, 1, 0, 1, 1); // Tên ở cột 1, hàng 0
  gtk_grid_attach(GTK_GRID(grid), email_label, 1, 1, 1, 1); // Email ở cột 1, hàng 1
  gtk_grid_attach(GTK_GRID(grid), phone_label, 1, 2, 1, 1); // SĐT ở cột 1, hàng 2

  return grid;
}

// Hàm được gọi khi nhấn nút "Back"
static void on_back_button_clicked(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *back_button;
  GtkWidget *user_info_widget;
  GtkWidget *image_area; // Vùng chứa ảnh

  // Lấy dữ liệu người dùng từ user_data
  UserData *data = (UserData *)user_data;

  // Tạo cửa sổ chính
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Thông tin người dùng");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);

  // Tạo lưới chính để chứa nút Back, vùng chứa ảnh và thông tin người dùng
  grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  gtk_container_add(GTK_CONTAINER(window), grid);

  // Tạo nút "Back"
  back_button = gtk_button_new_with_label("Back");
  g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), NULL);
  gtk_grid_attach(GTK_GRID(grid), back_button, 0, 0, 1, 1); // Cột 0, hàng 0

  // Tạo vùng chứa ảnh (ở đây vẫn dùng GtkImage)
  image_area = create_user_information(data);
  gtk_grid_attach(GTK_GRID(grid), image_area, 1, 0, 1, 1); // Cột 1, hàng 0

  // Hiển thị tất cả các widget
  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  // Dữ liệu người dùng mẫu (bạn cần thay đổi thông tin này)
  UserData user_data = {
    .username = "Tên người dùng: Nguyễn Văn A",
    .email = "Email: example@example.com",
    .phone_number = "Số điện thoại: 0123456789",
    .image_path = "test.png" // Thay đổi đường dẫn ảnh, sử dụng ảnh test.png cùng thư mục
  };

  // Khởi tạo ứng dụng Gtk
  app = gtk_application_new("com.example.userinfo", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), &user_data);

  // Chạy ứng dụng
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}