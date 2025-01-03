// #include <gtk/gtk.h>
// #include <cairo.h>
// #include <math.h>
// #include <string.h>

// #define NUM_EXAMS 5
// double student_scores[NUM_EXAMS] = {7.5, 8.0, 6.5, 9.0, 8.5};
// char *exam_labels[NUM_EXAMS] = {"Exam 1", "Exam 2", "Exam 3", "Exam 4", "Exam 5"};
// #define NUM_STUDENTS 20
// double room_scores[NUM_STUDENTS] = {5.2, 7.8, 6.4, 9.2, 8.6, 5.8, 7.2, 9.8, 6.0, 8.4, 7.6, 5.4, 8.8, 6.8, 9.0, 7.0, 8.2, 6.6, 5.0, 7.4};
// #define NUM_QUESTIONS 10
// int correct_answers_per_question[NUM_QUESTIONS] = {15, 12, 18, 9, 16, 10, 14, 19, 11, 13}; 
// char *question_labels[NUM_QUESTIONS] = {"Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8", "Q9", "Q10"};

// // Kích thước biểu đồ
// #define CHART_WIDTH 400
// #define CHART_HEIGHT 300
// #define MARGIN 50
// #define SCROLLABLE_CHART_WIDTH 600 // Chiều rộng cho biểu đồ có thanh cuộn
// #define Y_AXIS_LABEL_WIDTH 40
// #define X_AXIS_LABEL_HEIGHT 20

// // Hàm vẽ biểu đồ cột cho học sinh
// static void draw_student_bar_chart(cairo_t *cr, int x, int y, int width, int height) {
//     double max_score = 10;

//     double bar_width = (double)(width - Y_AXIS_LABEL_WIDTH) / (NUM_EXAMS * 2);
//     double x_pos = x + Y_AXIS_LABEL_WIDTH + bar_width / 2;

//     // Vẽ trục Y
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
//     cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_stroke(cr);

//     // Vẽ nhãn trục Y và các đường lưới
//     for (int i = 0; i <= 10; i++) {
//         double y_pos = y + height - (double)i / 10 * height;
//         char label[20];
//         snprintf(label, sizeof(label), "%d", i);

//         cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
//         cairo_text_extents_t extents;
//         cairo_text_extents(cr, label, &extents);
//         cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
//         cairo_show_text(cr, label);

//         // Vẽ đường lưới
//           if (i % 2 == 0 && i > 0)
//           {
//               cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
//               cairo_set_line_width(cr, 0.5);
//               cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
//               cairo_line_to(cr, x + width, y_pos);
//               cairo_stroke(cr);
//           }
//     }
    
//     cairo_set_line_width(cr, 1);
//     for (int i = 0; i < NUM_EXAMS; i++) {
//         double bar_height = student_scores[i] / max_score * height;
//         double y_pos = y + height - bar_height;

//         cairo_set_source_rgb(cr, 0.2 + i * 0.1, 0.4 - i * 0.05, 0.8 - i * 0.1);
//         cairo_rectangle(cr, x_pos, y_pos, bar_width, bar_height);
//         cairo_fill(cr);

//         // Vẽ nhãn trục X
//         cairo_set_source_rgb(cr, 0, 0, 0);
//         cairo_move_to(cr, x_pos + bar_width / 2, y + height + X_AXIS_LABEL_HEIGHT);
//         cairo_text_extents_t extents;
//         cairo_text_extents(cr, exam_labels[i], &extents);
//         cairo_rel_move_to(cr, -extents.width / 2, extents.height / 2);
//         cairo_show_text(cr, exam_labels[i]);

//         x_pos += bar_width * 2;
//     }
//     // Vẽ trục X
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_line_to(cr, x + width, y + height);
//     cairo_stroke(cr);
// }

// // Hàm vẽ biểu đồ đường cho học sinh
// static void draw_student_line_chart(cairo_t *cr, int x, int y, int width, int height) {
//     double max_score = 10;
//     double x_step = (double)(width - Y_AXIS_LABEL_WIDTH) / (NUM_EXAMS - 1);
//     double x_pos = x + Y_AXIS_LABEL_WIDTH;

//     // Vẽ trục Y
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
//     cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_stroke(cr);

//     // Vẽ nhãn trục Y và các đường lưới
//     for (int i = 0; i <= 10; i++) {
//       double y_pos = y + height - (double)i / 10 * height;
//       char label[20];
//       snprintf(label, sizeof(label), "%d", i);

//       cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
//       cairo_text_extents_t extents;
//       cairo_text_extents(cr, label, &extents);
//       cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
//       cairo_show_text(cr, label);
//       // Vẽ đường lưới
//         if (i % 2 == 0 && i > 0)
//         {
//             cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
//             cairo_set_line_width(cr, 0.5);
//             cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
//             cairo_line_to(cr, x + width, y_pos);
//             cairo_stroke(cr);
//         }
//     }

//     cairo_set_source_rgb(cr, 0, 0, 1);
//     cairo_set_line_width(cr, 2.0);

//     cairo_move_to(cr, x_pos, y + height - student_scores[0] / max_score * height);

//     for (int i = 1; i < NUM_EXAMS; i++) {
//         x_pos += x_step;
//         cairo_line_to(cr, x_pos, y + height - student_scores[i] / max_score * height);
//     }

//     cairo_stroke(cr);

//     // Vẽ các điểm
//     x_pos = x + Y_AXIS_LABEL_WIDTH;
//     for (int i = 0; i < NUM_EXAMS; i++) {
//         cairo_arc(cr, x_pos, y + height - student_scores[i] / max_score * height, 3, 0, 2 * M_PI);
//         cairo_fill(cr);

//         // Vẽ nhãn trục X
//         cairo_set_source_rgb(cr, 0, 0, 0);
//         cairo_move_to(cr, x_pos, y + height + X_AXIS_LABEL_HEIGHT);
//         cairo_text_extents_t extents;
//         cairo_text_extents(cr, exam_labels[i], &extents);
//         cairo_rel_move_to(cr, -extents.width / 2, extents.height/2);
//         cairo_show_text(cr, exam_labels[i]);

//         x_pos += x_step;
//     }
//     // Vẽ trục X
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_line_to(cr, x + width, y + height);
//     cairo_stroke(cr);
// }

// // Hàm vẽ histogram cho phòng thi
// static void draw_room_histogram(cairo_t *cr, int x, int y, int width, int height) {
//     int histogram[5] = {0}; // 5 bins: 0-2, 2-4, 4-6, 6-8, 8-10
//     int max_freq = 0;

//     for (int i = 0; i < NUM_STUDENTS; i++) {
//         if (room_scores[i] >= 0 && room_scores[i] < 2) histogram[0]++;
//         else if (room_scores[i] >= 2 && room_scores[i] < 4) histogram[1]++;
//         else if (room_scores[i] >= 4 && room_scores[i] < 6) histogram[2]++;
//         else if (room_scores[i] >= 6 && room_scores[i] < 8) histogram[3]++;
//         else if (room_scores[i] >= 8 && room_scores[i] <= 10) histogram[4]++;
//     }

//     for (int i = 0; i < 5; i++) {
//         if (histogram[i] > max_freq) {
//             max_freq = histogram[i];
//         }
//     }

//     double bar_width = (double)(width - Y_AXIS_LABEL_WIDTH) / 10;
//     double x_pos = x + Y_AXIS_LABEL_WIDTH + bar_width / 2;

//     // Vẽ trục Y
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
//     cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_stroke(cr);

//     // Vẽ nhãn trục Y và các đường lưới
//     for (int i = 0; i <= max_freq; i++) {
//       double y_pos = y + height - (double)i / max_freq * height;
//       char label[20];
//       snprintf(label, sizeof(label), "%d", i);

//       cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
//       cairo_text_extents_t extents;
//       cairo_text_extents(cr, label, &extents);
//       cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
//       cairo_show_text(cr, label);

//       // Vẽ đường lưới
//       if (i > 0) {
//         cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
//         cairo_set_line_width(cr, 0.5);
//         cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
//         cairo_line_to(cr, x + width, y_pos);
//         cairo_stroke(cr);
//       }
//     }
//     cairo_set_line_width(cr, 1);
//     for (int i = 0; i < 5; i++) {
//         double bar_height = (double)histogram[i] / max_freq * height;
//         double y_pos = y + height - bar_height;

//         cairo_set_source_rgb(cr, 0.8 - i * 0.1, 0.2 + i * 0.1, 0.4 - i * 0.05);
//         cairo_rectangle(cr, x_pos, y_pos, bar_width, bar_height);
//         cairo_fill(cr);

//         // Vẽ nhãn trục X
//         cairo_set_source_rgb(cr, 0, 0, 0);
//         char label[20];
//         snprintf(label, sizeof(label), "%d-%d", i * 2, (i + 1) * 2);
//         if(i == 4) snprintf(label, sizeof(label), "%d-%d", i * 2, (i + 1) * 2);
//         cairo_move_to(cr, x_pos + bar_width / 2, y + height + X_AXIS_LABEL_HEIGHT);
//         cairo_text_extents_t extents;
//         cairo_text_extents(cr, label, &extents);
//         cairo_rel_move_to(cr, -extents.width / 2, extents.height / 2);
//         cairo_show_text(cr, label);

//         x_pos += bar_width * 2;
//     }

//     // Vẽ trục X
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_line_to(cr, x + width, y + height);
//     cairo_stroke(cr);
// }

// // Hàm vẽ biểu đồ cột cho số câu trả lời đúng của phòng thi
// static void draw_correct_answers_bar_chart(cairo_t *cr, int x, int y, int width, int height) {
//     int max_correct = 0;
//     for (int i = 0; i < NUM_QUESTIONS; i++) {
//         if (correct_answers_per_question[i] > max_correct) {
//             max_correct = correct_answers_per_question[i];
//         }
//     }

//     double bar_width = (double)(width - Y_AXIS_LABEL_WIDTH) / (NUM_QUESTIONS * 2);
//     double x_pos = x + Y_AXIS_LABEL_WIDTH + bar_width / 2;

//     // Vẽ trục Y
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
//     cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_stroke(cr);

//     // Vẽ nhãn trục Y và các đường lưới
//     for (int i = 0; i <= max_correct; i++) {
//         double y_pos = y + height - (double)i / max_correct * height;
//         char label[20];
//         snprintf(label, sizeof(label), "%d", i);

//         cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
//         cairo_text_extents_t extents;
//         cairo_text_extents(cr, label, &extents);
//         cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
//         cairo_show_text(cr, label);

//         // Vẽ đường lưới
//         if (i > 0) {
//             cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
//             cairo_set_line_width(cr, 0.5);
//             cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
//             cairo_line_to(cr, x + width, y_pos);
//             cairo_stroke(cr);
//         }
//     }

//     cairo_set_line_width(cr, 1);
//     for (int i = 0; i < NUM_QUESTIONS; i++) {
//         double bar_height = (double)correct_answers_per_question[i] / max_correct * height;
//         double y_pos = y + height - bar_height;

//         cairo_set_source_rgb(cr, 0.2 + i * 0.05, 0.8 - i * 0.1, 0.4 - i * 0.05);
//         cairo_rectangle(cr, x_pos, y_pos, bar_width, bar_height);
//         cairo_fill(cr);

//         // Vẽ nhãn trục X
//         cairo_set_source_rgb(cr, 0, 0, 0);
//         cairo_move_to(cr, x_pos + bar_width / 2, y + height + X_AXIS_LABEL_HEIGHT);
//         cairo_text_extents_t extents;
//         cairo_text_extents(cr, question_labels[i], &extents);
//         cairo_rel_move_to(cr, -extents.width / 2, extents.height / 2);
//         cairo_show_text(cr, question_labels[i]);

//         x_pos += bar_width * 2;
//     }

//     // Vẽ trục X
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
//     cairo_line_to(cr, x + width, y + height);
//     cairo_stroke(cr);
// }

// // Hàm callback cho sự kiện vẽ của trang học sinh
// static gboolean on_draw_student_page(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
//     cairo_set_source_rgb(cr, 1, 1, 1);
//     cairo_paint(cr);

//     // Vẽ tiêu đề
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
//     cairo_set_font_size(cr, 16);
//     cairo_move_to(cr, MARGIN, MARGIN);
//     cairo_show_text(cr, "Student Charts");
//     int width = gtk_widget_get_allocated_width(widget);

//     draw_student_bar_chart(cr, MARGIN, MARGIN * 2 + 20, width / 2 - 2 * MARGIN, CHART_HEIGHT);
//     draw_student_line_chart(cr, width / 2 + MARGIN, MARGIN * 2 + 20, width / 2 - 2 * MARGIN, CHART_HEIGHT);

//     return FALSE;
// }

// // Hàm callback cho sự kiện vẽ của trang phòng thi (histogram)
// static gboolean on_draw_room_page_histogram(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
//     cairo_set_source_rgb(cr, 1, 1, 1);
//     cairo_paint(cr);
//     // Vẽ tiêu đề
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
//     cairo_set_font_size(cr, 16);
//     cairo_move_to(cr, MARGIN, MARGIN);
//     cairo_show_text(cr, "Exam Room Charts - Histogram");

//     draw_room_histogram(cr, MARGIN, MARGIN * 2 + 20, CHART_WIDTH, CHART_HEIGHT);

//     return FALSE;
// }

// // Hàm callback cho sự kiện vẽ của trang phòng thi (số câu trả lời đúng)
// static gboolean on_draw_room_page_correct_answers(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
//     cairo_set_source_rgb(cr, 1, 1, 1);
//     cairo_paint(cr);
//     // Vẽ tiêu đề
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
//     cairo_set_font_size(cr, 16);
//     cairo_move_to(cr, MARGIN, MARGIN);
//     cairo_show_text(cr, "Exam Room Charts - Correct Answers per Question");

//     draw_correct_answers_bar_chart(cr, MARGIN, MARGIN * 2 + 20, SCROLLABLE_CHART_WIDTH, CHART_HEIGHT);

//     return FALSE;
// }

// // Hàm hiển thị dữ liệu cho học sinh
// static void display_student_data(GtkWidget *treeview) {
//     GtkListStore *store;
//     GtkTreeViewColumn *column;
//     GtkCellRenderer *renderer;

//     store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_DOUBLE);

//     GtkTreeIter iter;
//     for (int i = 0; i < NUM_EXAMS; i++) {
//         gtk_list_store_append(store, &iter);
//         gtk_list_store_set(store, &iter, 0, exam_labels[i], 1, student_scores[i], -1);
//     }

//     gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
//     g_object_unref(store);

//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Exam", renderer, "text", 0, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Score", renderer, "text", 1, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
// }

// // Hàm hiển thị dữ liệu cho phòng thi
// static void display_room_data(GtkWidget *treeview) {
//     GtkListStore *store;
//     GtkTreeViewColumn *column;
//     GtkCellRenderer *renderer;

//     store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_DOUBLE);

//     GtkTreeIter iter;
//     for (int i = 0; i < NUM_STUDENTS; i++) {
//         gtk_list_store_append(store, &iter);
//         gtk_list_store_set(store, &iter, 0, i + 1, 1, room_scores[i], -1);
//     }

//     gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
//     g_object_unref(store);

//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Student ID", renderer, "text", 0, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Score", renderer, "text", 1, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
// }
// // Hàm hiển thị dữ liệu cho câu trả lời đúng
// static void display_correct_answers_data(GtkWidget *treeview) {
//     GtkListStore *store;
//     GtkTreeViewColumn *column;
//     GtkCellRenderer *renderer;

//     store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);

//     GtkTreeIter iter;
//     for (int i = 0; i < NUM_QUESTIONS; i++) {
//         gtk_list_store_append(store, &iter);
//         gtk_list_store_set(store, &iter, 0, question_labels[i], 1, correct_answers_per_question[i], -1);
//     }

//     gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
//     g_object_unref(store);

//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Question", renderer, "text", 0, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

//     renderer = gtk_cell_renderer_text_new();
//     column = gtk_tree_view_column_new_with_attributes("Correct Answers", renderer, "text", 1, NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
// }

// // Callback function for button clicks
// static void on_room_page_button_clicked(GtkButton *button, gpointer user_data) {
//     GtkNotebook *notebook = GTK_NOTEBOOK(user_data);
//     gint current_page = gtk_notebook_get_current_page(notebook);

//     // Switch to the other page
//     if (current_page == 0) {
//         gtk_notebook_set_current_page(notebook, 1);
//     } else {
//         gtk_notebook_set_current_page(notebook, 0);
//     }
// }

// int main(int argc, char *argv[]) {
//     GtkWidget *window;
//     GtkWidget *notebook;
//     GtkWidget *student_page_box;
//     GtkWidget *room_page_box;
//     GtkWidget *student_drawing_area;
//     GtkWidget *student_treeview;
//     GtkWidget *label_student;
//     GtkWidget *label_room;
//     GtkWidget *student_treeview_scrolled_window;
//     GtkWidget *room_notebook;
//     GtkWidget *room_page_histogram_drawing_area;
//     GtkWidget *room_page_correct_answers_drawing_area;
//     GtkWidget *room_page_histogram_treeview;
//     GtkWidget *room_page_correct_answers_treeview;
//     GtkWidget *room_page_histogram_treeview_scrolled_window;
//     GtkWidget *room_page_correct_answers_treeview_scrolled_window;
//     GtkWidget *room_page_histogram_box;
//     GtkWidget *room_page_correct_answers_box;
//     GtkWidget *button_switch_pages;
//     GtkWidget *label_room_histogram;
//     GtkWidget *label_room_correct_answers;

//     gtk_init(&argc, &argv);

//     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//     gtk_window_set_title(GTK_WINDOW(window), "Exam Statistics");
//     gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
//     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//     notebook = gtk_notebook_new();
//     gtk_container_add(GTK_CONTAINER(window), notebook);

//     // Trang cho học sinh
//     student_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
//     student_drawing_area = gtk_drawing_area_new();
//     gtk_widget_set_size_request(student_drawing_area, CHART_WIDTH + MARGIN * 2, CHART_HEIGHT + MARGIN * 3 + 20);
//     g_signal_connect(G_OBJECT(student_drawing_area), "draw", G_CALLBACK(on_draw_student_page), NULL);

//     // Thêm student_drawing_area vào student_page_box
//     gtk_box_pack_start(GTK_BOX(student_page_box), student_drawing_area, FALSE, FALSE, 0);

//     student_treeview = gtk_tree_view_new();
//     display_student_data(student_treeview);

//     // Tạo scrolled window cho bảng dữ liệu học sinh
//     student_treeview_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
//     gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(student_treeview_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//     gtk_widget_set_size_request(student_treeview_scrolled_window, CHART_WIDTH + MARGIN * 4, 250); // Đặt kích thước
//     gtk_container_add(GTK_CONTAINER(student_treeview_scrolled_window), student_treeview);

//     gtk_box_pack_start(GTK_BOX(student_page_box), student_treeview_scrolled_window, FALSE, FALSE, 0);

//     label_student = gtk_label_new("Student Page");
//     gtk_notebook_append_page(GTK_NOTEBOOK(notebook), student_page_box, label_student);

//     // Trang cho phòng thi - tạo notebook con
//     room_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//     room_notebook = gtk_notebook_new();
    
//     // button_switch_pages = gtk_button_new_with_label("Switch Pages");
//     // g_signal_connect(button_switch_pages, "clicked", G_CALLBACK(on_room_page_button_clicked), room_notebook);
//     gtk_box_pack_start(GTK_BOX(room_page_box), button_switch_pages, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(room_page_box), room_notebook, TRUE, TRUE, 0);

//     // Trang 1 cho phòng thi (histogram)
//     room_page_histogram_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//     room_page_histogram_drawing_area = gtk_drawing_area_new();
//     gtk_widget_set_size_request(room_page_histogram_drawing_area, CHART_WIDTH + MARGIN * 2, CHART_HEIGHT + MARGIN * 3 + 20);
//     g_signal_connect(G_OBJECT(room_page_histogram_drawing_area), "draw", G_CALLBACK(on_draw_room_page_histogram), NULL);
//     room_page_histogram_treeview = gtk_tree_view_new();
//     display_room_data(room_page_histogram_treeview);

//     room_page_histogram_treeview_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
//     gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(room_page_histogram_treeview_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//     gtk_widget_set_size_request(room_page_histogram_treeview_scrolled_window, CHART_WIDTH + MARGIN * 4, 250); // Đặt kích thước
//     gtk_container_add(GTK_CONTAINER(room_page_histogram_treeview_scrolled_window), room_page_histogram_treeview);

//     gtk_box_pack_start(GTK_BOX(room_page_histogram_box), room_page_histogram_drawing_area, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(room_page_histogram_box), room_page_histogram_treeview_scrolled_window, FALSE, FALSE, 0);

//     label_room_histogram = gtk_label_new("Histogram");
//     gtk_notebook_append_page(GTK_NOTEBOOK(room_notebook), room_page_histogram_box, label_room_histogram);

//     // Trang 2 cho phòng thi (số câu trả lời đúng)
//     room_page_correct_answers_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//     room_page_correct_answers_drawing_area = gtk_drawing_area_new();
//     gtk_widget_set_size_request(room_page_correct_answers_drawing_area, SCROLLABLE_CHART_WIDTH + MARGIN * 2, CHART_HEIGHT + MARGIN * 3 + 20);
//     g_signal_connect(G_OBJECT(room_page_correct_answers_drawing_area), "draw", G_CALLBACK(on_draw_room_page_correct_answers), NULL);
//     room_page_correct_answers_treeview = gtk_tree_view_new();
//     display_correct_answers_data(room_page_correct_answers_treeview);

//     room_page_correct_answers_treeview_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
//     gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(room_page_correct_answers_treeview_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//     gtk_widget_set_size_request(room_page_correct_answers_treeview_scrolled_window, SCROLLABLE_CHART_WIDTH + MARGIN * 4, 250); // Đặt kích thước
//     gtk_container_add(GTK_CONTAINER(room_page_correct_answers_treeview_scrolled_window), room_page_correct_answers_treeview);

//     gtk_box_pack_start(GTK_BOX(room_page_correct_answers_box), room_page_correct_answers_drawing_area, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(room_page_correct_answers_box), room_page_correct_answers_treeview_scrolled_window, FALSE, FALSE, 0);

//     label_room_correct_answers = gtk_label_new("Correct Answers");
//     gtk_notebook_append_page(GTK_NOTEBOOK(room_notebook), room_page_correct_answers_box, label_room_correct_answers);

//     label_room = gtk_label_new("Room Page");
//     gtk_notebook_append_page(GTK_NOTEBOOK(notebook), room_page_box, label_room);

//     gtk_widget_show_all(window);

//     gtk_main();

//     return 0;
// }



#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <string.h>

#define NUM_EXAMS 5
double student_scores[NUM_EXAMS] = {7.5, 8.0, 6.5, 9.0, 8.5};
char *exam_labels[NUM_EXAMS] = {"Exam 1", "Exam 2", "Exam 3", "Exam 4", "Exam 5"};
#define NUM_STUDENTS 20
double room_scores[NUM_STUDENTS] = {5.2, 7.8, 6.4, 9.2, 8.6, 5.8, 7.2, 9.8, 6.0, 8.4, 7.6, 5.4, 8.8, 6.8, 9.0, 7.0, 8.2, 6.6, 5.0, 7.4};
#define NUM_QUESTIONS 10
int correct_answers_per_question[NUM_QUESTIONS] = {15, 12, 18, 9, 16, 10, 14, 19, 11, 13}; 
char *question_labels[NUM_QUESTIONS] = {"Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8", "Q9", "Q10"};

// Kích thước biểu đồ
#define CHART_WIDTH 400
#define CHART_HEIGHT 300
#define MARGIN 50
#define SCROLLABLE_CHART_WIDTH 600 // Chiều rộng cho biểu đồ có thanh cuộn
#define Y_AXIS_LABEL_WIDTH 40
#define X_AXIS_LABEL_HEIGHT 20


// Hàm vẽ biểu đồ cột cho học sinh
static void draw_student_bar_chart(cairo_t *cr, int x, int y, int width, int height) {
    double max_score = 10;

    double bar_width = (double)(width - Y_AXIS_LABEL_WIDTH) / (NUM_EXAMS * 2);
    double x_pos = x + Y_AXIS_LABEL_WIDTH + bar_width / 2;

    // Vẽ trục Y
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
    cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_stroke(cr);

    // Vẽ nhãn trục Y và các đường lưới
    for (int i = 0; i <= 10; i++) {
        double y_pos = y + height - (double)i / 10 * height;
        char label[20];
        snprintf(label, sizeof(label), "%d", i);

        cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, label, &extents);
        cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
        cairo_show_text(cr, label);

        // Vẽ đường lưới
          if (i % 2 == 0 && i > 0)
          {
              cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
              cairo_set_line_width(cr, 0.5);
              cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
              cairo_line_to(cr, x + width, y_pos);
              cairo_stroke(cr);
          }
    }
    
    cairo_set_line_width(cr, 1);
    for (int i = 0; i < NUM_EXAMS; i++) {
        double bar_height = student_scores[i] / max_score * height;
        double y_pos = y + height - bar_height;

        cairo_set_source_rgb(cr, 0.2 + i * 0.1, 0.4 - i * 0.05, 0.8 - i * 0.1);
        cairo_rectangle(cr, x_pos, y_pos, bar_width, bar_height);
        cairo_fill(cr);

        // Vẽ nhãn trục X
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, x_pos + bar_width / 2, y + height + X_AXIS_LABEL_HEIGHT);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, exam_labels[i], &extents);
        cairo_rel_move_to(cr, -extents.width / 2, extents.height / 2);
        cairo_show_text(cr, exam_labels[i]);

        x_pos += bar_width * 2;
    }
    // Vẽ trục X
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_line_to(cr, x + width, y + height);
    cairo_stroke(cr);
}

// Hàm vẽ biểu đồ đường cho học sinh
static void draw_student_line_chart(cairo_t *cr, int x, int y, int width, int height) {
    double max_score = 10;
    double x_step = (double)(width - Y_AXIS_LABEL_WIDTH) / (NUM_EXAMS - 1);
    double x_pos = x + Y_AXIS_LABEL_WIDTH;

    // Vẽ trục Y
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
    cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_stroke(cr);

    // Vẽ nhãn trục Y và các đường lưới
    for (int i = 0; i <= 10; i++) {
      double y_pos = y + height - (double)i / 10 * height;
      char label[20];
      snprintf(label, sizeof(label), "%d", i);

      cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
      cairo_text_extents_t extents;
      cairo_text_extents(cr, label, &extents);
      cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
      cairo_show_text(cr, label);
      // Vẽ đường lưới
        if (i % 2 == 0 && i > 0)
        {
            cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
            cairo_set_line_width(cr, 0.5);
            cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
            cairo_line_to(cr, x + width, y_pos);
            cairo_stroke(cr);
        }
    }

    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr, 2.0);

    cairo_move_to(cr, x_pos, y + height - student_scores[0] / max_score * height);

    for (int i = 1; i < NUM_EXAMS; i++) {
        x_pos += x_step;
        cairo_line_to(cr, x_pos, y + height - student_scores[i] / max_score * height);
    }

    cairo_stroke(cr);

    // Vẽ các điểm
    x_pos = x + Y_AXIS_LABEL_WIDTH;
    for (int i = 0; i < NUM_EXAMS; i++) {
        cairo_arc(cr, x_pos, y + height - student_scores[i] / max_score * height, 3, 0, 2 * M_PI);
        cairo_fill(cr);

        // Vẽ nhãn trục X
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, x_pos, y + height + X_AXIS_LABEL_HEIGHT);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, exam_labels[i], &extents);
        cairo_rel_move_to(cr, -extents.width / 2, extents.height/2);
        cairo_show_text(cr, exam_labels[i]);

        x_pos += x_step;
    }
    // Vẽ trục X
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_line_to(cr, x + width, y + height);
    cairo_stroke(cr);
}

// Hàm vẽ histogram cho phòng thi
static void draw_room_histogram(cairo_t *cr, int x, int y, int width, int height) {
    int histogram[5] = {0}; // 5 bins: 0-2, 2-4, 4-6, 6-8, 8-10
    int max_freq = 0;

    for (int i = 0; i < NUM_STUDENTS; i++) {
        if (room_scores[i] >= 0 && room_scores[i] < 2) histogram[0]++;
        else if (room_scores[i] >= 2 && room_scores[i] < 4) histogram[1]++;
        else if (room_scores[i] >= 4 && room_scores[i] < 6) histogram[2]++;
        else if (room_scores[i] >= 6 && room_scores[i] < 8) histogram[3]++;
        else if (room_scores[i] >= 8 && room_scores[i] <= 10) histogram[4]++;
    }

    for (int i = 0; i < 5; i++) {
        if (histogram[i] > max_freq) {
            max_freq = histogram[i];
        }
    }

    double bar_width = (double)(width - Y_AXIS_LABEL_WIDTH) / 10;
    double x_pos = x + Y_AXIS_LABEL_WIDTH + bar_width / 2;

    // Vẽ trục Y
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
    cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_stroke(cr);

    // Vẽ nhãn trục Y và các đường lưới
    for (int i = 0; i <= max_freq; i++) {
      double y_pos = y + height - (double)i / max_freq * height;
      char label[20];
      snprintf(label, sizeof(label), "%d", i);

      cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
      cairo_text_extents_t extents;
      cairo_text_extents(cr, label, &extents);
      cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
      cairo_show_text(cr, label);

      // Vẽ đường lưới
      if (i > 0) {
        cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
        cairo_set_line_width(cr, 0.5);
        cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
        cairo_line_to(cr, x + width, y_pos);
        cairo_stroke(cr);
      }
    }
    cairo_set_line_width(cr, 1);
    for (int i = 0; i < 5; i++) {
        double bar_height = (double)histogram[i] / max_freq * height;
        double y_pos = y + height - bar_height;

        cairo_set_source_rgb(cr, 0.8 - i * 0.1, 0.2 + i * 0.1, 0.4 - i * 0.05);
        cairo_rectangle(cr, x_pos, y_pos, bar_width, bar_height);
        cairo_fill(cr);

        // Vẽ nhãn trục X
        cairo_set_source_rgb(cr, 0, 0, 0);
        char label[20];
        snprintf(label, sizeof(label), "%d-%d", i * 2, (i + 1) * 2);
        if(i == 4) snprintf(label, sizeof(label), "%d-%d", i * 2, (i + 1) * 2);
        cairo_move_to(cr, x_pos + bar_width / 2, y + height + X_AXIS_LABEL_HEIGHT);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, label, &extents);
        cairo_rel_move_to(cr, -extents.width / 2, extents.height / 2);
        cairo_show_text(cr, label);

        x_pos += bar_width * 2;
    }

    // Vẽ trục X
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_line_to(cr, x + width, y + height);
    cairo_stroke(cr);
}

// Hàm vẽ biểu đồ cột cho số câu trả lời đúng của phòng thi
static void draw_correct_answers_bar_chart(cairo_t *cr, int x, int y, int width, int height) {
    int max_correct = 0;
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (correct_answers_per_question[i] > max_correct) {
            max_correct = correct_answers_per_question[i];
        }
    }

    double bar_width = (double)(width - Y_AXIS_LABEL_WIDTH) / (NUM_QUESTIONS * 2);
    double x_pos = x + Y_AXIS_LABEL_WIDTH + bar_width / 2;

    // Vẽ trục Y
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y);
    cairo_line_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_stroke(cr);

    // Vẽ nhãn trục Y và các đường lưới
    for (int i = 0; i <= max_correct; i++) {
        double y_pos = y + height - (double)i / max_correct * height;
        char label[20];
        snprintf(label, sizeof(label), "%d", i);

        cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH - 10, y_pos);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, label, &extents);
        cairo_rel_move_to(cr, -extents.width - 2, extents.height / 2);
        cairo_show_text(cr, label);

        // Vẽ đường lưới
        if (i > 0) {
            cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
            cairo_set_line_width(cr, 0.5);
            cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y_pos);
            cairo_line_to(cr, x + width, y_pos);
            cairo_stroke(cr);
        }
    }

    cairo_set_line_width(cr, 1);
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        double bar_height = (double)correct_answers_per_question[i] / max_correct * height;
        double y_pos = y + height - bar_height;

        cairo_set_source_rgb(cr, 0.2 + i * 0.05, 0.8 - i * 0.1, 0.4 - i * 0.05);
        cairo_rectangle(cr, x_pos, y_pos, bar_width, bar_height);
        cairo_fill(cr);

        // Vẽ nhãn trục X
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, x_pos + bar_width / 2, y + height + X_AXIS_LABEL_HEIGHT);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, question_labels[i], &extents);
        cairo_rel_move_to(cr, -extents.width / 2, extents.height / 2);
        cairo_show_text(cr, question_labels[i]);

        x_pos += bar_width * 2;
    }

    // Vẽ trục X
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, x + Y_AXIS_LABEL_WIDTH, y + height);
    cairo_line_to(cr, x + width, y + height);
    cairo_stroke(cr);
}

// Hàm callback cho sự kiện vẽ của trang học sinh
static gboolean on_draw_student_page(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Vẽ tiêu đề
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, MARGIN, MARGIN);
    cairo_show_text(cr, "Student Charts");
    int width = gtk_widget_get_allocated_width(widget);

    draw_student_bar_chart(cr, MARGIN, MARGIN * 2 + 20, width / 2 - 2 * MARGIN, CHART_HEIGHT);
    draw_student_line_chart(cr, width / 2 + MARGIN, MARGIN * 2 + 20, width / 2 - 2 * MARGIN, CHART_HEIGHT);

    return FALSE;
}

// Hàm callback cho sự kiện vẽ của trang phòng thi (histogram)
static gboolean on_draw_room_page_histogram(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    // Vẽ tiêu đề
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, MARGIN, MARGIN);
    cairo_show_text(cr, "Exam Room Charts - Histogram");

    draw_room_histogram(cr, MARGIN, MARGIN * 2 + 20, CHART_WIDTH, CHART_HEIGHT);

    return FALSE;
}

// Hàm callback cho sự kiện vẽ của trang phòng thi (số câu trả lời đúng)
static gboolean on_draw_room_page_correct_answers(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    // Vẽ tiêu đề
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, MARGIN, MARGIN);
    cairo_show_text(cr, "Exam Room Charts - Correct Answers per Question");

    draw_correct_answers_bar_chart(cr, MARGIN, MARGIN * 2 + 20, SCROLLABLE_CHART_WIDTH, CHART_HEIGHT);

    return FALSE;
}

// Hàm hiển thị dữ liệu cho học sinh
static void display_student_data(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_DOUBLE);

    GtkTreeIter iter;
    for (int i = 0; i < NUM_EXAMS; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, exam_labels[i], 1, student_scores[i], -1);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Exam", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Score", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

// Hàm hiển thị dữ liệu cho phòng thi
static void display_room_data(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_DOUBLE);

    GtkTreeIter iter;
    for (int i = 0; i < NUM_STUDENTS; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, i + 1, 1, room_scores[i], -1);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Student ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Score", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}
// Hàm hiển thị dữ liệu cho câu trả lời đúng
static void display_correct_answers_data(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);

    GtkTreeIter iter;
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, question_labels[i], 1, correct_answers_per_question[i], -1);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Question", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Correct Answers", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}



GtkWidget *create_result_data_page() {
    GtkWidget *notebook;
    GtkWidget *student_page_box;
    GtkWidget *room_page_box;
    GtkWidget *student_drawing_area;
    GtkWidget *student_treeview;
    GtkWidget *label_student;
    GtkWidget *label_room;
    GtkWidget *student_treeview_scrolled_window;
    GtkWidget *room_notebook;
    GtkWidget *room_page_histogram_drawing_area;
    GtkWidget *room_page_correct_answers_drawing_area;
    GtkWidget *room_page_histogram_treeview;
    GtkWidget *room_page_correct_answers_treeview;
    GtkWidget *room_page_histogram_treeview_scrolled_window;
    GtkWidget *room_page_correct_answers_treeview_scrolled_window;
    GtkWidget *room_page_histogram_box;
    GtkWidget *room_page_correct_answers_box;
    GtkWidget *label_room_histogram;
    GtkWidget *label_room_correct_answers;

    notebook = gtk_notebook_new();

    // Trang cho học sinh
    student_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    student_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(student_drawing_area, CHART_WIDTH + MARGIN * 2, CHART_HEIGHT + MARGIN * 3 + 20);
    g_signal_connect(G_OBJECT(student_drawing_area), "draw", G_CALLBACK(on_draw_student_page), NULL);

    gtk_box_pack_start(GTK_BOX(student_page_box), student_drawing_area, FALSE, FALSE, 0);

    student_treeview = gtk_tree_view_new();
    display_student_data(student_treeview);

    student_treeview_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(student_treeview_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(student_treeview_scrolled_window, CHART_WIDTH + MARGIN * 4, 250);
    gtk_container_add(GTK_CONTAINER(student_treeview_scrolled_window), student_treeview);

    gtk_box_pack_start(GTK_BOX(student_page_box), student_treeview_scrolled_window, FALSE, FALSE, 0);

    label_student = gtk_label_new("Student Page");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), student_page_box, label_student);

    // Trang cho phòng thi - tạo notebook con
    room_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    room_notebook = gtk_notebook_new();

    gtk_box_pack_start(GTK_BOX(room_page_box), room_notebook, TRUE, TRUE, 0);

    // Trang 1 cho phòng thi (histogram)
    room_page_histogram_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    room_page_histogram_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(room_page_histogram_drawing_area, CHART_WIDTH + MARGIN * 2, CHART_HEIGHT + MARGIN * 3 + 20);
    g_signal_connect(G_OBJECT(room_page_histogram_drawing_area), "draw", G_CALLBACK(on_draw_room_page_histogram), NULL);
    room_page_histogram_treeview = gtk_tree_view_new();
    display_room_data(room_page_histogram_treeview);

    room_page_histogram_treeview_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(room_page_histogram_treeview_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(room_page_histogram_treeview_scrolled_window, CHART_WIDTH + MARGIN * 4, 250);
    gtk_container_add(GTK_CONTAINER(room_page_histogram_treeview_scrolled_window), room_page_histogram_treeview);

    gtk_box_pack_start(GTK_BOX(room_page_histogram_box), room_page_histogram_drawing_area, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(room_page_histogram_box), room_page_histogram_treeview_scrolled_window, FALSE, FALSE, 0);

    label_room_histogram = gtk_label_new("Histogram");
    gtk_notebook_append_page(GTK_NOTEBOOK(room_notebook), room_page_histogram_box, label_room_histogram);

    // Trang 2 cho phòng thi (số câu trả lời đúng)
    room_page_correct_answers_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    room_page_correct_answers_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(room_page_correct_answers_drawing_area, SCROLLABLE_CHART_WIDTH + MARGIN * 2, CHART_HEIGHT + MARGIN * 3 + 20);
    g_signal_connect(G_OBJECT(room_page_correct_answers_drawing_area), "draw", G_CALLBACK(on_draw_room_page_correct_answers), NULL);
    room_page_correct_answers_treeview = gtk_tree_view_new();
    display_correct_answers_data(room_page_correct_answers_treeview);

    room_page_correct_answers_treeview_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(room_page_correct_answers_treeview_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(room_page_correct_answers_treeview_scrolled_window, SCROLLABLE_CHART_WIDTH + MARGIN * 4, 250);
    gtk_container_add(GTK_CONTAINER(room_page_correct_answers_treeview_scrolled_window), room_page_correct_answers_treeview);

    gtk_box_pack_start(GTK_BOX(room_page_correct_answers_box), room_page_correct_answers_drawing_area, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(room_page_correct_answers_box), room_page_correct_answers_treeview_scrolled_window, FALSE, FALSE, 0);

    label_room_correct_answers = gtk_label_new("Correct Answers");
    gtk_notebook_append_page(GTK_NOTEBOOK(room_notebook), room_page_correct_answers_box, label_room_correct_answers);

    label_room = gtk_label_new("Room Page");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), room_page_box, label_room);

    return notebook;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *result_data_page;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Exam Statistics");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    result_data_page = create_result_data_page();
    gtk_container_add(GTK_CONTAINER(window), result_data_page);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}



