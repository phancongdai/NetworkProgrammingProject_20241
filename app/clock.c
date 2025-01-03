// #include <gtk/gtk.h>
// #include <time.h>

// // Thời gian đếm ngược ban đầu tính bằng giây
// static int initial_time = 2; // 10 phút

// // Thời gian còn lại tính bằng giây
// static int remaining_time = 0;

// // Con trỏ tới nhãn hiển thị thời gian đếm ngược
// static GtkWidget *time_label;

// // Hàm cập nhật hiển thị thời gian đếm ngược
// gboolean update_timer(gpointer data) {
//     if (remaining_time <= 0) {
//         // Đếm ngược kết thúc, thực hiện hành động mong muốn ở đây
//         gtk_label_set_text(GTK_LABEL(time_label), "Hết giờ!");
//         return G_SOURCE_REMOVE; // Dừng bộ hẹn giờ
//     }

//     remaining_time--;

//     // Định dạng thời gian còn lại
//     int hours = remaining_time / 3600;
//     int minutes = (remaining_time % 3600) / 60;
//     int seconds = remaining_time % 60;

//     gchar *time_str = g_strdup_printf("%02d:%02d:%02d", hours, minutes, seconds);
//     gtk_label_set_text(GTK_LABEL(time_label), time_str);
//     g_free(time_str);

//     return G_SOURCE_CONTINUE; // Tiếp tục bộ hẹn giờ
// }

// // Hàm được gọi khi nhấp vào nút "Bắt đầu"
// void start_button_clicked(GtkWidget *widget, gpointer data) {
//     remaining_time = initial_time;
//     g_timeout_add_seconds(1, update_timer, NULL); // Bắt đầu bộ hẹn giờ
// }

// int main(int argc, char *argv[]) {
//     gtk_init(&argc, &argv);

//     // Tạo cửa sổ chính
//     GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//     gtk_window_set_title(GTK_WINDOW(window), "Đồng hồ đếm ngược");
//     gtk_container_set_border_width(GTK_CONTAINER(window), 10);
//     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//     // Tạo một hộp dọc để bố trí các widget
//     GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//     gtk_container_add(GTK_CONTAINER(window), vbox);

//     // Tạo nhãn để hiển thị thời gian đếm ngược
//     time_label = gtk_label_new("");
//     gtk_label_set_markup(GTK_LABEL(time_label), "<span font_desc='48'>00:10:00</span>");
//     gtk_box_pack_start(GTK_BOX(vbox), time_label, TRUE, TRUE, 0);

//     // Tạo nút "Bắt đầu"
//     GtkWidget *start_button = gtk_button_new_with_label("Bắt đầu");
//     g_signal_connect(start_button, "clicked", G_CALLBACK(start_button_clicked), NULL);
//     gtk_box_pack_start(GTK_BOX(vbox), start_button, FALSE, FALSE, 0);

//     // Hiển thị tất cả các widget
//     gtk_widget_show_all(window);

//     gtk_main();

//     return 0;
// }



#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Cấu trúc cho mỗi câu hỏi
typedef struct {
    gchar *id;
    gchar *content;
    gchar *options[4];
    gint selected_option; // -1 nghĩa là chưa chọn
} Question;

// Mảng chứa tất cả các câu hỏi
Question questions[] = {
    {"14bb9049-9498-4c54-a58d-00d280a2689b",
     "Venous congestion of liver affects ?",
     {"Midzone", "Perihepatic zone", "Centrilobular zone", "All the above"},
     -1},
    {"2ef82f67-4906-4d5c-b65f-2edb36068c75",
     "Which of the following opioid can be given intranasally?",
     {"Tramadol", "Pethidine", "Butorphanol", "Buprenorphine"},
     -1},
    {"2bd0e778-c09a-4b3e-8dce-c59e8937953b",
     "A patient presented with fever, cervical lymphadenopathy and night "
     "sweats four weeks after unprotected sexual contact. Next investigation",
     {"p24", "ELISA", "CD4", "HIV PCR"},
     -1},
    {"cdf04af4-0d45-4d27-8595-36b6c320df52",
     "All are immediate causes of death in burns except",
     {"Injury", "Suffocation", "Toxemia", "Shock"},
     -1},
    {"cdc0584b-8fd6-451b-85f7-3cb56770b070",
     "A child presenting with localized patches of complete hair loss with "
     "normal appearance of scalp. Diagnosis is -",
     {"Tinea capitis", "Cradle cap", "Alopecia areata", "Telogen effluvium"},
     -1},
     {"0c030bfa-2ea3-4681-92dd-3ad00bba609b",
      "Drug of choice for acute severe asthma is: September 2006",
      {"Sho acting beta 2 agonists", "Long acting beta 2 agonists", "Oral theophylline", "Inhaled ipratropium bromide"},
      -1
     },
     {"6022af1f-be7a-4b78-8cda-43e0bac536e5",
      "A 28 year old labourer, 3 yrs back presented with penile ulcer which was not treated. Later he presented with neurological symptoms for which he got treated. Which is the test to monitor response to treatment?",
      {"VDRL", "FTA ABS", "TPI", "RPR"},
      -1
     },
     {"c337bf74-49d1-4b5d-9838-04319f80bd54",
     "A lady used to repeatedly wash her hands due to the fear of acquiring AIDS is due to",
     {"OCD", "Mania", "Depression", "Anxiety"},
     -1
     },
     {"440222b4-4c23-4ae3-b28d-a513db91dbfc",
      "Sixth cranial nerve palsy causes of left eye causes?",
      {"Accomodation paresis in left gaze", "Ptosis of left eye", "Adduction weakness of left eye", "Diplopia in left gaze"},
      -1
     },
     {"536aa21e-dde9-4e75-b75a-648a9b27589b",
      "Chlorhexidine as a root canal irrigant is",
      {"Active against gram +ve , gram –ve bacterias and to some extent Virus with lipid envelop.", "Normally used at 2% conc", "A poly biguinide", "All of the above"},
      -1
     }
};
int num_questions = sizeof(questions) / sizeof(questions[0]);

// Số cột trong Quiz Navigation
#define NUM_COLUMNS 5

// Biến toàn cục để theo dõi câu hỏi hiện tại
int current_question = 0;

// Chuỗi lưu các đáp án đã chọn
gchar *submit_ans;

// Widget toàn cục để cập nhật nội dung
GtkWidget *question_label;
GtkWidget *option_buttons[4];
GtkWidget *question_list_buttons[10];
GtkWidget *next_page_button;
GtkWidget *submit_button;
GtkWidget *main_grid;
GtkWidget *quiz_nav_label;
GtkWidget *question_title_label;

// Biến cho đồng hồ bấm giờ
static int remaining_time = 0; // Thời gian còn lại tính bằng giây
static GtkWidget *timer_label;  // Nhãn hiển thị thời gian




//Hàm cập nhật giao diện
void update_question_display(int question_index) {
    // Đặt nội dung câu hỏi
    gchar question_title[32];
    sprintf(question_title, "Question %d", question_index + 1);
    gtk_label_set_text(GTK_LABEL(question_title_label), question_title);
    gtk_label_set_text(GTK_LABEL(question_label), questions[question_index].content);

    // Đặt nội dung các nút câu trả lời
    for (int i = 0; i < 4; i++) {
        gtk_button_set_label(GTK_BUTTON(option_buttons[i]),
                            questions[question_index].options[i]);
        // Nếu câu trả lời đã được chọn, làm mờ nút tương ứng
        if (submit_ans[question_index] == 'A' + i) {
            gtk_widget_set_opacity(option_buttons[i], 0.5);
            gtk_widget_set_sensitive(option_buttons[i], FALSE);
        } else {
            gtk_widget_set_opacity(option_buttons[i], 1.0);
            gtk_widget_set_sensitive(option_buttons[i], TRUE);
        }
    }

    // Đặt lại kiểu cho tất cả các nút câu hỏi
    for (int i = 0; i < num_questions; i++) {
        GtkStyleContext *context = gtk_widget_get_style_context(question_list_buttons[i]);
        gtk_style_context_remove_class(context, "selected-question");
        gtk_widget_set_sensitive(question_list_buttons[i], TRUE);
    }

    // Đặt kiểu cho nút câu hỏi hiện tại
    GtkStyleContext *current_context = gtk_widget_get_style_context(question_list_buttons[question_index]);
    gtk_style_context_add_class(current_context, "selected-question");

    // Bôi đậm nút câu hỏi nếu đã được trả lời
    for (int i = 0; i < num_questions; i++) {
        if (questions[i].selected_option != -1) {
            GtkStyleContext *context = gtk_widget_get_style_context(question_list_buttons[i]);
            gtk_style_context_add_class(context, "answered-question");
        } else {
            GtkStyleContext *context = gtk_widget_get_style_context(question_list_buttons[i]);
            gtk_style_context_remove_class(context, "answered-question");
        }
    }

    // Làm mờ các nút câu hỏi không được chọn
    for (int i = 0; i < num_questions; i++) {
        if (i != question_index) {
            gtk_widget_set_opacity(question_list_buttons[i], 0.5);
        } else {
            gtk_widget_set_opacity(question_list_buttons[i], 1.0);
        }
    }
}

// Hàm callback cho các nút câu trả lời
static void answer_button_clicked(GtkWidget *widget, gpointer data) {
    int option_index = GPOINTER_TO_INT(data);

    // Lưu câu trả lời vào struct
    questions[current_question].selected_option = option_index;

    // Cập nhật chuỗi submit_ans
    submit_ans[current_question] = 'A' + option_index;

    // Làm mờ và vô hiệu hóa nút của đáp án đã chọn
    gtk_widget_set_opacity(widget, 0.5);
    gtk_widget_set_sensitive(widget, FALSE);

    update_question_display(current_question);
    // In ra để kiểm tra
    g_print("Question %d, Answer %c selected\n", current_question + 1,
            'A' + option_index);
    g_print("submit_ans: %s\n", submit_ans);
}

// Hàm callback cho các nút danh sách câu hỏi
static void question_list_button_clicked(GtkWidget *widget, gpointer data) {
  int question_index = GPOINTER_TO_INT(data);
  current_question = question_index;
  update_question_display(current_question);
}

// Hàm callback cho nút "Next Page"
static void next_page_button_clicked(GtkWidget *widget, gpointer data) {
    if (current_question < num_questions - 1) {
        current_question++;
        update_question_display(current_question);
    } else {
        // Nếu đã là câu cuối cùng, quay lại câu đầu tiên
        current_question = 0;
        update_question_display(current_question);
    }
}

// Hàm callback cho nút "Submit Answer"
static void submit_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Submitted answers: %s\n", submit_ans);
    // Dừng bộ hẹn giờ nếu nộp bài
    remaining_time = 0;
}


// Hàm cập nhật hiển thị đồng hồ bấm giờ
gboolean update_timer(gpointer data) {
    if (remaining_time <= 0) {
        // Hết giờ, thực hiện hành động mong muốn ở đây (ví dụ: nộp bài)
        gtk_label_set_text(GTK_LABEL(timer_label), "Hết giờ!");
        submit_button_clicked(submit_button, NULL); // Gửi câu trả lời
        return G_SOURCE_REMOVE; // Dừng bộ hẹn giờ
    }

    remaining_time--;

    // Định dạng thời gian còn lại
    int minutes = remaining_time / 60;
    int seconds = remaining_time % 60;

    gchar *time_str = g_strdup_printf("%02d:%02d", minutes, seconds);
    gtk_label_set_text(GTK_LABEL(timer_label), time_str);
    g_free(time_str);

    return G_SOURCE_CONTINUE; // Tiếp tục bộ hẹn giờ
}

// Hàm bắt đầu đồng hồ bấm giờ (có thể gọi khi bắt đầu bài kiểm tra)
void start_timer(int duration_minutes) {
    remaining_time = duration_minutes * 60;
    // Bắt đầu bộ hẹn giờ cập nhật mỗi giây
    g_timeout_add_seconds(1, update_timer, NULL);
}


int main(int argc, char *argv[]) {
    GtkWidget *window;

    // Khởi tạo GTK
    gtk_init(&argc, &argv);

    // Khởi tạo chuỗi submit_ans (ban đầu tất cả là ' ')
    submit_ans = g_malloc0(num_questions + 1);
    memset(submit_ans, ' ', num_questions);
    submit_ans[num_questions] = '\0';

    // Tạo cửa sổ
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Quiz");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Tạo grid chính
    main_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(main_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(main_grid), 10);
    gtk_container_add(GTK_CONTAINER(window), main_grid);

    // Tạo khung viền cho Quiz Navigation
    GtkWidget *quiz_nav_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(quiz_nav_frame), GTK_SHADOW_ETCHED_IN);
    gtk_grid_attach(GTK_GRID(main_grid), quiz_nav_frame, 0, 0, 1, 2);

    // Tạo grid cho Quiz Navigation
    GtkWidget *quiz_nav_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(quiz_nav_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(quiz_nav_grid), 5);
    gtk_container_add(GTK_CONTAINER(quiz_nav_frame), quiz_nav_grid);

    // Tạo label "Quiz Navigation"
    quiz_nav_label = gtk_label_new("Quiz Navigation");
    gtk_grid_attach(GTK_GRID(quiz_nav_grid), quiz_nav_label, 0, 0, NUM_COLUMNS, 1);

    // Tạo các nút cho danh sách câu hỏi
    for (int i = 0; i < num_questions; i++) {
        gchar button_text[4];
        sprintf(button_text, "%d", i + 1);
        question_list_buttons[i] = gtk_button_new_with_label(button_text);
        g_signal_connect(question_list_buttons[i], "clicked",
                        G_CALLBACK(question_list_button_clicked),
                        GINT_TO_POINTER(i));

        // Thêm nút vào quiz_nav_grid
        gtk_grid_attach(GTK_GRID(quiz_nav_grid), question_list_buttons[i], i % NUM_COLUMNS, i / NUM_COLUMNS + 1, 1, 1);
    }

    // Tạo khung viền cho khu vực câu hỏi
    GtkWidget *question_area_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(question_area_frame), GTK_SHADOW_ETCHED_IN);
    gtk_grid_attach(GTK_GRID(main_grid), question_area_frame, 1, 0, 4, 2);

    // Tạo grid cho khu vực câu hỏi
    GtkWidget *question_area_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(question_area_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(question_area_grid), 10);
    gtk_container_add(GTK_CONTAINER(question_area_frame), question_area_grid);

    // Tạo label tiêu đề câu hỏi
    question_title_label = gtk_label_new("Question 1");
    gtk_grid_attach(GTK_GRID(question_area_grid), question_title_label, 0, 0, 4, 1);

    // Tạo label cho nội dung câu hỏi
    question_label = gtk_label_new("");
    gtk_label_set_line_wrap(GTK_LABEL(question_label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(question_label), 0); // Căn lề trái
    gtk_grid_attach(GTK_GRID(question_area_grid), question_label, 0, 1, 4, 1);

    // Tạo các nút cho câu trả lời
    for (int i = 0; i < 4; i++) {
        option_buttons[i] = gtk_button_new_with_label("");
        g_signal_connect(option_buttons[i], "clicked",
                        G_CALLBACK(answer_button_clicked), GINT_TO_POINTER(i));
        gtk_grid_attach(GTK_GRID(question_area_grid), option_buttons[i], 0, i + 2, 1, 1);
    }

    // Tạo nút "Next Page"
    next_page_button = gtk_button_new_with_label("Next Page");
    g_signal_connect(next_page_button, "clicked",
                    G_CALLBACK(next_page_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(question_area_grid), next_page_button, 0, 6, 1, 1);

    // Tạo nút "Submit Answer"
    submit_button = gtk_button_new_with_label("Submit Answer");
    g_signal_connect(submit_button, "clicked",
                     G_CALLBACK(submit_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(question_area_grid), submit_button, 1, 6, 1, 1);

    // Tạo nhãn cho đồng hồ bấm giờ
    timer_label = gtk_label_new("00:00");
    // Có thể đặt kiểu cho timer_label (ví dụ: font lớn hơn)
    gtk_grid_attach(GTK_GRID(question_area_grid), timer_label, 3, 6, 1, 1);

  // CSS để bôi đậm các nút
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_data(provider,
    ".answered-question { font-weight: bold; }"
    ".selected-question { background-color: #E0FFFF; font-weight: bold; }", -1, NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                           GTK_STYLE_PROVIDER(provider),
                                           GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Hiển thị câu hỏi đầu tiên
    update_question_display(current_question);

    // Bắt đầu đồng hồ bấm giờ với thời lượng mong muốn (ví dụ: 5 phút)
    start_timer(5);

    // Hiển thị tất cả các widget
    gtk_widget_show_all(window);

    // Bắt đầu vòng lặp sự kiện GTK
    gtk_main();

    // Giải phóng bộ nhớ của submit_ans
    g_free(submit_ans);

    return 0;
}