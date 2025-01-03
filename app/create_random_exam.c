#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

// Cấu trúc để lưu thông tin tạo đề thi
typedef struct {
    GtkWidget *title_entry;
    GtkWidget *subject_combo;
    GtkWidget *easy_entry;
    GtkWidget *normal_entry;
    GtkWidget *hard_entry;
    GtkWidget *stack; // Thêm stack vào struct
} CreateExamData;

// Hàm hiển thị popup thông báo
void show_success_popup() {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL,
                                   GTK_DIALOG_MODAL,
                                   GTK_MESSAGE_INFO,
                                   GTK_BUTTONS_OK,
                                   "Create exam successfully!");
    gtk_window_set_title(GTK_WINDOW(dialog), "Success");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Hàm xử lý khi nhấn nút "Create"
void on_create_random_exam_clicked(GtkButton *button, gpointer user_data) {
    CreateExamData *data = (CreateExamData *)user_data;

    // Lấy thông tin từ các widget nhập liệu
    const gchar *title = gtk_entry_get_text(GTK_ENTRY(data->title_entry));
    const gchar *subject = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->subject_combo));
    const gchar *easy_str = gtk_entry_get_text(GTK_ENTRY(data->easy_entry));
    const gchar *normal_str = gtk_entry_get_text(GTK_ENTRY(data->normal_entry));
    const gchar *hard_str = gtk_entry_get_text(GTK_ENTRY(data->hard_entry));

    // Kiểm tra thông tin nhập (đơn giản)
    if (strlen(title) == 0 || subject == NULL || strlen(easy_str) == 0 || strlen(normal_str) == 0 || strlen(hard_str) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Please fill all fields!");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    int easy = atoi(easy_str);
    int normal = atoi(normal_str);
    int hard = atoi(hard_str);

    // In ra thông tin đã nhập (cho mục đích debug)
    g_print("Title: %s\n", title);
    g_print("Subject: %s\n", subject);
    g_print("Easy: %d\n", easy);
    g_print("Normal: %d\n", normal);
    g_print("Hard: %d\n", hard);

    // TODO: Xử lý logic tạo đề thi ngẫu nhiên ở đây
    // ...

    // Hiển thị popup thành công
    show_success_popup();
    gtk_stack_set_visible_child_name(GTK_STACK(data->stack), "create_exam_page");
}

// Hàm xử lý khi nhấn nút "Back"
void on_back_from_random_exam_clicked(GtkButton *button, gpointer user_data) {
    CreateExamData *data = (CreateExamData *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(data->stack), "create_exam_page");
}

// Hàm tạo giao diện "Create Random Exam"
GtkWidget *create_random_exam_form(GtkWidget *stack) {
    GtkWidget *grid;
    GtkWidget *title_label, *subject_label, *easy_label, *normal_label, *hard_label;
    GtkWidget *back_button, *create_button;
    CreateExamData *data;

    // Cấp phát bộ nhớ cho dữ liệu và lưu stack vào đó
    data = g_malloc(sizeof(CreateExamData));
    data->stack = stack;

    // Tạo lưới
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Label và Entry cho tiêu đề
    title_label = gtk_label_new("Enter the title of exam:");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0);
    data->title_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), data->title_entry, 1, 0, 1, 1);

    // Label và ComboBox cho môn học
    subject_label = gtk_label_new("Choose a subject:");
    gtk_label_set_xalign(GTK_LABEL(subject_label), 0);
    data->subject_combo = gtk_combo_box_text_new();
    // Thêm các môn học vào ComboBox
    const char *subjects[] = {"Medicine", "Pharmacology", "Microbiology", "Surgery", "Pediatrics", "Anaesthesia",
                             "Gynaecology & Obstetrics", "Psychiatry", "Radiology", "Unknown", "Pathology",
                             "Orthopaedics", "Social & Preventive Medicine", "Anatomy", "Physiology", "Dental",
                             "Skin", "ENT", "Forensic Medicine", "Biochemistry", "Ophthalmology"};
    for (int i = 0; i < sizeof(subjects) / sizeof(subjects[0]); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->subject_combo), subjects[i]);
    }
    gtk_grid_attach(GTK_GRID(grid), subject_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), data->subject_combo, 1, 1, 1, 1);

    // Label và Entry cho số lượng câu hỏi dễ
    easy_label = gtk_label_new("Enter the number of easy questions:");
    gtk_label_set_xalign(GTK_LABEL(easy_label), 0);
    data->easy_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), easy_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), data->easy_entry, 1, 2, 1, 1);

    // Label và Entry cho số lượng câu hỏi trung bình
    normal_label = gtk_label_new("Enter the number of normal questions:");
    gtk_label_set_xalign(GTK_LABEL(normal_label), 0);
    data->normal_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), normal_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), data->normal_entry, 1, 3, 1, 1);

    // Label và Entry cho số lượng câu hỏi khó
    hard_label = gtk_label_new("Enter the number of hard questions:");
    gtk_label_set_xalign(GTK_LABEL(hard_label), 0);
    data->hard_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), hard_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), data->hard_entry, 1, 4, 1, 1);

    // Nút Back
    back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_from_random_exam_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, 5, 1, 1);

    // Nút Create
    create_button = gtk_button_new_with_label("Create");
    g_signal_connect(create_button, "clicked", G_CALLBACK(on_create_random_exam_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), create_button, 1, 5, 1, 1);

    return grid;
}

// Hàm activate (giả định bạn có hàm này trong ứng dụng chính)
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *create_random_exam_page;
    GtkWidget *stack;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Create Random Exam");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 350);

    // Tạo stack
    stack = gtk_stack_new();

    // Tạo trang "Create Random Exam" và thêm vào stack
    create_random_exam_page = create_random_exam_form(stack);
    gtk_stack_add_named(GTK_STACK(stack), create_random_exam_page, "create_random_exam_page"); 

    gtk_container_add(GTK_CONTAINER(window), stack);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.create_random_exam", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}