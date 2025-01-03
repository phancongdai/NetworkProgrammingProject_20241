/*
1. Quan ly cau hoi
    1.1. Tim kiem cau hoi
    1.2. Them cau hoi
    1.3. Sua cau hoi
    1.4. Xoa cau hoi
2. Quan ly bai ktra
    2.1. Tim kiem bai ktra
    2.2. Them bai ktra
    2.3. Sua bai ktra
    2.4. Xoa bai ktra
3. Quan ly phong thi
    3.1. Tao phong thi(Ma Phong Thi(Tao tu dong), Ten, Mon(subject), Topic, So luong thi sinh toi da, Thoi gian bat dau, Thoi gian ket thuc)
    3.2. Thêm user vào phòng thi (user cũng có thể là admin)
    3.3. Quan ly user trong phòng thi (Xem danh sách user, add user, xóa user)
    
4. Xem KQ dua tren phong thi hoac bai ktra
5. Xem thong tin User dua tren User_id
6. Cap quyen cho User thanh Admin
*/

#include "../data.h"
#include "client.h"
#include "utils.h"
#include <mysql/mysql.h>
#include <time.h>

#define MAX_QUERY_LEN 3000

void UIMainAppMenuAdmin(int client_sockfd);
void manageQuestion(int sockfd);
void manageExam(int sockfd);
void manageExamRoom(int sockfd);
void getExamRoomResult(int sockfd);
void getUserInfo(int sockfd);
void approveAdminRequest(int sockfd);
void searchQuestion(int sockfd);
void searchQuestionById(int sockfd);
void searchQuestionByContent(int sockfd);
void advanceQuestionSearch(int sockfd);
void addQuestion(int sockfd);
void deleteQuestion(int sockfd);
void showQuestionManagementInterface();
void showExamManagementInterface();
void searchExam(int sockfd);
void createRandomExam(int sockfd);
void createExamByQuestion(int sockfd);
void createExam(int sockfd);
void createRoom(int client_sockfd);
void deleteRoom(int client_sockfd);
void showYourRoom(int client);

void createExamFromRoom(int sockfd);
void createRandomExamFromRoom(int sockfd);
void createExamByQuestionFromRoom(int sockfd);

void adminTakeExam(int client_sockfd, exam_data* exam);

// Trong admin.c

void adminPrintExamInfo(exam_data* exam){
    printf("Exam info:\n");
    printf("Exam id: %d\n", exam->exam_id);
    printf("Exam title: %s\n", exam->title);
    printf("Number of question: %d\n", exam->number_of_question);
}

void adminTakeExam(int client_sockfd, exam_data* exam){
    adminPrintExamInfo(exam);
    printf("Do you want to take this exam?(y/n): ");
    char option;
    scanf(" %1c", &option);
    __fpurge(stdin);
    if(option == 'n'){
        // showExamList(client_sockfd);
        return;
    }
    else if(option == 'y'){
        printf("\n\n##########################################\n\n");
        printf("Exam: %s\n\n", (exam->title));

        request_question_list request_question_list;
        request_question_list.opcode = 204;
        request_question_list.user_id = data.user_id;
        request_question_list.exam_id = exam->exam_id;
        request_question_list.number_of_question = exam->number_of_question;
        send(client_sockfd, &request_question_list, sizeof(request_question_list), 0);

        //Receive question list
        question_data **question_list;
        question_list = malloc(sizeof(question_data*));
        *question_list = malloc(sizeof(question_data)*(exam->number_of_question));

        // Array to store user's answers
        char user_answers[exam->number_of_question];
        memset(user_answers, 0, sizeof(user_answers)); // Initialize all answers to 0 (unanswered)

        for(int i=0; i<exam->number_of_question; i++){
            recv(client_sockfd, (*(question_list)+i), sizeof(question_data), MSG_WAITALL);
            send(client_sockfd, "OK", sizeof("OK"), 0);
        }

        int current_question = 0;
        while(current_question < exam->number_of_question){

            printf("Question %d: %s\n", current_question+1, (*(question_list)+current_question)->content);
            for(int j=0; j<4; j++){
                printf("%c. %s\n", j+65, (*(question_list)+current_question)->op[j]);
            }

            // Display the current answer (if any)
            if (user_answers[current_question] != 0) {
                printf("Your current answer: %c\n", user_answers[current_question]);
            }

            printf("Enter your answer (A, B, C, D, P: Previous, N: Next, S: Submit): ");
            char ans;
            scanf(" %c", &ans);
            __fpurge(stdin);
            ans = toupper(ans); // Convert answer to uppercase

            if (ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D') {
                user_answers[current_question] = ans;
                // Check if it's the last question before incrementing
                if (current_question < exam->number_of_question - 1) {
                    current_question++;
                } else {
                    // If it's the last question, jump to the submit section
                    goto submit_section;
                }
            } else if (ans == 'P') {
                // Go to the previous question, if any
                if (current_question > 0) {
                    current_question--;
                }
            } else if (ans == 'N') {
                // Go to the next question, if any
                if (current_question < exam->number_of_question - 1) {
                    current_question++;
                } else {
                    // If it's the last question, jump to the submit section
                    goto submit_section;
                }
            } else if (ans == 'S') {
            submit_section: // Label to jump to
                // Display answers and allow changes
                while (1) {
                    printf("\nYour final answers:\n");
                    for (int i = 0; i < exam->number_of_question; i++) {
                        printf("Question %d: %c\n", i + 1, (user_answers[i] == 0) ? ' ' : user_answers[i]);
                    }
                    printf("\nDo you want to change any answer? (Enter question number to change, or 'EXIT' to submit): ");
                    char change_choice[10];
                    scanf(" %s", change_choice);
                    __fpurge(stdin);

                    if (strcmp(change_choice, "EXIT") == 0) {
                        break; // Exit the loop to submit
                    } else {
                        int question_to_change = atoi(change_choice);
                        if (question_to_change >= 1 && question_to_change <= exam->number_of_question) {
                            // Display the question to be changed
                            printf("Question %d: %s\n", question_to_change, (*(question_list) + question_to_change - 1)->content);
                            for (int j = 0; j < 4; j++) {
                                printf("%c. %s\n", j + 65, (*(question_list) + question_to_change - 1)->op[j]);
                            }
                            printf("Enter new answer for question %d (A, B, C, D): ", question_to_change);
                            char new_ans;
                            scanf(" %c", &new_ans);
                            __fpurge(stdin);
                            new_ans = toupper(new_ans);
                            if (new_ans == 'A' || new_ans == 'B' || new_ans == 'C' || new_ans == 'D'){
                                user_answers[question_to_change - 1] = new_ans;
                            } else {
                                printf("Invalid answer.\n");
                            }
                        } else {
                            printf("Invalid question number.\n");
                        }
                    }
                }

                printf("Are you sure you want to submit? (y/n): ");
                char submit_choice;
                scanf(" %c", &submit_choice);
                __fpurge(stdin);
                if (submit_choice == 'y') {
                    break; // Exit the outer while loop to submit
                }
            } else {
                printf("Invalid option!\n");
            }
        }

        // Prepare the final answer string
        char final_answer[exam->number_of_question + 1];
        for(int i = 0; i < exam->number_of_question; i++){
            final_answer[i] = (user_answers[i] == 0) ? ' ' : user_answers[i]; // If unanswered, leave a space
        }
        final_answer[exam->number_of_question] = '\0';

        // Send the answer to server
        submit_ans ans;
        ans.opcode = 205;
        ans.exam_id = exam->exam_id;
        ans.user_id = data.user_id;
        strcpy(ans.username, data.username);
        strcpy(ans.submit_ans, final_answer);
        ans.number_of_question = exam->number_of_question;
        send(client_sockfd, &ans, sizeof(submit_ans), 0); 

        // Receive the result
        int result;
        recv(client_sockfd, &result, sizeof(result), 0);
        printf("\nYour score: %d/%d\n", result, exam->number_of_question);
        // showExamList(client_sockfd);
    }
    else{
        printf("Invalid option!\n");
        adminTakeExam(client_sockfd, exam);
        return;
    }
}


void showAllSubjects(int sockfd){
    subject_request request;
    request.opcode = 607;
    send(sockfd, &request, sizeof(request), 0);
    int num_of_subject;
    recv(sockfd, &num_of_subject, sizeof(int), 0);
    if(num_of_subject == 0){
        printf("No subject found!\n");
        return;
    }
    char subject[150];
    for(int i = 0; i < num_of_subject; i++){
        recv(sockfd, subject, sizeof(subject), 0);
        printf("%d. %s\n", i+1, subject);
    }
}

void showCountOfChosenSubject(int sockfd, char *subject){
    create_random_exam request;
    request.opcode = 608;
    strcpy(request.subject, subject);
    send(sockfd, &request, sizeof(request), 0);
    level_count level;
    recv(sockfd, &level, sizeof(level), 0);
    printf("Easy: %d\n", level.easy_count);
    printf("Normal: %d\n", level.normal_count);
    printf("Hard: %d\n", level.hard_count);
}


void UIMainAppMenuAdmin(int client_sockfd){

    printf("Main application system!\n");
    printf("1. Manage question database(Search, Add, Edit, Delete)\n");
    printf("2. Manage exam database(Search, Add, Edit, Delete)\n");
    printf("3. Manage exam room\n");
    printf("4. See exam room result\n");
    printf("5. See user information\n");
    printf("6. Approve admin request\n");
    printf("7. Log out\n");
    printf("Please choose your option: ");
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
        switch(option){
        case 1:
            manageQuestion(client_sockfd);
            return;
        case 2:
            manageExam(client_sockfd);
            return;
        case 3:
            manageExamRoom(client_sockfd);
            return;
        case 4:
            getExamRoomResult(client_sockfd);
            return;
        case 5:
            getUserInfo(client_sockfd);
            return;
        case 6:
            approveAdminRequest(client_sockfd);
            return;
        case 7:
            printf("Log out successfully!\n");
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
            UIHomePage(client_sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
        }
    UIMainAppMenuAdmin(client_sockfd);
}

void showQuestionManagementInterface(){
    printf("Question management interface!\n");
    printf("1. Search question\n");
    printf("2. Add question\n");
    printf("3. Delete question\n");
    printf("4. Back\n");
    printf("Please choose your option: ");
}

//!TODO: Manage question
void searchQuestion(int sockfd);
// void addQuestion(int sockfd);
// void editQuestion(int sockfd);
// void deleteQuestion(int sockfd);
void manageQuestion(int sockfd){
    showQuestionManagementInterface();
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
    switch(option){
        case 1:
            searchQuestion(sockfd);
            return;
        case 2:
            addQuestion(sockfd);
            return;
        case 3:
            deleteQuestion(sockfd);
            return;
        case 4:
            UIMainAppMenuAdmin(sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
    }
    manageQuestion(sockfd);
}

//############## SEARCH QUESTION #################

void searchQuestionById(int sockfd){
    printf("Search question by id!\n");
    printf("Please enter question id: ");
    // Get question id
    char question_id[255];
    scanf(" %[^\n]255s", question_id);
    __fpurge(stdin);
    search_question_by_id question;
    question.opcode = 601;
    strcpy(question.question_id, question_id);
    question.next_page = 0;
    // Send question id to server
    send(sockfd, &question, sizeof(question), 0);
    // Receive number of found question
    int numOfFoundQuestion;
    recv(sockfd, &numOfFoundQuestion, sizeof(question_data), 0);
    // If no question found
    if(numOfFoundQuestion == 0){
        printf("No question found!\n");
        searchQuestion(sockfd);
        return;
    }else{ // If found question
        int numCurrentQuestion = 0;
        while(numCurrentQuestion < numOfFoundQuestion){
            printf("Found %d question(s)!\n", numOfFoundQuestion);
            question_data **question_list;
            question_list = malloc(sizeof(question_data*));
            *question_list = malloc(sizeof(question_data)*10);
            send(sockfd, &numCurrentQuestion, 4, 0); // Send the begin index to search question
            // Receive 10 question from server
            int num_question_recv = 0;
            recv(sockfd, &num_question_recv, 4, 0);  
            printf("num_question_recv: %d\n", num_question_recv);
            for(int i = 0; i < num_question_recv; i++){
                send(sockfd, "OK", sizeof("OK"), 0);
                recv(sockfd, (*(question_list)+i), sizeof(question_data), 0);
                printf("Question %d: %s\n", i+1, (*(question_list)+i)->content);
                // printf("A: %s\nB: %s\nC: %s\nD: %s\n", (*(question_list)+i)->opa,(*(question_list)+i)->opb, (*(question_list)+i)->opc, (*(question_list)+i)->opd);
            }
            numCurrentQuestion += 10;
            printf("\n1. Next page\n");
            printf("2. Back\n");
            printf("Please choose your option: ");
            int option;
            scanf(" %1d", &option);
            __fpurge(stdin);
            switch(option){
                case 1:
                    question.next_page = 1;
                    send(sockfd, &question, sizeof(question), 0);
                    break;
                case 2:
                    searchQuestion(sockfd);
                    return;
                default:
                    searchQuestion(sockfd);
                    return;
            }
        }
    }
}

void searchQuestionByContent(int sockfd){
    printf("Search question by content!\n");
    printf("Please enter question content: ");
    // Get question content
    char question_content[3000];
    scanf(" %[^\n]3000s", question_content);
    __fpurge(stdin);
    search_question_by_content question;
    question.opcode = 602;
    question.next_page = 0;
    strcpy(question.question_content, question_content);
    // Send question content to server
    send(sockfd, &question, sizeof(question), 0);
    // Receive number of found question
    int numOfFoundQuestion;
    recv(sockfd, &numOfFoundQuestion, sizeof(question_data), 0);
    // If no question found
    if(numOfFoundQuestion == 0){
        printf("No question found!\n");
        searchQuestion(sockfd);
        return;
    }else{ // If found question
        int numCurrentQuestion = 0;
        while(numCurrentQuestion < numOfFoundQuestion){
            printf("Found %d question(s)!\n", numOfFoundQuestion);
            question_data **question_list;
            question_list = malloc(sizeof(question_data*));
            *question_list = malloc(sizeof(question_data)*10);
            send(sockfd, &numCurrentQuestion, 4, 0); // Send the begin index to search question
            // Receive 10 question from server
            int num_question_recv = 0;
            recv(sockfd, &num_question_recv, 4, 0);
            for(int i = 0; i < num_question_recv; i++){
                send(sockfd, "OK", sizeof("OK"), 0);
                recv(sockfd, (*(question_list)+i), sizeof(question_data), 0);
                printf("Question %d: %s\n", i+1, (*(question_list)+i)->content);
                // printf("A: %s\nB: %s\nC: %s\nD: %s\n", (*(question_list)+i)->opa,(*(question_list)+i)->opb, (*(question_list)+i)->opc, (*(question_list)+i)->opd);

            }
            numCurrentQuestion += 10;
            printf("\n1. Next page\n");
            printf("2. Back\n");
            printf("Please choose your option: ");
            int option;
            scanf(" %1d", &option);
            __fpurge(stdin);
            switch(option){
                case 1:
                    question.next_page = 1;
                    send(sockfd, &question, sizeof(question), 0);
                    break;
                case 2:
                    searchQuestion(sockfd);
                    return;
                default:
                    searchQuestion(sockfd);
                    return;
            }
        }
    }
    //searchQuestion(sockfd);
}

void advanceQuestionSearch(int sockfd){
    printf("Advance question search!\n");
    char question_id[255];
    char question_content[3000];
    printf("Please enter question id: ");
    scanf(" %[^\n]255s", question_id);
    __fpurge(stdin);
    printf("Please enter question content: ");
    scanf(" %[^\n]3000s", question_content);
    __fpurge(stdin);
    char topic[255];
    printf("Please choose a subject: \n");
    for(int i = 0; i<19; i+=2){
        printf("%d. %s\t", i+1, subject_list[i]);
        printf("%d. %s\n", i+2, subject_list[i+1]);
    }
    printf("%d. %s\n", 21, subject_list[20]);
    int subject;
    scanf(" %d", &subject);
    __fpurge(stdin);
    if(subject < 1 || subject > 21){
        printf("Invalid option!\n");
        subject = 0;
    }
    printf("Please enter a topic: ");
    scanf(" %[^\n]255s", topic);
    __fpurge(stdin);

    char level[11];
    printf("Please enter question level (VD: easy, medium, hard): ");
    scanf(" %[^\n]10s", level);
    __fpurge(stdin);

    advance_search_question question;
    question.opcode = 603;
    strcpy(question.question_id, question_id);
    strcpy(question.question_content, question_content);
    strcpy(question.subject, subject_list[subject-1]);
    strcpy(question.topic, topic);
    strcpy(question.level, level);
    // Send question content to server
    send(sockfd, &question, sizeof(question), 0);
    // Receive number of found question
    int numOfFoundQuestion;
    recv(sockfd, &numOfFoundQuestion, sizeof(question_data), 0);
    // If no question found
    if(numOfFoundQuestion == 0){
        printf("No question found!\n");
        searchQuestion(sockfd);
        return;
    }else{ // If found question
        int numCurrentQuestion = 0;
        while(numCurrentQuestion < numOfFoundQuestion){
            printf("Found %d question(s)!\n", numOfFoundQuestion);
            question_data **question_list;
            question_list = malloc(sizeof(question_data*));
            *question_list = malloc(sizeof(question_data)*10);
            send(sockfd, &numCurrentQuestion, 4, 0); // Send the begin index to search question
            // Receive 10 question from server
            int num_question_recv = 0;
            recv(sockfd, &num_question_recv, 4, 0);
            for(int i = 0; i < num_question_recv; i++){
                recv(sockfd, (*(question_list)+i), sizeof(question_data), 0);
                send(sockfd, "OK", sizeof("OK"), 0);
                printf("Question %d: %s\n", i+1, (*(question_list)+i)->content);
            }
            numCurrentQuestion += 10;
            printf("\n1. Next page\n");
            printf("2. Back\n");
            printf("Please choose your option: ");
            int option;
            scanf(" %1d", &option);
            __fpurge(stdin);
            switch(option){
                case 1:
                    question.next_page = 1;
                    send(sockfd, &question, sizeof(question), 0);
                    break;
                case 2:
                    searchQuestion(sockfd);
                    return;
                default:
                    searchQuestion(sockfd);
                    return;
            }
        }
    }
}


void searchQuestion(int sockfd){
    printf("Search question!\n");
    printf("1. Search by question id\n");
    printf("2. Search by question content\n");
    printf("3. Advance question search\n");
    printf("4. Back\n");
    printf("Please choose your option: ");
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
    switch(option){
        case 1:
            searchQuestionById(sockfd);
            return;
        case 2:
            searchQuestionByContent(sockfd);
            return;
        case 3:
            advanceQuestionSearch(sockfd);
            return;
        case 4:
            manageQuestion(sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
    }
    searchQuestion(sockfd);
}


//############## END SEARCH FUNCTION #################

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@ “If opportunity doesn’t knock, build a door.” ―Kurt Cobain

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

//############## ADD/DELETE QUESTION #################
void addQuestion(int sockfd){
    printf("Add new question to question database!\n");
    add_question question;
    char question_id[51], question_content[3000], subject[30], topic[150];
    char op[4][300];
    char level[11];
    int ans;

    //Get question information
    printf("Please enter question id: ");
    scanf(" %51s", question_id);
    printf("Please enter question content: ");
    scanf(" %[^\n]1000", question_content);
    printf("Please enter subject: \n");
    scanf(" %[^\n]30s", subject);
    printf("Please enter topic: ");
    scanf(" %[^\n]150s", topic);


    printf("Please enter 4 options: \n");
    for(int i = 0; i < 4; i++){
        printf("Option %d: ", i+1);
        scanf(" %[^\n]300s", op[i]);
    }
    printf("Please enter the correct answer: ");
    scanf(" %d", &ans);
    printf("Please enter question level (Easy / Normal / Hard): ");
    scanf(" %9s", level);

    //Send question information to server
    question.opcode = 604;
    strcpy(question.question_id, question_id);
    strcpy(question.question_content, question_content);
    strcpy(question.subject, subject);
    strcpy(question.topic, topic);
    for(int i = 0; i < 4; i++){
        strcpy(question.op[i], op[i]);
    }
    question.ans = ans;
    strcpy(question.level, level);
    printf("Are you sure to add this question? (y/n)\n");
    char opt;
    scanf(" %c", &opt);
    if(opt == 'n'){
        manageQuestion(sockfd);
        return;
    }
    send(sockfd, &question, sizeof(question), 0);
    int reply;
    recv(sockfd, &reply, sizeof(int), 0);
    if(reply == 1){
        printf("Add question successfully!\n");
    }
    else{
        printf("Cannot add question!\n");
    }
    manageQuestion(sockfd);
}

void deleteQuestion(int sockfd){
    printf("Delete question from question database!\n");
    char question_id[51];
    printf("Please enter question id: ");
    scanf(" %51s", question_id);
    question_data question;
    question.opcode = 605;
    strcpy(question.question_id, question_id);
    printf("Are you sure to delete question with id %s? (y/n)\n", question_id);
    char opt;
    scanf(" %c", &opt);
    if(opt == 'n'){
        manageQuestion(sockfd);
        return;
    }
    send(sockfd, &question, sizeof(question), 0);
    int reply;
    recv(sockfd, &reply, sizeof(int), 0);
    if(reply == 1){
        printf("Delete question successfully!\n");
    }
    else{
        printf("Cannot delete question!\n");
    }
    manageQuestion(sockfd);
}


//############## END ADD/DELETE QUESTION #################

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@ “Do what you can, with what you have, where you are.” ― Theodore Roosevelt

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


//?############## MANAGE EXAM #################


//!TODO: Manage exam
void showExamManagementInterface(){
    printf("Exam management interface!\n");
    printf("1. Search Exam\n");
    printf("2. Create new Exam\n");
    printf("3. Edit Existing Exam\n");
    //printf("4. Delete exam\n");
    printf("4. Back\n");
    printf("Please choose your option: ");
}

void searchExam(int sockfd){
    int exam_id;
    printf("Enter the id of the exam: ");
    scanf(" %d", &exam_id);
    __fpurge(stdin);
    search_exam_by_id search_request;
    search_request.opcode = 701;
    search_request.exam_id = exam_id;
    send(sockfd, &search_request, sizeof(search_request), 0); //Send request to the server to get info of exam
    int numberOfExam;
    recv(sockfd, &numberOfExam, sizeof(int), 0); // Receive number of exam
    if(numberOfExam == 0){
        printf("Cannot find the exam with id %d\n", exam_id);
        searchExam(sockfd);
        return ;
    }
    else{ 
        exam_data exam;
        //Show information of exam
        recv(sockfd, &exam, sizeof(exam_data), 0);
        send(sockfd, "OK", sizeof("OK"), 0);
        printf("Information of exam %d\n", exam.exam_id);
        printf("Title: %s\n", exam.title);
        printf("Number of question: %d\n", exam.number_of_question);
        printf("Author: %d\n", exam.admin_id);
        printf("Do you want to see in detail? (y/n)\n");
        char opt;
        scanf(" %c", &opt);
        if(opt == 'n') return;
        if(opt == 'y'){
            request_question_list request_question;    
            request_question.opcode = 204;
            request_question.exam_id = exam_id;
            request_question.number_of_question = exam.number_of_question;
            send(sockfd, &request_question, sizeof(request_question_list), 0);  //Send request to get question from the server to show for admin

            question_data **question_list;
            question_list = malloc(sizeof(question_data*));
            *question_list = malloc(sizeof(question_data)*(request_question.number_of_question));
            for(int i=0; i<request_question.number_of_question; i++){
                recv(sockfd, (*(question_list)+i), sizeof(question_data), 0); // Receive the question from server
                send(sockfd, "OK", sizeof("OK"), 0);
                printf("Question %d: %s\n", i+1, (*(question_list)+i)->content);
            for(int j=0; j<4; j++){
                printf("%c. %s\n", j+65, (*(question_list)+i)->op[j]);
            }
            }
        }
    }
    manageExam(sockfd);
}

// Cuong
void createRandomExam(int sockfd){
    create_random_exam request;
    request.opcode = 702;
    request.user_id = data.user_id;
    
    printf("Enter the title of exam: \n");
    scanf(" %[^\n]s", request.title);
    __fpurge(stdin);

    request.admin_id = data.user_id;
    
    printf("List of subjects:\n");
    showAllSubjects(sockfd);

    int chosen_subject_index;
    printf("Choose a subject (enter the number): ");
    scanf(" %d", &chosen_subject_index);
    __fpurge(stdin);

    // Get the actual subject name based on the index
    subject_request sub_req;
    sub_req.opcode = 607;
    send(sockfd, &sub_req, sizeof(sub_req), 0);
    int num_of_subject;
    recv(sockfd, &num_of_subject, sizeof(int), 0);
    char subject_list[num_of_subject][150];
    for(int i = 0; i < num_of_subject; i++){
        recv(sockfd, subject_list[i], sizeof(subject_list[i]), 0);
    }
    strcpy(request.subject, subject_list[chosen_subject_index - 1]);

    showCountOfChosenSubject(sockfd, request.subject);

    printf("Enter the number of easy questions: ");
    scanf(" %d", &request.easy_count);
    __fpurge(stdin);

    printf("Enter the number of normal questions: ");
    scanf(" %d", &request.normal_count);
    __fpurge(stdin);

    printf("Enter the number of hard questions: ");
    scanf(" %d", &request.hard_count);
    __fpurge(stdin);

    request.number_of_question = request.easy_count + request.normal_count + request.hard_count;

    send(sockfd, &request, sizeof(request), 0); // Send request to create random exam
    
    int reply;
    recv(sockfd, &reply, sizeof(int), 0); // Receive reply from server ( 1 is exam is created successfully)
    if(reply == 1){
        printf("The exam is created\n");
    }
    else{
        printf("Cannot create the exam\n");
    }
    manageExam(sockfd);
}

// Cuong
void createExamByQuestion(int sockfd){
    create_exam request;
    char question_id[51];
    request.opcode = 703;
    int reply;
    printf("Enter the name of the exam: \n");
    scanf("%s", request.title);
    printf("Enter the number of question in the exam: \n");
    scanf("%d", &request.number_of_ques);
    send(sockfd, &request, sizeof(create_exam), 0); // Send request to server to create exam- opcode = 703
    int num_of_question;
    recv(sockfd, &num_of_question, sizeof(int), 0); // Receive number of question from server
    char question_ids[num_of_question][51];
    for(int i = 0; i < request.number_of_ques; i++){
        printf("Enter the question id to add: \n");
        scanf(" %s", question_ids[i]);
    }
    send(sockfd, question_ids, sizeof(question_ids), 0); // Send the signal to end recv in server
    recv(sockfd, &reply, sizeof(int), 0); //Receive reply from server ( 1 if the exam was created)
    if(reply == 1){
        printf("The exam has been created\n");
    }
    else{
        printf("Cannot create the exam\n");
    }
    manageExam(sockfd);
}
// Cuong
void createExam(int sockfd){
    int option;
    printf("1. Create a random exam\n");
    printf("2. Create exam from chosen question\n");
    printf("3. Back\n");
    printf("Choose an option to start create!\n");
    scanf("%d",&option);
    switch(option){
        case 1:
            createRandomExam(sockfd);
            break;
        case 2:
            createExamByQuestion(sockfd);
            break;
        case 3:
            manageExam(sockfd);
            break;
        default:
            createExam(sockfd);
            break;
    }
}

// void deleteExam(int sockfd){
//     int exam_id;
//     printf("Enter the id of the exam: ");
//     scanf(" %d", &exam_id);
//     __fpurge(stdin);
//     request_edit request;
//     request.opcode = 704;
//     request.exam_id = exam_id;
//     request.type = 0;
//     send(sockfd, &request, sizeof(request_edit), 0); // Send request to delete exam
//     int reply;
//     recv(sockfd, &reply, sizeof(int), 0); // Receive reply from server ( 1 is exam is deleted successfully)
//     if(reply == 1){
//         printf("The exam is deleted\n");
//     }
//     else{
//         printf("Cannot delete the exam\n");
//     }
//     manageExam(sockfd);
// }

void manageExam(int sockfd){
    showExamManagementInterface();
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
    switch(option){
        case 1:
            searchExam(sockfd);
            return;
        case 2:
            createExam(sockfd);
            return;
        case 3:
            //editExam(sockfd);
            return;
        case 4:
            UIMainAppMenuAdmin(sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
    }
    manageExam(sockfd);
}

//?############## END MANAGE EXAM #################


/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@ “If everything seems to be under control, you’re not going fast enough.” ―Mario Andretti

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


//?############## MANAGE EXAM ROOM #################

//!TODO: Manage exam room
void manageExamRoom(int client_sockfd){
    printf("MANAGE ROOM MENU!\n");
    printf("1. Create a room\n");
    printf("2. Delete a room\n");
    printf("3. Show your rooms\n");
    printf("4. Main application system!\n");
    printf("Please choose your option: ");
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
        switch(option){
        case 1:
            createRoom(client_sockfd);
            break;
        case 2:
            deleteRoom(client_sockfd);
            break;
        case 3:
            showYourRoom(client_sockfd);
            break;
        case 4:
            UIMainAppMenuAdmin(client_sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
        }
    manageExamRoom(client_sockfd);
}

void createRandomExamFromRoom(int sockfd){
    create_random_exam request;
    request.opcode = 702;
    request.user_id = data.user_id;
    
    printf("Enter the title of exam: \n");
    scanf(" %[^\n]s", request.title);
    __fpurge(stdin);

    request.admin_id = data.user_id;
    
    printf("List of subjects:\n");
    showAllSubjects(sockfd);

    int chosen_subject_index;
    printf("Choose a subject (enter the number): ");
    scanf(" %d", &chosen_subject_index);
    __fpurge(stdin);

    // Get the actual subject name based on the index
    subject_request sub_req;
    sub_req.opcode = 607;
    send(sockfd, &sub_req, sizeof(sub_req), 0);
    int num_of_subject;
    recv(sockfd, &num_of_subject, sizeof(int), 0);
    char subject_list[num_of_subject][150];
    for(int i = 0; i < num_of_subject; i++){
        recv(sockfd, subject_list[i], sizeof(subject_list[i]), 0);
    }
    strcpy(request.subject, subject_list[chosen_subject_index - 1]);

    showCountOfChosenSubject(sockfd, request.subject);

    printf("Enter the number of easy questions: ");
    scanf(" %d", &request.easy_count);
    __fpurge(stdin);

    printf("Enter the number of normal questions: ");
    scanf(" %d", &request.normal_count);
    __fpurge(stdin);

    printf("Enter the number of hard questions: ");
    scanf(" %d", &request.hard_count);
    __fpurge(stdin);

    request.number_of_question = request.easy_count + request.normal_count + request.hard_count;

    send(sockfd, &request, sizeof(request), 0); // Send request to create random exam
    
    int reply;
    recv(sockfd, &reply, sizeof(int), 0); // Receive reply from server ( 1 is exam is created successfully)
    if(reply == 1){
        printf("The exam is created\n");
    }
    else{
        printf("Cannot create the exam\n");
    }
}

void createExamByQuestionFromRoom(int sockfd){

}

void createExamFromRoom(int sockfd){
    int option;
    printf("1. Create a random exam\n");
    printf("2. Create exam from chosen question\n");
    printf("Choose an option to start create!\n");
    scanf("%d",&option);
    switch(option){
        case 1:
            createRandomExamFromRoom(sockfd);
            break;
        case 2:
            createExamByQuestionFromRoom(sockfd);
            break;
        default:
            createExamFromRoom(sockfd);
            break;
    }
}

void createRoom(int client_sockfd){
    //opcode = 801
    char r_name[128];
    char r_open_hour[64], r_open_date[64], r_close_hour[64], r_close_date[64];
    char r_open_time[128]; // = r_start_date + r_start_hour
    char r_close_time[128]; // = r_end_date + r_end_hour
    int r_complete_time; // time to complete the exam

    printf("Create a room!\n");
    // Get room name
    printf("Enter the room's name: ");
    fgets(r_name, 127, stdin);
    r_name[strlen(r_name)-1] = '\0';
    __fpurge(stdin);

    // Get open time
    printf("Enter the open date (yy-mm-dd): ");
    fgets(r_open_date, 64, stdin);
    r_open_date[strlen(r_open_date) - 1] = '\0';
    __fpurge(stdin);

    printf("Enter the open hour (hh:mm:ss): ");
    fgets(r_open_hour, 64, stdin);
    r_open_hour[strlen(r_open_hour) - 1] = '\0';
    __fpurge(stdin);

    // Get close time
    printf("Enter the close date (yy-mm-dd): ");
    fgets(r_close_date, 64, stdin);
    r_close_date[strlen(r_close_date) - 1] = '\0';
    __fpurge(stdin);

    printf("Enter the close hour (hh:mm:ss): ");
    fgets(r_close_hour, 64, stdin);
    r_close_hour[strlen(r_close_hour) - 1] = '\0';
    __fpurge(stdin);

    // Get completetion time
    printf("Enter the complete time (in minutes): ");
    scanf("%d", &r_complete_time);
    __fpurge(stdin);

    strcpy(r_open_time, r_open_date);
    strcat(r_open_time, " ");
    strcat(r_open_time, r_open_hour);
    r_open_time[strlen(r_open_time)] = '\0';

    strcpy(r_close_time, r_close_date);
    strcat(r_close_time, " ");
    strcat(r_close_time, r_close_hour);
    r_close_time[strlen(r_close_time)] = '\0';

    printf("The room will open from %s and close on %s\n", r_open_time, r_close_time);
    printf("Time to complete the exam: %d minutes\n", r_complete_time);

    room_create_del room;
    room.opcode = 801;
    strcpy(room.r_name, r_name);
    room.r_name[strlen(room.r_name)] = '\0';
    strcpy(room.username, data.username);
    room.username[strlen(room.username)] = '\0';
    strcpy(room.open_time, r_open_time);
    room.open_time[strlen(room.open_time)] = '\0';
    strcpy(room.close_time, r_close_time);
    room.close_time[strlen(room.close_time)] = '\0';
    room.complete_time = r_complete_time;

    // printf("%s\n", room.open_time);
    send(client_sockfd, &room, sizeof(room), 0);

    int oke_signal;
    recv(client_sockfd, &oke_signal, sizeof(int), 0);
    if(oke_signal == -1){
        printf("The room's name '%s' is already existed\n\n", room.r_name);
    }
    else{
        // manageExam(client_sockfd);
        createExamFromRoom(client_sockfd);

        MYSQL *room_conn;

        char *server = "localhost";
        char *user = "root";
        char *password = "123456"; 
        char *database = "network_db_01";

        room_conn = mysql_init(NULL);

        if (!mysql_real_connect(room_conn, server, user, password, database, 0, NULL, 0)) {
            exit(1);
        }

        char query[MAX_QUERY_LEN];
        // strcpy(query, "SELECT test_id FROM `TEST` WHERE adminid = ");
        // strcat(query, data.user_id);
        sprintf(query, "SELECT test_id FROM Test WHERE admin_id = %d ORDER BY test_id DESC LIMIT 1;", data.user_id);
        // strcat(query, ";");
        // strcat(query, " ORDER BY test_id DESC LIMIT 1;");

        if (mysql_query(room_conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(room_conn));
            exit(1);
        }

        MYSQL_RES *res = mysql_store_result(room_conn);

        int exam_id;

        MYSQL_ROW row = mysql_fetch_row(res);
        exam_id = atoi(row[0]);

        mysql_free_result(res);

        memset(query, 0, MAX_QUERY_LEN);

        sprintf(query, "INSERT INTO Room_test(r_id, test_id) VALUES ((SELECT r_id FROM Room where r_name = '%s'), %d)", room.r_name, exam_id);

        if (mysql_query(room_conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(room_conn));
            exit(1);
        }

        mysql_close(room_conn);

        printf("Room created successfully\n\n");

    }
}

void deleteRoom(int client_sockfd){
    //opcode = 802
    char r_name[128];
    printf("Delete a room!\n");
    printf("Enter the room's name: ");
    fgets(r_name, 127, stdin);
    r_name[strlen(r_name)-1] = '\0';
    __fpurge(stdin);

    room_create_del room;
    room.opcode = 802;
    strcpy(room.r_name, r_name);
    strcpy(room.username, data.username);

    send(client_sockfd, &room, sizeof(room), 0);

    int oke_signal;
    recv(client_sockfd, &oke_signal, sizeof(int), 0);
    // 1 -> success
    // -1 -> that room does not exist
    // -2 -> you don't have permission to delete it
    if(oke_signal == -1){
        printf("The room '%s' does not exist\n\n", room.r_name);
    }
    else if(oke_signal == -2){
        printf("You don't have permission to delete room '%s'\n\n", room.r_name);
    }
    else{
        printf("Room deleted successfully\n\n");
    }
}



//1
void addUserToRoom(int client_sockfd, int r_id){
    int flag;
    request_add_remove_student request;
    while(1){
        printf("\nEnter the user_id of student to you want to add to this room (enter 0 to get back): ");
        scanf("%d", &(request.user_id));
        __fpurge(stdin);

        if(request.user_id == 0){
            return;
        }

        request.opcode = 811;
        request.r_id = r_id;
        
        send(client_sockfd, &request, sizeof(request_add_remove_student), 0); //Send request to add student in a room
        recv(client_sockfd, &flag, sizeof(int), 0); //Receive the reply message from server\

        switch (flag)
        {
        case 1:
            printf("Add successfully: user's id = %d\n to room's id = %d\n", request.user_id, request.r_id);
            break;
        case -2:
            printf("Add fail: no user has id = %d\n", request.user_id);
            break;
        case -3:
            printf("Add fail: user with id = %d has been added to room_id %d before\n", request.user_id, request.r_id);
            break;
        case -4:
            printf("Add fail: user with id = %d is you!\nYou cannot add yourself to the room you created\n", request.user_id);
            break;
        default:
            printf("Add failt: Something went wrong at addUserToRoom()\n");
            break;
        }
    }
}

//2
void deleteUserFromRoom(int client_sockfd, int r_id){
    int flag;
    request_add_remove_student request;
    while(1){
        printf("\nEnter the user-id of student you want to delete from the room (enter 0 to get back): ");

        scanf("%d", &(request.user_id));
        __fpurge(stdin);

        if(request.user_id == 0){
            return;
        }

        request.opcode = 812;
        request.r_id = r_id;
        
        send(client_sockfd, &request, sizeof(request_add_remove_student), 0); //Send request to add student in a room
        recv(client_sockfd, &flag, sizeof(int), 0); //Receive the reply message from server
        if(flag == 1){
            printf("Delete successfully: User with id %d has been removed from this room!\n", request.user_id);
        }
        else if (flag = -2){
            printf("Delete fail: user with id = %d is not in this room\n", request.user_id);
        }
        else{
            printf("Something went wrong in deleteUserFromRoom()\n");
        }
    }
}

//3
void addTestFromBank(int client_sockfd, int r_id){
    //opcode = 813
    int flag;
    add_test_room add_test;
    printf("Enter the test-id you wanted to add to room: \n");
    scanf("%d", &(add_test.test_id));
    add_test.r_id = r_id;
    add_test.opcode = 813;
    send(client_sockfd, &add_test, sizeof(add_test_room), 0); //Send request to add test in a room
    recv(client_sockfd, &flag, sizeof(int), 0); //Receive the reply message from server
    if(flag == 1){
        printf("Test with id %d has been added the room %d successfully!\n", add_test.test_id, r_id);
    }
    else{
        printf("Cannot add the test %d to the room\n", add_test.test_id);
    }
}

//4 - hàm này sẽ tạo test, cho vào bank
//rồi hỏi luôn người đó là có muốn cho test này vào room luôn ko?
void createAndAddTest(int client_sockfd, int r_id){
    //opcode = 814
}

//5
void deleteTest(int client_sockfd, int r_id){
    //opcode = 815
}

//6
void Admin_showSubmittedRecordOfTest(int client_sockfd, int r_id){
    //opcode = 816

}

//7
void Admin_showAllSubmittedRecord(int client_socket, int r_id){
    //opcode = 817
}

void goIntoRoomYouAreAdmin(int client_sockfd, int r_id){
    while(1){
        printf("\nADMIN ROOM MENU !\n");
        printf("1. Add user\n");
        printf("2. Delete user\n");
        printf("3. Add a test from the bank\n");
        printf("4. Create a new test and add to room\n");
        printf("5. Delete a test\n");
        printf("6. Show submitted record of a test\n"); //provide test's name
        printf("7. Show all submitted record\n");
        printf("8. Get back\n");
        printf("Please choose your option: ");
        int option;
        scanf(" %1d", &option);
        __fpurge(stdin);
            switch(option){
            case 1:
                addUserToRoom(client_sockfd, r_id);
                break;
            case 2:
                deleteUserFromRoom(client_sockfd, r_id);
                break;
            case 3:
                addTestFromBank(client_sockfd, r_id);
                return;
            case 4: 
                createAndAddTest(client_sockfd, r_id);
                break;
            case 5:
                deleteTest(client_sockfd, r_id);
                break;
            case 6:
                Admin_showSubmittedRecordOfTest(client_sockfd, r_id);
                break;
            case 7:
                Admin_showAllSubmittedRecord(client_sockfd, r_id);
                break;
            case 8: 
                printf("Exit ROOM MENU\n\n");
                return;
            default:
                printf("Invalid option!\n");
                break;
            }
    }
}





//1-user
void doATestInRoom(int client_sockfd, int r_id){
    MYSQL *room_conn;

    char *server = "localhost";
    char *user = "root";
    char *password = "123456"; 
    char *database = "network_db_01";

    room_conn = mysql_init(NULL);

    if (!mysql_real_connect(room_conn, server, user, password, database, 0, NULL, 0)) {
        exit(1);
    }

    char query[MAX_QUERY_LEN];
    sprintf(query, "SELECT * FROM Test WHERE test_id = (SELECT test_id from Room_test where r_id=%d);", r_id);

    if (mysql_query(room_conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(room_conn));
        exit(1);
    }

    MYSQL_RES *res = mysql_store_result(room_conn);

    MYSQL_ROW row = mysql_fetch_row(res);

    exam_data *exam = malloc(sizeof(exam_data));
    exam->opcode = 201;
    exam->exam_id = atoi(row[0]);
    strcpy(exam->title, row[1]);
    exam->number_of_question = atoi(row[2]);
    exam->admin_id = atoi(row[3]);

    mysql_free_result(res);

    mysql_close(room_conn);

    adminTakeExam(client_sockfd, exam);

    free(exam);
}

//2-user
void User_showSubmittedRecordOfTest(int client_sockfd, int r_id){

}

//3-user
void User_showAllSubmittedRecord(int client_sockfd, int r_id){

}


void goIntoRoomYouAreUser(int client_sockfd, int r_id){
    while(1){
        printf("\nUSER ROOM MENU !\n");
        printf("1. Do a test\n");
        // printf("2. Show submitted record of a test\n");
        // printf("3. Show all submitted record\n");
        printf("2. Get back\n");
        printf("Please choose your option: ");
        int option;
        scanf(" %1d", &option);
        __fpurge(stdin);
            switch(option){
            case 1:
                doATestInRoom(client_sockfd, r_id);
                return;
            // case 2:
            //     User_showSubmittedRecordOfTest(client_sockfd, r_id);
            //     break;
            // case 3:
            //     User_showAllSubmittedRecord(client_sockfd, r_id);
            //     break;
            case 2: 
                printf("Exit USER ROOM MENU\n\n");
                return;
            default:
                printf("Invalid option!\n");
                break;
            }
    }
}

// Việt
void showAllYourRoom(int client_sockfd, int opcode){
    // printf("You are in show all your room\n");
    int num_of_room; // Number of room
    show_your_room your_info;
    room_info room; 
    your_info.opcode = opcode;
    strcpy(your_info.username, data.username);
    send(client_sockfd, &your_info, sizeof(show_your_room), 0); //Send request to get room info to server
    // printf("Client send oke?\n");
    recv(client_sockfd, &num_of_room, sizeof(int), 0); // Receive number of room from server

    // printf("after received\n");

    if(num_of_room <= 0){
        printf("There is no room!\n\n");
        return;
    }

    char oke_signal[OKE_SIGNAL_LEN] = "OK";
    send(client_sockfd, oke_signal, OKE_SIGNAL_LEN, 0);
    
    int r_id_list[num_of_room];
    int r_status_list[num_of_room];
    char r_admin_name_list[num_of_room][256];
    printf("%-15s%-30s%-30s%-20s%-30s%-30s%-15s%-20s\n", "room_id", "room_name", "create_account", "create_date", "open_time", "close_time", "complete_time", "status");
    
    // Get current datetime
    time_t rawtime;
    struct tm *timeinfo;
    char current_time[20];  // Array to hold the current time in the format "YYYY-MM-DD HH:MM:SS"

    // Get current time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format the current time into "YYYY-MM-DD HH:MM:SS"
    strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", timeinfo);

    for(int i=0; i<num_of_room; i++){
        char status[20];
        memset(&room, 0, sizeof(room_info));
        recv(client_sockfd, &room, sizeof(room_info), 0);

        if (strcmp(current_time, room.open_time) < 0){
            strcpy(status, "Not open");
            r_status_list[i] = 0;
        }
        else if (strcmp(current_time, room.open_time) >= 0 && strcmp(current_time, room.close_time) <= 0){
            strcpy(status, "Opening");
            r_status_list[i] = 1;
        }
        else if (strcmp(current_time, room.close_time) > 0){
            strcpy(status, "Closed");
            r_status_list[i] = 2;
        }
        status[strlen(status)] = '\0';

        r_id_list[i] = room.r_id;
        strcpy(r_admin_name_list[i], room.admin_name);
        r_admin_name_list[i][strlen(r_admin_name_list[i])] = '\0';
        printf("%-15d%-30s%-30s%-20s%-30s%-30s%-15d%-20s\n", room.r_id, room.r_name, room.admin_name, room.create_date, room.open_time, room.close_time, room.complete_time, status);
        send(client_sockfd, oke_signal, OKE_SIGNAL_LEN, 0);
    }

    recv(client_sockfd, oke_signal, OKE_SIGNAL_LEN, 0);

    int go_r_id;
    int valid = 0; //valid = 1 -> you are admin in that room 
                   //valid = 2 -> you are user in that room
    while(1){
        printf("Enter the id of the room you want to go in (enter 0 to get back): ");
        scanf("%d", &go_r_id);
        __fpurge(stdin);
        if(go_r_id == 0){
            return;
        }
        for(int i=0; i<num_of_room; i++){
            if(go_r_id == r_id_list[i]){
                {
                    if (r_status_list[i] == 0){
                        valid = -1;
                        break;
                    }
                    else if (r_status_list[i] == 1){
                        if(strcpy(r_admin_name_list[i], data.username) == 0){
                            valid = 1;
                        }
                        else{
                            valid = 2;
                        }
                    }
                    else{
                        valid = -2;
                    }
                }
            }
        }
        if(valid > 0){
            break;
        }
        // printf("Invalid Option!\n");
        if (valid == -1){
            printf("The room does not started yet!\n");
        }
        else if (valid == -2){
            printf("The room has been closed!\n");
        }
    }
    // printf("Choose room id: %d\n", go_r_id);
    // printf("Valid: %d\n", valid);
    if(valid == 1){
        goIntoRoomYouAreAdmin(client_sockfd, go_r_id);
    }
    else if(valid == 2){
        goIntoRoomYouAreUser(client_sockfd, go_r_id);
    }
    else{
        printf("Something went wrong in showYourRoom()\n");
    }
}

void showYourRoom(int client_sockfd){
    while(1){
        printf("YOUR ROOM MENU!\n");
        printf("1. Show rooms you created\n");
        printf("2. Show rooms you were added\n");
        printf("3. Show all your rooms\n");
        printf("4. Get back\n");
        printf("Please choose your option: ");
        int option;
        scanf(" %1d", &option);
        __fpurge(stdin);
            switch(option){
            case 1:
                showAllYourRoom(client_sockfd, 803);
                break;
            case 2:
                showAllYourRoom(client_sockfd, 804);
                break;
            case 3:
                showAllYourRoom(client_sockfd, 805);
                break;
            case 4:
                return;
            default:
                printf("Invalid option!\n");
                break;
            }
    }
}





//?############## END MANAGE EXAM ROOM #################


/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//@ “The only difference between ordinary and extraordinary is that little extra.” ―Jimmy Johnson

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


//?############## GET EXAM ROOM RESULT #################

//!TODO: Get exam room result
void getExamRoomResult(int sockfd){

}

//?############## END GET EXAM ROOM RESULT #################

//!TODO: Get user info
void getUserInfo(int sockfd){
    request_user_info info;
    info.opcode = 103;
    info.user_id = data.user_id;
    char username[30];
    printf("Enter student's name: ");
    scanf(" %s", username);
    strcpy(info.username, username);
    send(sockfd, &info, sizeof(info), 0);
    int check;
    recv(sockfd, &check, sizeof(int), 0);
    if(check == 1){
        printf("Get user info successfully!\n");
        send(sockfd, "OK", sizeof("OK"), 0);
        register_data user_info;
        recv(sockfd, &user_info, sizeof(user_info), 0);
        //Print user info
        printUserInfo(user_info);
        UIMainAppMenuAdmin(sockfd);
    }
    else{
        printf("Cannot get user info!\n");
        UIMainAppMenuAdmin(sockfd);
        return;
    }
}

//!TODO: Approve admin request
void approveAdminRequest(int sockfd){
    check_admin_previlege_request request;
    request.opcode = 302;
    request.user_id = 0;
    request.next_page = 0;
    send(sockfd, &request, sizeof(request), 0);
    int numOfRequest;
    recv(sockfd, &numOfRequest, sizeof(int), 0);
    if(numOfRequest == 0){
        printf("No request found!\n");
        UIMainAppMenuAdmin(sockfd);
        return;
    }
    else{
        int numCurrentRequest = 0;
        while(numCurrentRequest < numOfRequest){
            printf("Found %d request(s)!\n", numOfRequest);
            admin_request **request_list;
            request_list = malloc(sizeof(admin_request*)*numOfRequest);
            *request_list = malloc(sizeof(admin_request)*numOfRequest);
            send(sockfd, &numCurrentRequest, 4, 0); // Send the begin index to search question
            // Receive 10 question from server
            int num_request_recv = 0;
            recv(sockfd, &num_request_recv, 4, 0);
            for(int i = 0; i < num_request_recv; i++){
                send(sockfd, "OK", sizeof("OK"), 0);
                recv(sockfd, (*(request_list)+i), sizeof(admin_request), 0);
                printf("Request %d: %s\tDate: %s\n", (*(request_list)+i)->id, (*(request_list)+i)->username, (*(request_list)+i)->date);
            }
            numCurrentRequest += 10;
            printf("\n1. Approve request\n");
            //printf("2. Next page\n");
            printf("2. Back\n");
            printf("Please choose your option: ");
            int option;
            scanf(" %1d", &option);
            __fpurge(stdin);
            switch(option){
                case 1:
                    char username[30];
                    printf("Please enter username to approve request: ");
                    scanf(" %s", username);
                    __fpurge(stdin);
                    approve_admin_request approve_request;
                    approve_request.user_id = 0;
                    approve_request.opcode = 304;
                    strcpy(approve_request.username, username);
                    send(sockfd, &approve_request, sizeof(approve_request), 0);
                    int reply;
                    recv(sockfd, &reply, sizeof(int), 0);
                    printf("Reply from server %d", reply);
                    if(reply == 1){
                        printf("Approve request successfully!\n");
                    }
                    else{
                        printf("Cannot approve request!\n");
                    }
                    printf("Do you want to approve another request? (y/n)\n");
                    char opt;
                    scanf(" %c", &opt);
                    __fpurge(stdin);
                    if(opt != 'y'){
                        UIMainAppMenuAdmin(sockfd);
                        return;
                    }
                case 2:
                    request.next_page = 1;
                    send(sockfd, &request, sizeof(request), 0);
                    break;
                default:
                    UIMainAppMenuAdmin(sockfd);
                    return;
            } while(1);
        }
    }
}


