#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../data.h"
#include "client.h"
#include "utils.h"
#include "show_image.c"
#include <mysql/mysql.h>


#define MAX_INPUT_LENGTH 512
#define MAX_QUERY_LEN 3000

exam_data** getExamList(int client_sockfd, int number_of_exam);
void printExamInfo(exam_data* exam);
void send_answer(int client_sockfd, exam_data *exam);
void take_exam(int client_sockfd, exam_data* exam);
void seeExamHistrory(int client_socket);
void seeUserInfo(int client_sockfd);
void request_admin(int client_sockfd);
void UIMainAppMenu(int client_sockfd);
void printExamList(exam_data** exams, int number_of_exam);
void showExamList(int client_sockfd);
void processAnswer(char *str);
void showAdvancedFeaturesMenu(int sockfd);
// exam_data** exams;


// exam_data** getExamList(int client_sockfd, int number_of_exam){
//     request_exam_list request;
//     request.opcode = 203;
//     request.number_of_exam = number_of_exam;
//     request.user_id = data.user_id;
//     send(client_sockfd, &request, sizeof(request), 0);
//     exam_data** exams = malloc(number_of_exam * sizeof(exam_data*)); // Allocate memory for the array of pointers
//     for (int i = 0; i < number_of_exam; i++) {
//         exams[i] = malloc(sizeof(exam_data)); // Allocate memory for each exam_data
//         memset(exams[i], 0, sizeof(exam_data)); // Initialize each exam_data to 0
//     }
//     for(int i = 0; i < number_of_exam; i++) {
//         recv(client_sockfd, exams[i], sizeof(exam_data), MSG_WAITALL);
//         send(client_sockfd, "OK", sizeof("OK"), 0);
//     }
//     return exams;
// }

// exam_data** getExamList(int client_sockfd, int number_of_exam){
//     request_exam_list request;
//     request.opcode = 203;
//     request.number_of_exam = number_of_exam;
//     request.user_id = data.user_id;
//     send(client_sockfd, &request, sizeof(request), 0);
//     // recv(client_sockfd, &number_of_exam, sizeof(number_of_exam), 0);
//     printf("Number of exam: %d\n", number_of_exam);
//     exam_data** exams;
//     exams = malloc(sizeof(exam_data*));
//     *exams = malloc(sizeof(exam_data)*number_of_exam);
//     for(int i = 0; i < number_of_exam; i++){
//         recv(client_sockfd, (*(exams)+i), sizeof(exam_data), 0);
//         send(client_sockfd, "OK", sizeof("OK"), 0);
//     }
//     return exams;
// }

exam_data** getExamList(int client_sockfd, int number_of_exam) {
    request_exam_list request;
    request.opcode = 203;
    printf("---%d---\n", request.opcode);
    request.number_of_exam = number_of_exam;
    request.user_id = data.user_id;
    long unsigned int sent_bytes = send(client_sockfd, &request, sizeof(request), 0);
    if(sent_bytes<0){
        printf("Resend request\n");
        send(client_sockfd, &request, sizeof(request), 0);
    }

    printf("Number of exam: %d\n", number_of_exam);

    exam_data** exams = malloc(sizeof(exam_data*));
    *exams = malloc(sizeof(exam_data) * number_of_exam);

    for (int i = 0; i < number_of_exam; i++) {
        memset((*exams) + i, 0, sizeof(exam_data));
        recv(client_sockfd, (*exams) + i, sizeof(exam_data), 0);
        send(client_sockfd, "OK", 2, 0);
    }
    return exams;
}


void printExamInfo(exam_data* exam){
    printf("Exam info:\n");
    printf("Exam id: %d\n", exam->exam_id);
    printf("Exam title: %s\n", exam->title);
    printf("Number of question: %d\n", exam->number_of_question);
}

void send_answer(int client_sockfd, exam_data *exam){
    char answer[exam->number_of_question+1];
    while(1){
        printf("Enter your answer: ");
        scanf(" %s", answer);
        processAnswer(answer); //Process answer
        int flag = 1;
        for(int i = 0; i < strlen(answer); i++){
            if(!( answer[i] >= 'A' && answer[i] <= 'Z')){
                flag = 0;
            }
        }
        if(strlen(answer) == exam->number_of_question && flag == 1){
            break;
        }
        else{
            printf("Enter your answer: (must be A, B, C or D and number of answers equal to number of questions)\n");
        }
    }
    printf("Your answer: \n");
    for(int i = 0; i < exam->number_of_question; i++){
        printf("Question %d: %c\n", i+1, answer[i]);
    }
    //!TODO Check answer validation and send to server for evaluation
    char option;
    printf("Do you want to submit the answer? (y/n)\n");
    scanf(" %1c", &option);
    if(option == 'y'){
        submit_ans ans;
        ans.opcode = 205;
        ans.exam_id = exam->exam_id;
        ans.user_id = data.user_id;
        strcpy(ans.username, data.username);
        strcpy(ans.submit_ans, answer);
        ans.number_of_question = exam->number_of_question;
        send(client_sockfd, &ans, sizeof(submit_ans), 0); //Send the answer to server
    }else{
        send_answer(client_sockfd, exam);
    }
}

// void take_exam(int client_sockfd, exam_data* exam) {
//     printExamInfo(exam);
//     printf("Do you want to take this exam? (y/n): ");
//     char option;
//     scanf(" %1c", &option);
//     __fpurge(stdin);

//     if (option == 'n') {
//         showExamList(client_sockfd);
//         return;
//     } else if (option == 'y') {
//         printf("\n\n##########################################\n\n");
//         printf("Exam: %s\n\n", exam->title);

//         request_question_list request_question_list;
//         request_question_list.opcode = 204;
//         request_question_list.user_id = data.user_id;
//         request_question_list.exam_id = exam->exam_id;
//         request_question_list.number_of_question = exam->number_of_question;
//         send(client_sockfd, &request_question_list, sizeof(request_question_list), 0);

//         // Allocate memory for question list
//         question_data *question_list = malloc(sizeof(question_data) * exam->number_of_question);
//         if (!question_list) {
//             perror("Memory allocation failed");
//             exit(EXIT_FAILURE);
//         }

//         // Array to store user's answers
//         char user_answers[exam->number_of_question];
//         memset(user_answers, 0, sizeof(user_answers));

//         for (int i = 0; i < exam->number_of_question; i++) {
//             recv(client_sockfd, &question_list[i], sizeof(question_data), MSG_WAITALL);
//             send(client_sockfd, "OK", sizeof("OK"), 0);
//         }

//         int current_question = 0;
//         while (current_question < exam->number_of_question) {
//             printf("Question %d: %s\n", current_question + 1, question_list[current_question].content);
//             for (int j = 0; j < 4; j++) {
//                 printf("%c. %s\n", j + 65, question_list[current_question].op[j]);
//             }

//             if (user_answers[current_question] != 0) {
//                 printf("Your current answer: %c\n", user_answers[current_question]);
//             }

//             printf("Enter your answer (A, B, C, D, P: Previous, N: Next, S: Submit): ");
//             char ans;
//             scanf(" %c", &ans);
//             __fpurge(stdin);
//             ans = toupper(ans);

//             if (ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D') {
//                 user_answers[current_question] = ans;
//                 if (current_question < exam->number_of_question - 1) {
//                     current_question++;
//                 } else {
//                     goto submit_section;
//                 }
//             } else if (ans == 'P' && current_question > 0) {
//                 current_question--;
//             } else if (ans == 'N' && current_question < exam->number_of_question - 1) {
//                 current_question++;
//             } else if (ans == 'S') {
//             submit_section:
//                 while (1) {
//                     printf("\nYour final answers:\n");
//                     for (int i = 0; i < exam->number_of_question; i++) {
//                         printf("Question %d: %c\n", i + 1, (user_answers[i] == 0) ? ' ' : user_answers[i]);
//                     }

//                     printf("\nDo you want to change any answer? (Enter question number to change, or 'EXIT' to submit): ");
//                     char change_choice[10];
//                     scanf(" %s", change_choice);
//                     __fpurge(stdin);

//                     if (strcasecmp(change_choice, "EXIT") == 0) {
//                         break;
//                     } else {
//                         int question_to_change = atoi(change_choice);
//                         if (question_to_change >= 1 && question_to_change <= exam->number_of_question) {
//                             printf("Question %d: %s\n", question_to_change, question_list[question_to_change - 1].content);
//                             for (int j = 0; j < 4; j++) {
//                                 printf("%c. %s\n", j + 65, question_list[question_to_change - 1].op[j]);
//                             }
//                             printf("Enter new answer for question %d (A, B, C, D): ", question_to_change);
//                             char new_ans;
//                             scanf(" %c", &new_ans);
//                             __fpurge(stdin);
//                             new_ans = toupper(new_ans);
//                             if (new_ans == 'A' || new_ans == 'B' || new_ans == 'C' || new_ans == 'D') {
//                                 user_answers[question_to_change - 1] = new_ans;
//                             } else {
//                                 printf("Invalid answer.\n");
//                             }
//                         } else {
//                             printf("Invalid question number.\n");
//                         }
//                     }
//                 }

//                 printf("Are you sure you want to submit? (y/n): ");
//                 char submit_choice;
//                 scanf(" %c", &submit_choice);
//                 __fpurge(stdin);
//                 if (submit_choice == 'y') {
//                     break;
//                 }
//             } else {
//                 printf("Invalid option!\n");
//             }
//         }

//         char final_answer[exam->number_of_question + 1];
//         for (int i = 0; i < exam->number_of_question; i++) {
//             final_answer[i] = (user_answers[i] == 0) ? ' ' : user_answers[i];
//         }
//         final_answer[exam->number_of_question] = '\0';

//         submit_ans ans;
//         ans.opcode = 205;
//         ans.exam_id = exam->exam_id;
//         ans.user_id = data.user_id;
//         strcpy(ans.username, data.username);
//         strcpy(ans.submit_ans, final_answer);
//         ans.number_of_question = exam->number_of_question;
//         send(client_sockfd, &ans, sizeof(submit_ans), 0);

//         int result;
//         recv(client_sockfd, &result, sizeof(result), 0);
//         printf("\nYour score: %d/%d\n\n", result, exam->number_of_question);

//         printf("Do you want to see detailed results and corrections? (y/n): ");
//         char see_detail;
//         while (1) {
//             scanf(" %c", &see_detail);
//             fflush(stdin);
//             send(client_sockfd, &see_detail, 1, 0);
//             if (see_detail == 'y') {
//                 char correct_ans[exam->number_of_question + 1];
//                 recv(client_sockfd, correct_ans, exam->number_of_question, MSG_WAITALL);
//                 correct_ans[exam->number_of_question] = '\0';
//                 for (int i = 0; i < exam->number_of_question; i++) {
//                     printf("Question %d: %s\n", i + 1, question_list[i].content);
//                     for (int j = 0; j < 4; j++) {
//                         printf("%c. %s\n", 'A' + j, question_list[i].op[j]);
//                     }
//                     printf("Your answer: %c   -   Correct answer: %c\n", final_answer[i], correct_ans[i]);
//                 }
//                 memset(correct_ans, 0, sizeof(correct_ans));
//                 fflush(stdin);
//                 // int c;
//                 // while ((c = getchar()) != '\n' && c != EOF);
//                 break;
//             } else if (see_detail == 'n') {
//                 break;
//             } else {
//                 printf("Invalid option! Please enter 'y' or 'n': ");
//             }
//         }
//         // free(question_list);
//         if(question_list != NULL) free(question_list);
//         fflush(stdin);
//         // int c;
//         // while ((c = getchar()) != '\n' && c != EOF);
//         showExamList(client_sockfd);
//         // UIMainAppMenu(client_sockfd);
//     } else {
//         printf("Invalid option!\n");
//         take_exam(client_sockfd, exam);
//         return;
//     }
// }

// void take_exam(int client_sockfd, exam_data* exam){
//     printExamInfo(exam);
//     printf("Do you want to take this exam?(y/n): ");
//     char option;
//     scanf(" %1c", &option);
//     __fpurge(stdin);
//     if(option == 'n'){
//         showExamList(client_sockfd);
//         return;
//     }
//     else if(option == 'y') {
//         printf("\n\n##########################################\n\n");
//         printf("Exam: %s\n\n", (exam->title));

//         request_question_list request_question_list;
//         request_question_list.opcode = 204;
//         request_question_list.user_id = data.user_id;
//         request_question_list.exam_id = exam->exam_id;
//         request_question_list.number_of_question = exam->number_of_question;
//         send(client_sockfd, &request_question_list, sizeof(request_question_list), 0);

//         //Receive question list
//         question_data **question_list;
//         question_list = malloc(sizeof(question_data*));
//         *question_list = malloc(sizeof(question_data)*(exam->number_of_question));

//         // Array to store user's answers
//         char user_answers[exam->number_of_question];
//         memset(user_answers, 0, sizeof(user_answers)); // Initialize all answers to 0 (unanswered)

//         for(int i=0; i<exam->number_of_question; i++){
//             recv(client_sockfd, (*(question_list)+i), sizeof(question_data), MSG_WAITALL);
//             send(client_sockfd, "OK", sizeof("OK"), 0);
//         }

//         int current_question = 0;
//         while(current_question < exam->number_of_question){

//             printf("Question %d: %s\n", current_question+1, (*(question_list)+current_question)->content);
//             for(int j=0; j<4; j++){
//                 printf("%c. %s\n", j+65, (*(question_list)+current_question)->op[j]);
//             }

//             // Display the current answer (if any)
//             if (user_answers[current_question] != 0) {
//                 printf("Your current answer: %c\n", user_answers[current_question]);
//             }

//             printf("Enter your answer (A, B, C, D, P: Previous, N: Next, S: Submit): ");
//             char ans;
//             scanf(" %c", &ans);
//             __fpurge(stdin);
//             ans = toupper(ans); // Convert answer to uppercase

//             if (ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D') {
//                 user_answers[current_question] = ans;
//                 // Check if it's the last question before incrementing
//                 if (current_question < exam->number_of_question - 1) {
//                     current_question++;
//                 } else {
//                     // If it's the last question, jump to the submit section
//                     goto submit_section;
//                 }
//             } else if (ans == 'P') {
//                 // Go to the previous question, if any
//                 if (current_question > 0) {
//                     current_question--;
//                 }
//             } else if (ans == 'N') {
//                 // Go to the next question, if any
//                 if (current_question < exam->number_of_question - 1) {
//                     current_question++;
//                 } else {
//                     // If it's the last question, jump to the submit section
//                     goto submit_section;
//                 }
//             } else if (ans == 'S') {
//             submit_section: // Label to jump to
//                 // Display answers and allow changes
//                 while (1) {
//                     printf("\nYour final answers:\n");
//                     for (int i = 0; i < exam->number_of_question; i++) {
//                         printf("Question %d: %c\n", i + 1, (user_answers[i] == 0) ? ' ' : user_answers[i]);
//                     }
//                     printf("\nDo you want to change any answer? (Enter question number to change, or 'EXIT' to submit): ");
//                     char change_choice[10];
//                     scanf(" %s", change_choice);
//                     __fpurge(stdin);

//                     if (strcmp(change_choice, "EXIT") == 0||strcmp(change_choice, "exit") == 0) {
//                         break; // Exit the loop to submit
//                     } else {
//                         int question_to_change = atoi(change_choice);
//                         if (question_to_change >= 1 && question_to_change <= exam->number_of_question) {
//                             // Display the question to be changed
//                             printf("Question %d: %s\n", question_to_change, (*(question_list) + question_to_change - 1)->content);
//                             for (int j = 0; j < 4; j++) {
//                                 printf("%c. %s\n", j + 65, (*(question_list) + question_to_change - 1)->op[j]);
//                             }
//                             printf("Enter new answer for question %d (A, B, C, D): ", question_to_change);
//                             char new_ans;
//                             scanf(" %c", &new_ans);
//                             __fpurge(stdin);
//                             new_ans = toupper(new_ans);
//                             if (new_ans == 'A' || new_ans == 'B' || new_ans == 'C' || new_ans == 'D'){
//                                 user_answers[question_to_change - 1] = new_ans;
//                             } else {
//                                 printf("Invalid answer.\n");
//                             }
//                         } else {
//                             printf("Invalid question number.\n");
//                         }
//                     }
//                 }

//                 printf("Are you sure you want to submit? (y/n): ");
//                 char submit_choice;
//                 scanf(" %c", &submit_choice);
//                 __fpurge(stdin);
//                 if (submit_choice == 'y') {
//                     break; // Exit the outer while loop to submit
//                 }
//             } else {
//                 printf("Invalid option!\n");
//             }  
//         }

//         // Prepare the final answer string
//         char final_answer[exam->number_of_question + 1];
//         for(int i = 0; i < exam->number_of_question; i++){
//             final_answer[i] = (user_answers[i] == 0) ? ' ' : user_answers[i]; 
//         }
//         final_answer[exam->number_of_question] = '\0';

//         // Send the answer to server
//         submit_ans ans;
//         ans.opcode = 205;
//         ans.exam_id = exam->exam_id;
//         ans.user_id = data.user_id;
//         strcpy(ans.username, data.username);
//         strcpy(ans.submit_ans, final_answer);
//         ans.number_of_question = exam->number_of_question;
//         send(client_sockfd, &ans, sizeof(submit_ans), 0); 
//         // Receive the result
//         int result;
//         recv(client_sockfd, &result, sizeof(result), 0);
//         printf("\nYour score: %d/%d\n", result, exam->number_of_question);
//         printf("\n");
//         detail_result: 
//             printf("Are you want to see detail result and corrections? (y/n): ");
//             char see_detail;
//             scanf(" %c", &see_detail);
//             __fpurge(stdin);
//             send(client_sockfd, &see_detail, 1, 0);
//             if(see_detail == 'y') {
//                 char correct_ans[exam->number_of_question+1];
//                 recv(client_sockfd, correct_ans, exam->number_of_question , MSG_WAITALL);
//                 correct_ans[exam->number_of_question] = '\0';
//                 for(int i = 0; i < exam->number_of_question; i++){
//                     printf("Question %d: %s\n", i+1, (*(question_list)+i)->content);
//                     for(int j = 0; j < 4; j++){
//                         printf("%c. %s\n", j+65, (*(question_list)+i)->op[j]);
//                     }
//                     printf("Your answer: %c   -   Correct answer: %c\n", final_answer[i] , correct_ans[i]);

//                 }
//                 // memset(final_answer, 0, sizeof(final_answer));
//                 memset(correct_ans, 0, sizeof(correct_ans));
                    
//                 printf("\nDo you want to ask for explanation with AI Assistant? (y/n): ");
//                 char ai_assistant;
//                 while(1) {
//                     scanf(" %c", &ai_assistant);
//                     __fpurge(stdin);
//                     if(ai_assistant == 'y') { 
//                         printf("AI Assistant is ready to help you! Type 'exit' to quit.\n");
//                         ask_ai_data ai_question;
//                         memset(&ai_question, 0, sizeof(ai_question));
//                         while(1) {
//                             printf("Enter the question you want to ask: ");
//                             char question_number[5];
//                             char prompt[MAX_INPUT_LENGTH];
//                             scanf(" %s", question_number);
//                             __fpurge(stdin);
//                             if(strcmp(question_number, "exit") == 0){
//                                 strcpy(prompt, "exit");
//                             } else {
//                                 int question_index = atoi(question_number);
//                                 strcpy(prompt, (*(question_list)+question_index-1)->content);
//                                 for (int j = 0; j < 4; j++) {
//                                     strcat(prompt, "\n");
//                                     strcat(prompt, (*(question_list)+question_index-1)->op[j]);
//                                 }
//                             }
//                             ai_question.opcode = 998;
//                             ai_question.user_id = data.user_id;
//                             strcpy(ai_question.question, prompt);
//                             send(client_sockfd, &ai_question, sizeof(ai_question), 0);
//                             if(strcmp(prompt, "exit") == 0){
//                                 break;
//                             }
//                             char answer[MAX_QUERY_LEN];
//                             memset(answer, 0, sizeof(answer));
//                             recv(client_sockfd, answer, sizeof(answer), 0);
//                             answer[strlen(answer)] = '\0';
//                             printf("%s\n", answer);                  
//                         }
//                         memset(&ai_question, 0, sizeof(ai_question));
//                         break;
//                     } else if(ai_assistant == 'n'){
//                         break;
//                     } else {
//                         printf("Invalid option!\n");
//                         // goto ask_ai;
//                     }
//                 }
                    
//                     // scanf(" %c", &ai_assistant);
//                     // __fpurge(stdin);
//                     // if(ai_assistant == 'y') { 
//                     //     printf("AI Assistant is ready to help you! Type 'exit' to quit.\n");
//                     //     ask_ai_data ai_question;
//                     //     memset(&ai_question, 0, sizeof(ai_question));
//                     //     while(1){
//                     //         printf("Enter the question you want to ask: ");
//                     //         char question_number[5];
//                     //         char prompt[MAX_INPUT_LENGTH];
//                     //         scanf(" %s", question_number);
//                     //         __fpurge(stdin);
//                     //         if(strcmp(question_number, "exit") == 0){
//                     //             strcpy(prompt, "exit");
//                     //         } else {
//                     //             int question_index = atoi(question_number);
//                     //             strcpy(prompt, (*(question_list)+question_index-1)->content);
//                     //             for (int j = 0; j < 4; j++) {
//                     //                 strcat(prompt, "\n");
//                     //                 strcat(prompt, (*(question_list)+question_index-1)->op[j]);
//                     //             }
//                     //         }
//                     //         ai_question.opcode = 998;
//                     //         ai_question.user_id = data.user_id;
//                     //         strcpy(ai_question.question, prompt);
//                     //         send(client_sockfd, &ai_question, sizeof(ai_question), 0);
//                     //         if(strcmp(prompt, "exit") == 0){
//                     //             break;
//                     //         }
//                     //         char answer[MAX_QUERY_LEN];
//                     //         memset(answer, 0, sizeof(answer));
//                     //         recv(client_sockfd, answer, sizeof(answer), 0);
//                     //         answer[strlen(answer)] = '\0';
//                     //         printf("%s\n", answer);                  
//                     //     }
//                     //     memset(&ai_question, 0, sizeof(ai_question));
//                     //     // free(&ai_question);
//                     // } else if(ai_assistant == 'n'){
//                     //     //Do nothing
//                     // } else {
//                     //     printf("Invalid option!\n");
//                     //     goto ask_ai;
//                     // }
//                     // printf("abc\n");
//             } else if(see_detail == 'n'){
//                 //Do nothing
//                 } else {
//                     printf("Invalid option!\n");
//                     goto detail_result;
//                 }
//             printf("\n");
//     }
//     else {
//         printf("Invalid option!\n");
//         take_exam(client_sockfd, exam);
//         return;
//     }
//     memset(&exam, 0, sizeof(exam));
//     showExamList(client_sockfd);
//     return;
// }
void take_exam(int client_sockfd, exam_data* exam) {
    printExamInfo(exam);
    printf("Do you want to take this exam? (y/n): ");
    char option;
    scanf(" %1c", &option);
    __fpurge(stdin);

    if (option == 'n') {
        showExamList(client_sockfd);
        return;
    } else if (option == 'y') {
        printf("\n\n##########################################\n\n");
        printf("Exam: %s\n\n", exam->title);

        request_question_list request_question_list;
        request_question_list.opcode = 204;
        request_question_list.user_id = data.user_id;
        request_question_list.exam_id = exam->exam_id;
        request_question_list.number_of_question = exam->number_of_question;
        send(client_sockfd, &request_question_list, sizeof(request_question_list), 0);

        // Allocate memory for question list
        question_data **question_list = malloc(sizeof(question_data*));
        *question_list = malloc(sizeof(question_data) * exam->number_of_question);

        // Array to store user's answers
        char user_answers[exam->number_of_question];
        memset(user_answers, 0, sizeof(user_answers)); // Initialize all answers to 0 (unanswered)

        for (int i = 0; i < exam->number_of_question; i++) {
            recv(client_sockfd, (*(question_list) + i), sizeof(question_data), MSG_WAITALL);
            send(client_sockfd, "OK", sizeof("OK"), 0);
        }

        int current_question = 0;
        while (current_question < exam->number_of_question) {
            printf("Question %d: %s\n", current_question + 1, (*(question_list) + current_question)->content);
            for (int j = 0; j < 4; j++) {
                printf("%c. %s\n", j + 65, (*(question_list) + current_question)->op[j]);
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

                    if (strcasecmp(change_choice, "EXIT") == 0 || strcasecmp(change_choice, "exit") == 0) {
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
                            if (new_ans == 'A' || new_ans == 'B' || new_ans == 'C' || new_ans == 'D') {
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
        for (int i = 0; i < exam->number_of_question; i++) {
            final_answer[i] = (user_answers[i] == 0) ? ' ' : user_answers[i];
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
        printf("\n");
        // detail_result:
        // printf("Are you want to see detail result and corrections? (y/n): ");
        // char see_detail;
        // scanf(" %c", &see_detail);
        // __fpurge(stdin);
        // send(client_sockfd, &see_detail, 1, 0);
        // if (see_detail == 'y') {
        //     char correct_ans[exam->number_of_question + 1];
        //     recv(client_sockfd, correct_ans, exam->number_of_question, MSG_WAITALL);
        //     correct_ans[exam->number_of_question] = '\0';
        //     for (int i = 0; i < exam->number_of_question; i++) {
        //         printf("Question %d: %s\n", i + 1, (*(question_list) + i)->content);
        //         for (int j = 0; j < 4; j++) {
        //             printf("%c. %s\n", j + 65, (*(question_list) + i)->op[j]);
        //         }
        //         printf("Your answer: %c   -   Correct answer: %c\n", final_answer[i], correct_ans[i]);
        //     }
        //     // memset(final_answer, 0, sizeof(final_answer));
        //     memset(correct_ans, 0, sizeof(correct_ans));

            printf("\nDo you want to ask for explanation with AI Assistant? (y/n): ");
            char ai_assistant;
            while (1) {
                scanf(" %c", &ai_assistant);
                __fpurge(stdin);
                if (ai_assistant == 'y') {
                    printf("AI Assistant is ready to help you! Type 'exit' to quit.\n");
                    ask_ai_data ai_question;
                    memset(&ai_question, 0, sizeof(ai_question));
                    while (1) {
                        printf("Enter the question you want to ask: ");
                        char question_number[5];
                        char prompt[MAX_INPUT_LENGTH];
                        scanf(" %s", question_number);
                        __fpurge(stdin);
                        if (strcmp(question_number, "exit") == 0) {
                            strcpy(prompt, "exit");
                        } else {
                            int question_index = atoi(question_number);
                            strcpy(prompt, (*(question_list) + question_index - 1)->content);
                            for (int j = 0; j < 4; j++) {
                                strcat(prompt, "\t");
                                // strcat(prompt, char('A'+j));
                                // strcat(prompt, ". ");
                                strcat(prompt, (*(question_list) + question_index - 1)->op[j]);
                            }
                        }
                        ai_question.opcode = 998;
                        ai_question.user_id = data.user_id;
                        strcpy(ai_question.question, prompt);
                        send(client_sockfd, &ai_question, sizeof(ai_question), 0);
                        prompt[strcspn(prompt, "\n")] = '\0';
                        if (strcmp(prompt, "exit") == 0) {
                            break;
                        }
                        char answer[MAX_QUERY_LEN];
                        memset(answer, 0, sizeof(answer));
                        recv(client_sockfd, answer, sizeof(answer), 0);
                        answer[strlen(answer)] = '\0';
                        printf("%s\n", answer);
                    }
                    memset(&ai_question, 0, sizeof(ai_question));
                    break;
                } else if (ai_assistant == 'n') {
                    break;
                } else {
                    printf("Invalid option!\n");
                }
            }
            showExamList(client_sockfd);
            return;
            // fflush(stdin);
    } else {
        printf("Invalid option!\n");
        take_exam(client_sockfd, exam);
        return;
    }
    // memset(&exam, 0, sizeof(exam));
    // showExamList(client_sockfd);
    // return;
}


void seeExamHistrory(int client_socket){
    request_user_history request;
    request.opcode = 207;
    request.user_id = data.user_id;
    strcpy(request.username, data.username);
    send(client_socket, &request, sizeof(request), 0);
    int number_of_history_records;
    char oke_signal[OKE_SIGNAL_LEN] = "OK";
    recv(client_socket, &number_of_history_records, sizeof(int), 0);
    send(client_socket, oke_signal, sizeof(oke_signal), 0);
    exam_result **history_list = malloc(sizeof(exam_result*));
    *history_list = malloc(sizeof(exam_result)*number_of_history_records);
    for(int i=0; i<number_of_history_records; i++){
        recv(client_socket, (*history_list)+i, sizeof(exam_result), 0);
        send(client_socket, oke_signal, sizeof(oke_signal), 0);
    }

    printf("EXAM HISTORY OF %s\n\n", data.username);
    for(int i=0; i<number_of_history_records; i++){
        printf("%d. ", i+1);
        printf("Test: %s\tDate: %s\n", (*history_list)[i].title, (*history_list)[i].date);
        printf("Score: %s\n\n", (*history_list)[i].score);
    }
    //free(history_list);
    UIMainAppMenu(client_socket);
}

void seeUserInfo(int client_sockfd){
    request_user_info info;
    info.opcode = 103;
    info.user_id = data.user_id;
    strcpy(info.username, data.username);
    send(client_sockfd, &info, sizeof(info), 0);
    int check;
    recv(client_sockfd, &check, sizeof(int), 0);
    send(client_sockfd, "OK", sizeof("OK"), 0);
    register_data user_info;
    recv(client_sockfd, &user_info, sizeof(user_info), 0);
    //Print user info
    printUserInfo(user_info);
    UIMainAppMenu(client_sockfd);
}

void request_admin(int client_sockfd){
    printf("Request admin previlege!\n");
    request_admin_privilege request;
    request.opcode = 301;
    request.user_id = data.user_id;
    strcpy(request.username, data.username);
    send(client_sockfd, &request, sizeof(request), 0);
    char result[16];
    recv(client_sockfd, result, sizeof(result), 0);
    if(strcmp(result, "OK") == 0){
        printf("Your request has been sent!\nPlease wait for admin to approve!\n");
    }
    else if(strcmp(result, "DUPLICATED") == 0){
        printf("You have sent the request before!\nPlease wait for approval!\n");
    }
    else{
        printf("Request failed!\n");
    }
    UIMainAppMenu(client_sockfd);
}

//##### Advanced features #####
//!TODO: Implement exam with images
void takeImageEmbeddedExam(int sockfd){
    image_exam request;
    request.opcode = 1001;
    request.user_id = data.user_id;
    printf("Enter number of questions: ");
    scanf(" %d", &request.number_of_question);
    __fpurge(stdin);
    //Send request to server
    send(sockfd, &request, sizeof(request), 0);
    int number_of_question_available;
    recv(sockfd, &number_of_question_available, sizeof(number_of_question_available), 0);
    for(int i = 0; i < number_of_question_available; i++){
        send(sockfd, "OK", sizeof("OK"), 0);
        char image_name[255];
        recv(sockfd, image_name, sizeof(image_name), 0);
        if(strcmp(image_name, "ERROR") == 0){
            printf("Cannot get image name!\n");
            continue;
        }
        char image_full_path[1000];
        sprintf(image_full_path, "/home/phcogdai/NP_20241/main_reconstructx2/client/source/image/%s", image_name);
        printf("Image path: %s\n", image_full_path);
        //Open image file for writing
        FILE *image_file = fopen(image_full_path, "wb");
        if(image_file == NULL){
            printf("Cannot open image file!\n");
            send(sockfd, "ERROR", sizeof("ERROR"), 0);
            return;
        }
            send(sockfd, "OK", sizeof("OK"), 0);
        while(1){
            char buffer[2048];
            int read_size = recv(sockfd, buffer, sizeof(buffer), 0);
            if(strcmp(buffer, "END") == 0 || read_size <= 0){
                break;
            }
            fwrite(buffer, read_size, 1, image_file);
            send(sockfd, "OK", sizeof("OK"), 0);
            memset(buffer, 0, sizeof(buffer));
        }
    //Close image file
    if(image_file != NULL) fclose(image_file);
    }
    //Get question list
    recv(sockfd, &number_of_question_available, sizeof(number_of_question_available), 0);
    question_image **question_list = malloc(sizeof(question_image*));
    *question_list = malloc(sizeof(question_image)*number_of_question_available);
    for(int i = 0; i < number_of_question_available; i++){
        send(sockfd, "OK", sizeof("OK"), 0);
        recv(sockfd, (*question_list)+i, sizeof(question_image), 0);
    }
    //Print question list
    for(int i=0; i<number_of_question_available; i++){
    printf("Question %d: %s\n", i+1, (*(question_list)+i)->question_content);
        for(int j=0; j<4; j++){
            if(strlen((*(question_list)+i)->op[j]) > 0)
                printf("%c. %s\n", j+65, (*(question_list)+i)->op[j]);
        }
    }
    //Open image viewer
    getInput(question_list, number_of_question_available, 0);
    startImageQuestionInterface();
    //Show option
    int opt;
    do  {
    //Print question list
    for(int i=0; i<number_of_question_available; i++){
    printf("Question %d: %s\n", i+1, (*(question_list)+i)->question_content);
        for(int j=0; j<4; j++){
            if(strlen((*(question_list)+i)->op[j]) > 0)
                printf("%c. %s\n", j+65, (*(question_list)+i)->op[j]);
        }
    }
    printf("\n 1. Reopen image viewer\n");
    printf(" 2. Submit answer\n");
    printf("Please choose your option: ");
    scanf(" %d", &opt);
    __fpurge(stdin);
    char answer[number_of_question_available+1];
    switch(opt){
        case 1:
            if(viewerOpen == 0){
                getInput(question_list, number_of_question_available, 0);
                startImageQuestionInterface();
                printf("Image viewer has been reopened!\n");
            } 
            break;
        case 2:
            printf("Enter your answer: ");
            scanf(" %s", answer);
            processAnswer(answer);
            printf("Your answer: %s\n", answer);
            printf("Do you want to submit the answer? (y/n)\n");
            char option;
            scanf(" %1c", &option);
            if(option == 'y'){
                send(sockfd, answer, sizeof(answer), 0); //Send the answer to server
                //Get result
                int result;
                recv(sockfd, &result, sizeof(result), 0);
                printf("\nYour score: %d/%d\n", result, number_of_question_available);
                //destroyWindow();
                //Delete image files
                deleteImageAfterUsed();
                showAdvancedFeaturesMenu(sockfd);
                return;
            }else{
                printf("Your answer has not been submitted!\n");
                break;
            }
            break;
        default:
            printf("Invalid option!\n");
            break;
        }
    } while(1);
}

//!TODO: Implement exam with sounds
void takeSoundEmbeddedExam(int sockfd){
    sound_exam request;
    request.opcode = 1002;
    request.user_id = data.user_id;
    printf("Enter number of questions: ");
    scanf(" %d", &request.number_of_question);
    __fpurge(stdin);
    //Send request to server
    send(sockfd, &request, sizeof(request), 0);
    int number_of_question_available;
    recv(sockfd, &number_of_question_available, sizeof(number_of_question_available), 0);
    for(int i = 0; i < number_of_question_available; i++){
        send(sockfd, "OK", sizeof("OK"), 0);
        char audio_name[255];
        recv(sockfd, audio_name, sizeof(audio_name), 0);
        if(strcmp(audio_name, "ERROR") == 0){
            printf("Cannot get audio file name!\n");
            continue;
        }
        char audio_full_path[1000];
        sprintf(audio_full_path, "/home/phcogdai/NP_20241/main_reconstructx2/client/source/soundtrack/%s", audio_name);
        printf("Audio path: %s\n", audio_full_path);
        //Open audio file for writing
        FILE *audio_file = fopen(audio_full_path, "wb");
        if(audio_file == NULL){
            printf("Cannot open audio file!\n");
            send(sockfd, "ERROR", sizeof("ERROR"), 0);
            return;
        }
            send(sockfd, "OK", sizeof("OK"), 0);
        while(1){
            char buffer[2048];
            int read_size = recv(sockfd, buffer, sizeof(buffer), 0);
            if(strcmp(buffer, "END") == 0 || read_size <= 0){
                break;
            }
            fwrite(buffer, read_size, 1, audio_file);
            send(sockfd, "OK", sizeof("OK"), 0);
            memset(buffer, 0, sizeof(buffer));
        }
    //Close image file
    if(audio_file != NULL) fclose(audio_file);
    }
    //Get question list
    recv(sockfd, &number_of_question_available, sizeof(number_of_question_available), 0);
    question_image **question_list = malloc(sizeof(question_image*));
    *question_list = malloc(sizeof(question_image)*number_of_question_available);
    for(int i = 0; i < number_of_question_available; i++){
        send(sockfd, "OK", sizeof("OK"), 0);
        recv(sockfd, (*question_list)+i, sizeof(question_image), 0);
    }
    //Print question list
    for(int i=0; i<number_of_question_available; i++){
    printf("Question %d: %s\n", i+1, (*(question_list)+i)->question_content);
        for(int j=0; j<4; j++){
            if(strlen((*(question_list)+i)->op[j]) > 0)
                printf("%c. %s\n", j+65, (*(question_list)+i)->op[j]);
        }
    }
    //Open audio listener
    getInput(question_list, number_of_question_available, 1);
    startSoundQuestionInterface();
    //Show option
    int opt;
    do{
    //Print question list
    for(int i=0; i<number_of_question_available; i++){
    printf("Question %d: %s\n", i+1, (*(question_list)+i)->question_content);
        for(int j=0; j<4; j++){
            if(strlen((*(question_list)+i)->op[j]) > 0)
                printf("%c. %s\n", j+65, (*(question_list)+i)->op[j]);
        }
    }
    printf("\n 1. Reopen audio listener\n");
    printf(" 2. Submit answer\n");
    printf("Please choose your option: ");
    scanf(" %d", &opt);
    __fpurge(stdin);
    char answer[number_of_question_available+1];
    switch(opt){
        case 1:
            if(viewerOpen == 0){
                getInput(question_list, number_of_question_available, 1);
                startSoundQuestionInterface();
                printf("Image viewer has been reopened!\n");
            } 
            break;
        case 2:
            printf("Enter your answer: ");
            scanf(" %s", answer);
            processAnswer(answer);
            printf("Your answer: %s\n", answer);
            printf("Do you want to submit the answer? (y/n)\n");
            char option;
            scanf(" %1c", &option);
            if(option == 'y'){
                send(sockfd, answer, sizeof(answer), 0); //Send the answer to server
                //Get result
                int result;
                recv(sockfd, &result, sizeof(result), 0);
                printf("\nYour score: %d/%d\n", result, number_of_question_available);
                //destroyWindow();
                //Delete audio file
                deleteSoundAfterUsed();
                showAdvancedFeaturesMenu(sockfd);
                return;
            }else{
                printf("Your answer has not been submitted!\n");
                break;
            }
            break;
        default:
            printf("Invalid option!\n");
            break;
        }
    }while(1);
}

//!TODO: Implement see study statistics
void seeStudyStatistic(int sockfd){
    
}

void chat(int sockfd) {
    int opcode = 999;
    send(sockfd, &opcode, 4, 0);
    printf("Assistant is ready! Type 'exit' to quit.\n");
    char prompt[MAX_INPUT_LENGTH];
    char buffer[MAX_QUERY_LEN];
    while(1) {
        memset(prompt, 0, MAX_INPUT_LENGTH);
        memset(buffer, 0, MAX_QUERY_LEN);
        printf("You: ");
        fgets(prompt, sizeof(prompt), stdin);
        prompt[strcspn(prompt, "\n")] = '\0';
        send(sockfd, prompt, strlen(prompt), 0);
        if(strcmp(prompt, "exit")==0) {
            break;
        }
        recv(sockfd, buffer, MAX_QUERY_LEN, 0);
        buffer[strlen(buffer)] = '\0';
        printf("%s\n", buffer);
    }
    printf("\n");
    return;
}

//##### End of advanced features #####


void showAdvancedFeaturesMenu(int sockfd){
    printf("Advanced features!\n");
    printf("1. Exam with images\n");
    printf("2. Exam with sounds\n");
    printf("3. See study statistics\n");
    printf("4. Back to main menu\n");
    printf("Please choose your option: ");
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
    switch(option){
        case 1:
            takeImageEmbeddedExam(sockfd);
            return;
        case 2:
            takeSoundEmbeddedExam(sockfd);
            return;
        case 3:
            seeStudyStatistic(sockfd);
            return;
        case 4:
            UIMainAppMenu(sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
    }
    showAdvancedFeaturesMenu(sockfd);
}

void UIMainAppMenu(int client_sockfd){
    printf("Main application system!\n");
    printf("1. Take a practice exam\n");
    printf("2. See examination history\n");
    printf("3. See user information\n");
    printf("4. Request admin previlege\n");
    printf("5. Advanced features\n");
    printf("6. Chat with AI\n");
    printf("7. Log out\n");
    printf("Please choose your option: ");
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
        switch(option){
        case 1:
            showExamList(client_sockfd);
            return;
        case 2:
            seeExamHistrory(client_sockfd);
            return;
        case 3:
            seeUserInfo(client_sockfd);
            return;
        case 4:
            request_admin(client_sockfd);
            return;
        case 5:
            showAdvancedFeaturesMenu(client_sockfd);
            return;
        case 6:
            chat(client_sockfd);
            break;
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
            logout_data logout_sig;
            memset(&logout_sig, 0, sizeof(logout_sig));
            strcpy(logout_sig.username, data.username);
            logout_sig.opcode = 111;
            send(client_sockfd, &logout_sig, sizeof(logout_sig), 0);
            UIHomePage(client_sockfd);
            return;
        default:
            printf("Invalid option!\n");
            break;
        }
    UIMainAppMenu(client_sockfd);
}

void printExamList(exam_data ** exams, int number_of_exam){
    printf("\nExam list:\n");
    for(int i = 0; i < number_of_exam; i++){
        printf("%d. %s\t%d questions\n", i+1, (*(exams)+i)->title, (*(exams)+i)->number_of_question);
    }
}

// void showExamList(int client_sockfd){
//     int number_of_exam=10;
//     exam_data** exams = getExamList(client_sockfd, number_of_exam);
//     printExamList(exams, number_of_exam);
//     printf("\n##############################\n");
//     while(1) {
//         printf("Please choose your option(0 for back to main menu): ");
//         int option;
//         scanf(" %d", &option);
//         __fpurge(stdin);
//         if(option == 0) {
//             UIMainAppMenu(client_sockfd);
//             return;
//         } else if(option < 0 || option > number_of_exam){
//             printf("Invalid option!\n");
//             continue;
//         }
//         exam_data *exam = (*(exams)+option-1);
//         take_exam(client_sockfd, exam);
//     }
// }

void showExamList(int client_sockfd){
    int number_of_exam = 10;
    exam_data** exams = getExamList(client_sockfd, number_of_exam) ;
    printExamList(exams, number_of_exam);
    printf("\n##############################\n");
    printf("Please choose your option(0 for back to main menu): ");
    int option;
    scanf(" %d", &option);
    __fpurge(stdin);
    if(option == 0){
        UIMainAppMenu(client_sockfd);
        return;
    }else if(option < 0 || option > number_of_exam){
        printf("Invalid option!\n");
        showExamList(client_sockfd);
        return;
    }
    take_exam(client_sockfd, (*(exams)+option-1));
}

void processAnswer(char *str)
{
    // To keep track of non-space character count
    int count = 0;
    // Traverse the provided string. If the current character is not a space,
    //move it to index 'count++'.
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is incremented
    str[count] = '\0';
    for(int i = 0; i < strlen(str); i++){
        if( str[i] >= 'a' && str[i] <= 'z'){
            str[i] = str[i] + 'A' - 'a';
        }
    }
}
