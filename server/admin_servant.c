#include "../data.h"
#include "server.h"
#define MAX_QUERY_LEN 3000


//!####### ADMIN FUNCTIONS #######
// Trong admin_servant.c

// Cuong
void getAllSubjects(int socket, char *buffer){
    char query[MAX_QUERY_LEN];
    sprintf(query, 
      "SELECT DISTINCT question_subject "
      "FROM Question "
      "WHERE question_subject IS NOT NULL AND question_subject != '';"
    );

    MYSQL_RES *res = make_query(query);
    if(res == NULL){
        int zero = 0;
        send(socket, &zero, sizeof(zero), 0);
        return;
    }
    int rowCount = mysql_num_rows(res);

    send(socket, &rowCount, sizeof(rowCount), 0);

    if(rowCount <= 0){
        mysql_free_result(res);
        return;
    }

    MYSQL_ROW row;
    int i = 1;
    while( (row = mysql_fetch_row(res)) ){
        char subject[150];
        if(row[0]) {
          //sprintf(subject, "%d. %s", i, row[0]);
          strcpy(subject, row[0]);
        }
        else strcpy(subject, "");
        // Gửi subject qua socket
        send(socket, subject, sizeof(subject), 0);
        i++;
    }
    mysql_free_result(res);
}
// Cuong
void getCountOfChosenSubject(int socket, char *buffer){
    create_random_exam *request = (create_random_exam*)buffer;
    char *subject = request->subject;
    char query[MAX_QUERY_LEN];
    level_count level;
    level.opcode = 609;
    // Count easy questions
    sprintf(query, "SELECT COUNT(*) FROM Question WHERE question_subject = '%s' AND level = 'easy';", subject);
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row = mysql_fetch_row(res);
    level.easy_count = atoi(row[0]);
    mysql_free_result(res);

    // Count normal questions
    sprintf(query, "SELECT COUNT(*) FROM Question WHERE question_subject = '%s' AND level = 'normal';", subject);
    res = make_query(query);
    row = mysql_fetch_row(res);
    level.normal_count = atoi(row[0]);
    mysql_free_result(res);

    // Count hard questions
    sprintf(query, "SELECT COUNT(*) FROM Question WHERE question_subject = '%s' AND level = 'hard';", subject);
    res = make_query(query);
    row = mysql_fetch_row(res);
    level.hard_count = atoi(row[0]);
    mysql_free_result(res);

    // Send the counts back to the client
    send(socket, &level, sizeof(level), 0);
}




//@TODO: Search Operation
void searchQuestionById(int socket, char *buffer){
    //Convert buffer to search_question_by_id
    search_question_by_id *request = (search_question_by_id*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d search question by id\n", user_id);
    char *question_id = request->question_id;
    printf("Question id: %s\n", question_id);
    //Get number of question
    char query[MAX_QUERY_LEN];
    char select[] = "SELECT count(*) FROM Question where question_id like '";
    sprintf(query, "%s%c%s%c';", select, '%', question_id, '%');
    printf("Query: %s\n", query);
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    int number_of_question = atoi(row[0]);
    printf("Number of question: %d\n", number_of_question);
    //Send number of question to client
    if(request->next_page == 0){
        int n = number_of_question;
        send(socket, &n, sizeof(n), 0);
    }
    //No question found
    if(number_of_question == 0){
        mysql_free_result(res);
        return;
    } 
    //Get question from database
    recv(socket, &number_of_question, sizeof(number_of_question), 0); //receive begin index
    memset(query, 0, sizeof(query));
    char select1[] = "SELECT * FROM Question where question_id like '";
    sprintf(query, "%s%c%s%c' limit %d, 10;", select1, '%', question_id, '%', number_of_question);
    printf("Query: %s\n", query);
    mysql_free_result(res);
    res = make_query(query);
    int i = 0;
    question_data **question_list;
    question_list = malloc(sizeof(question_data*));
    *question_list = malloc(sizeof(question_data)*10);
    while((row = mysql_fetch_row(res)) != NULL && i < 10){
        (*question_list)[i].opcode = 202;
        strcpy((*question_list)[i].question_id, row[0]);
        strcpy((*question_list)[i].content, row[1]);
        strcpy((*question_list)[i].op[0], row[2]);
        strcpy((*question_list)[i].op[1], row[3]);
        strcpy((*question_list)[i].op[2], row[4]);
        strcpy((*question_list)[i].op[3], row[5]);
        i++;
    }
    //check question_list
    for(int j = 0; j < i; j++){
        printf("Question %d: %s\n", j+1, (*question_list)[j].question_id);
        printf("Content: %s\n", (*question_list)[j].content);
        printf("A: %s\n", (*question_list)[j].op[0]);
        printf("B: %s\n", (*question_list)[j].op[1]);
        printf("C: %s\n", (*question_list)[j].op[2]);
        printf("D: %s\n", (*question_list)[j].op[3]);
    }
    //Send question list to client
    char rcv[10];
    send(socket, &i, sizeof(i), 0);
    printf("Send number of question: %d\n", i);
    for(int j = 0; j < i; j++){
        recv(socket, rcv, sizeof(rcv), 0);
        if(strcmp(rcv, "OK") == 0){
            send(socket, ((*question_list)+j), sizeof(question_data), 0);
            continue;
        }else{
            //!TODO: send error signal
        }
    }
    //Free memory
    mysql_free_result(res);
}

void searchQuestionByContent(int socket, char *buffer){
    //Convert buffer to search_question_by_content
    search_question_by_content *request = (search_question_by_content*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d search question by question's content\n", user_id);
    char *content = request->question_content;
    //Get number of question
    char query[MAX_QUERY_LEN];
    char select[] = "SELECT count(*) FROM Question where question_content like '";
    sprintf(query, "%s%c%s%c';", select, '%', content, '%');
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    int number_of_question = atoi(row[0]);
    printf("Number of question: %d\n", number_of_question);
    //Send number of question to client
    if(request->next_page == 0){
        int n = number_of_question;
        send(socket, &n, sizeof(n), 0);
    }
    //No question found
    if(number_of_question == 0){
        mysql_free_result(res);
        return;
    } 
    //Get question from database
    recv(socket, &number_of_question, sizeof(number_of_question), 0); //receive begin index
    memset(query, 0, sizeof(query));
    char select1[] = "SELECT * FROM Question where question_content like '";
    sprintf(query, "%s%c%s%c' limit %d, 10;", select1, '%', content, '%', number_of_question);
    printf("Query: %s\n", query);
    mysql_free_result(res);   
    res = make_query(query);
    int i = 0;
    question_data **question_list;
    question_list = malloc(sizeof(question_data*));
    *question_list = malloc(sizeof(question_data)*10);
    while((row = mysql_fetch_row(res)) != NULL && i < 10){
        (*question_list)[i].opcode = 202;
        strcpy((*question_list)[i].question_id, row[0]);
        strcpy((*question_list)[i].content, row[1]);
        strcpy((*question_list)[i].op[0], row[2]);
        strcpy((*question_list)[i].op[1], row[3]);
        strcpy((*question_list)[i].op[2], row[4]);
        strcpy((*question_list)[i].op[3], row[5]);
        i++;
    }
    //check question_list
    for(int j = 0; j < i; j++){
        printf("Question %d: %s\n", j+1, (*question_list)[j].question_id);
        printf("Content: %s\n", (*question_list)[j].content);
        printf("A: %s\n", (*question_list)[j].op[0]);
        printf("B: %s\n", (*question_list)[j].op[1]);
        printf("C: %s\n", (*question_list)[j].op[2]);
        printf("D: %s\n", (*question_list)[j].op[3]);
    }
    //Send question list to client
    char rcv[10];
    send(socket, &i, sizeof(i), 0);
    for(int j = 0; j < i; j++){
        recv(socket, rcv, sizeof(rcv), 0);
        if(strcmp(rcv, "OK") == 0){
            send(socket, ((*question_list)+j), sizeof(question_data), 0);
            continue;
        }
    }
    //Free memory
    mysql_free_result(res);
}

void advanceQuestionSearch(int socket, char *buffer){
    //Convert buffer to advance_question_search
    advance_search_question *request = (advance_search_question*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d advance search question\n", user_id);
    char *question_id = request->question_id;
    char *content = request->question_content;
    char *subject = request->subject;
    char *topic = request->topic;
    char *level       = request->level;
    //Get number of question
    char query[MAX_QUERY_LEN];
    char select[] = "SELECT count(*) FROM Question where question_id like '";
    sprintf(query, "%s%c%s%c' and question_content like '%c%s%c' and subject like '%c%s%c' and topic like '%c%s%c';", select, '%', question_id, '%', '%', content, '%', '%', subject, '%', '%', topic, '%');
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    int number_of_question = atoi(row[0]);
    printf("Number of question: %d\n", number_of_question);
    //Send number of question to client
    if(request->next_page == 0){
        int n = number_of_question;
        send(socket, &n, sizeof(n), 0);
    }
    //No question found
    if(number_of_question == 0){
        mysql_free_result(res);
        return;
    } 
    //Get question from database
    recv(socket, &number_of_question, sizeof(number_of_question), 0); //receive begin index
    memset(query, 0, sizeof(query));
    char select1[] = "SELECT * FROM Question where question_id like '";
    sprintf(query, "%s%c%s%c' and question_content like '%c%s%c' and subject like '%c%s%c' and topic like '%c%s%c' limit %d, 10;", select1, '%', question_id, '%', '%', content, '%', '%', subject, '%', '%', topic, '%', number_of_question);
    mysql_free_result(res);
    res = make_query(query);
    int i = 0;
    question_data **question_list;
    question_list = malloc(sizeof(question_data*));
    *question_list = malloc(sizeof(question_data)*10);
    while((row = mysql_fetch_row(res)) && i < 10){
        (*question_list)[i].opcode = 202;
        strcpy((*question_list)[i].question_id, row[0]);
        strcpy((*question_list)[i].content, row[1]);
        strcpy((*question_list)[i].op[0], row[2]);
        strcpy((*question_list)[i].op[1], row[3]);
        strcpy((*question_list)[i].op[2], row[4]);
        strcpy((*question_list)[i].op[3], row[5]);
        i++;
    }
    //check question_list
    for(int j = 0; j < i; j++){
        printf("Question %d: %s\n", j+1, (*question_list)[j].question_id);
        printf("Content: %s\n", (*question_list)[j].content);
        printf("A: %s\n", (*question_list)[j].op[0]);
        printf("B: %s\n", (*question_list)[j].op[1]);
        printf("C: %s\n", (*question_list)[j].op[2]);
        printf("D: %s\n", (*question_list)[j].op[3]);
    }
    //Send question list to client
    send(socket, &i, sizeof(i), 0);
    char rcv[10];
    for(int j = 0; j < i; j++){
        recv(socket, rcv, sizeof(rcv), 0);
        if(strcmp(rcv, "OK") == 0){
            send(socket, ((*question_list)+j), sizeof(question_data), 0);
            continue;
        }
    }
    //Free memory
    mysql_free_result(res);
}

void addNewQuestion(int socket, char *buffer){
    //Convert buffer to add_new_question
    add_question *request = (add_question*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d add new question\n", user_id);
    char *question_id = request->question_id;
    char *content = request->question_content;
    char *op[4];
    for(int i = 0; i < 4; i++){
        op[i] = request->op[i];
    }
    int cop = request->ans;
    char *subject = request->subject;
    char *topic = request->topic;
    char *level       = request->level;
    //Insert new question to database
    char query[MAX_QUERY_LEN];
    sprintf(query, 
       "INSERT INTO Question(question_id, question_content, opa, opb, opc, opd, question_subject, question_topic, level) "
       "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');",
       question_id, content, op[0], op[1], op[2], op[3], subject, topic, level
    );
    MYSQL_RES *res = make_query(query);
    memset(query, 0, sizeof(query));
    sprintf(query, "INSERT INTO Answer(question_id, ans) VALUES ('%s', %d);", question_id, cop);
    //Free memory
    mysql_free_result(res);
    res = make_query(query);
    //Send add new question success signal to client
    int oke_signal = 1;
    send(socket, &oke_signal, sizeof(oke_signal), 0);
    //Free memory
    mysql_free_result(res);
}

void deleteQuestion(int socket, char *buffer){
    //Convert buffer to delete_question
    delete_question *request = (delete_question*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d delete question by id\n", user_id);
    char *question_id = request->question_id;
    //Delete question from database
    char query[MAX_QUERY_LEN];
    sprintf(query, "DELETE FROM Question WHERE question_id = '%s';", question_id);
    MYSQL_RES *res = make_query(query);
    //Send delete question success signal to client
    int oke_signal = 1;
    send(socket, &oke_signal, sizeof(oke_signal), 0);
    //Free memory
    mysql_free_result(res);
}

//!TODO: Exam Operation
void searchExamById(int socket, char *buffer){
    //Convert buffer to search_exam_by_id
    search_exam_by_id *request = (search_exam_by_id*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d search exam by id\n", user_id);
    int exam_id = request->exam_id;
    //Get number of exam
    char query[MAX_QUERY_LEN];
    char select[] = "SELECT count(*) FROM Test where test_id = ";
    sprintf(query, "%s %d;", select, exam_id);
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    int number_of_exam = atoi(row[0]);
    printf("Number of exam: %d\n", number_of_exam);
    //Send number of exam to client
    int n = number_of_exam;
    send(socket, &n, sizeof(n), 0);
    if(number_of_exam == 0){
        mysql_free_result(res);
        return;
    }
    //Get exam from database
    memset(query, 0, sizeof(query));
    char select1[] = "SELECT * FROM Test where test_id = ";
    sprintf(query, "%s %d;", select1, exam_id);
    mysql_free_result(res);
    res = make_query(query);
    exam_data exam_list;
    row = mysql_fetch_row(res);
    (exam_list).opcode = 201;
    (exam_list).exam_id = atoi(row[0]);
    strcpy((exam_list).title, row[1]);
    (exam_list).number_of_question = atoi(row[2]);
    if(row[3] != NULL) (exam_list).admin_id = atoi(row[3]);
    else (exam_list).admin_id = 0;
    //check exam_list
    printf("Exam : %s\n", (exam_list).title);
    
    //Send exam list to client
    char rcv[10];
    send(socket, &(exam_list), sizeof(exam_data), 0);
    recv(socket, rcv, sizeof(rcv), 0);
    //Free memory
    mysql_free_result(res);
    if(strcmp(rcv, "OK") == 0) return;
    else {
        //!TODO: Process ERROR
    }
}

// Cuong
void createRandomExam(int socket, char *buffer){
    //Convert buffer to create_random_exam
    create_random_exam *request = (create_random_exam*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d create exam with random question\n", user_id);
    char *title = request->title;
    int number_of_question = request->number_of_question;
    int admin_id = request->admin_id;
    char *subject = request->subject;
    int easy_count = request->easy_count;
    int normal_count = request->normal_count;
    int hard_count = request->hard_count;
    //Create new exam
    char query[MAX_QUERY_LEN];
    sprintf(query, "INSERT INTO Test(title, num_of_question, admin_id) VALUES ('%s', %d, %d);", title, number_of_question, admin_id);
    MYSQL_RES *res = make_query(query);
    //Get test_id
    memset(query, 0, sizeof(query));
    char select[] = "SELECT test_id FROM Test where title = '";
    sprintf(query, "%s%s';", select, title);
    //Free memory
    mysql_free_result(res);
    res = make_query(query);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    int test_id = atoi(row[0]);
    printf("Test id: %d\n", test_id);
    //Get question list
    mysql_free_result(res);

    char question_ids[number_of_question][255];
    int i = 0;
    // Select easy questions
    if (easy_count > 0) {
        sprintf(query, "SELECT question_id FROM Question WHERE question_subject = '%s' AND level = 'easy' ORDER BY RAND() LIMIT %d;", subject, easy_count);
        res = make_query(query);
        while ((row = mysql_fetch_row(res))) {
            strcpy(question_ids[i], row[0]);
            i++;
        }
        mysql_free_result(res);
    }

    // Select normal questions
    if (normal_count > 0) {
        sprintf(query, "SELECT question_id FROM Question WHERE question_subject = '%s' AND level = 'normal' ORDER BY RAND() LIMIT %d;", subject, normal_count);
        res = make_query(query);
        while ((row = mysql_fetch_row(res))) {
            strcpy(question_ids[i], row[0]);
            i++;
        }
        mysql_free_result(res);
    }

    // Select hard questions
    if (hard_count > 0) {
        sprintf(query, "SELECT question_id FROM Question WHERE question_subject = '%s' AND level = 'hard' ORDER BY RAND() LIMIT %d;", subject, hard_count);
        res = make_query(query);
        while ((row = mysql_fetch_row(res))) {
            strcpy(question_ids[i], row[0]);
            i++;
        }
        mysql_free_result(res);
    }

    //Insert question list to Test_question
    for(int j = 0; j < number_of_question; j++){
        memset(query, 0, sizeof(query));
        sprintf(query, "INSERT INTO Test_question(test_id, question_id) VALUES (%d, '%s');", test_id, question_ids[j]);
        make_query(query);
    }
    //Send create exam success signal to client
    int oke_signal = 1;
    send(socket, &oke_signal, sizeof(oke_signal), 0);
}

// Cuong
void createExamByQuestion(int socket, char *buffer){
    //Convert buffer to create_exam_by_question
    create_exam *request = (create_exam*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d create exam\n", user_id);
    char *title = request->title;
    int number_of_question = request->number_of_ques;
    send(socket, &number_of_question, sizeof(number_of_question), 0);
    char question_ids[number_of_question][255];
    recv(socket, question_ids, sizeof(question_ids), 0);
    //Create new exam
    char query[MAX_QUERY_LEN];
    sprintf(query, "INSERT INTO Test(title, number_of_question) VALUES ('%s', %d);", title, number_of_question);
    MYSQL_RES *res = make_query(query);
    if(res == NULL){
        int reply = 0;
        send(socket, &reply, sizeof(reply), 0);
        return;
    }
    //Get test_id
    memset(query, 0, sizeof(query));
    char select[] = "SELECT test_id FROM Test where title = '";
    sprintf(query, "%s%s';", select, title);
    mysql_free_result(res);
    res = make_query(query);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    int test_id = atoi(row[0]);
    printf("Test id: %d\n", test_id);
    //Insert question list to Test_question
    mysql_free_result(res);
    for(int j = 0; j < number_of_question; j++){
        memset(query, 0, sizeof(query));
        sprintf(query, "INSERT INTO Test_question(test_id, question_id) VALUES (%d, '%s');", test_id, question_ids[j]);
        make_query(query);
    }
}
// Cuong
void deleteExam(int socket, char *buffer){
    //Convert buffer to delete_exam
    request_edit *request = (request_edit*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d delete exam by id\n", user_id);
    int exam_id = request->exam_id;
    //Delete exam from database
    char query[MAX_QUERY_LEN];
    sprintf(query, "DELETE FROM Test WHERE test_id = %d ;", exam_id);
    MYSQL_RES *res = make_query(query);
    //Send delete exam success signal to client
    int oke_signal = 1;
    send(socket, &oke_signal, sizeof(oke_signal), 0);
    //Free memory
    mysql_free_result(res);
}

//!TODO: Room Operation
void createRoom(int socket, char client_message[]){
    room_create_del *room = (room_create_del*)client_message;
    char query[MAX_QUERY_LEN];
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query, "SELECT * FROM Room WHERE r_name = '%s';", room->r_name);
    res = make_query(query);
    row = mysql_fetch_row(res);
    
    int oke_signal;
    if(row == NULL){
        sprintf(query, "INSERT INTO Room(r_name, admin_id, create_date) VALUES ('%s', (select user_id from User_info where username = '%s'), CURDATE());", room->r_name, room->username);
        make_query(query);
        oke_signal = 1;
    }
    else{
        oke_signal = -1;
    }
    mysql_free_result(res);
    send(socket, &oke_signal, sizeof(int), 0);
}

void deleteRoom(int socket, char client_message[]){
    room_create_del *room = (room_create_del*)client_message;
    char query[MAX_QUERY_LEN];
    MYSQL_RES *res;
    MYSQL_ROW row;
    int oke_signal;

    sprintf(query, "SELECT * FROM Room WHERE r_name = '%s';", room->r_name);
    res = make_query(query);
    row = mysql_fetch_row(res);
    

    if(row == NULL){
        oke_signal = -1;
        send(socket, &oke_signal, sizeof(int), 0);
        mysql_free_result(res);
        return;
    }
    mysql_free_result(res);
    sprintf(query, "SELECT count(Room.r_id) from Room, User_info\
                    WHERE User_info.username = '%s' AND Room.r_name = '%s'\
                    AND Room.admin_id = User_info.user_id; ", room->username, room->r_name);
    res = make_query(query);
    row = mysql_fetch_row(res);
    int num = atoi(row[0]);
    

    if(num == 0){
        oke_signal = -2;
        send(socket, &oke_signal, sizeof(int), 0);
        mysql_free_result(res);
        return;
    }
    mysql_free_result(res);
    
    oke_signal = 1;
    sprintf(query, "DELETE FROM Room WHERE r_name = '%s';", room->r_name);
    make_query(query);
    send(socket, &oke_signal, sizeof(int), 0);
}


void showRoomYouCreated(int socket, char client_message[]){
    // printf("Call here?\n");
    //opcode = 803
    show_your_room *your_room = (show_your_room*)client_message;
    // typedef struct{
    // int opcode; // 800 for stroring information of the room
    // int r_id;
    // char r_name[256];
    // char admin_name[256];
    // char create_date[16];
    // }room_info;

    char query[MAX_QUERY_LEN];
    int num_of_room;
    room_info room;
    sprintf(query, "SELECT COUNT(*) FROM Room, User_info WHERE User_info.username = '%s' AND Room.admin_id = User_info.user_id;", your_room->username);
    // printf("Before make query\n");
    MYSQL_RES *res = make_query(query);
    // printf("make query oke\n");
    MYSQL_ROW row = mysql_fetch_row(res);
    
    num_of_room = atoi(row[0]);
    mysql_free_result(res);

    
    // printf("server received yet?\n");
    send(socket, &num_of_room, sizeof(int), 0);
    // printf("Server send oke?\n");

    if(num_of_room == 0){
        return;
    }

    char oke_signal[OKE_SIGNAL_LEN];
    memset(oke_signal, 0, OKE_SIGNAL_LEN);

    recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);

    sprintf(query, "SELECT r.r_id, r.r_name, u.username, r.create_date FROM Room as r, User_info as u WHERE u.username = '%s' AND u.user_id = r.admin_id;", your_room->username);
    res = make_query(query);
    while((row = mysql_fetch_row(res)) != NULL){
        memset(&room, 0, sizeof(room_info));
        room.r_id = atoi(row[0]);
        strcpy(room.r_name, row[1]);
        strcpy(room.admin_name, row[2]);
        strcpy(room.create_date, row[3]);
        send(socket, &room, sizeof(room_info), 0);
        recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);
    }

    mysql_free_result(res);
    send(socket, oke_signal, OKE_SIGNAL_LEN, 0);

}




void showRoomYouWereAdded(int socket, char client_message[]){
    //opcode = 804
    //opcode = 803
    show_your_room *your_room = (show_your_room*)client_message;
    // typedef struct{
    // int opcode; // 800 for stroring information of the room
    // int r_id;
    // char r_name[256];
    // char admin_name[256];
    // char create_date[16];
    // }room_info;

    char query[MAX_QUERY_LEN];
    int num_of_room;
    room_info room;
    sprintf(query, "SELECT COUNT(*) FROM User_info, Room_user WHERE User_info.username = '%s' AND Room_user.user_id = User_info.user_id;", your_room->username);
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row = mysql_fetch_row(res);
    
    num_of_room = atoi(row[0]);
    mysql_free_result(res);

    send(socket, &num_of_room, sizeof(int), 0);

    
    if(num_of_room == 0){
        return;
    }

    char oke_signal[OKE_SIGNAL_LEN];
    memset(oke_signal, 0, OKE_SIGNAL_LEN);

    recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);

    

    sprintf(query, "SELECT r.r_id, r.r_name, u.username, r.create_date\
                    FROM Room as r, User_info as u\
                    WHERE r.admin_id = u.user_id AND r.r_id = (select r_id from Room_user, User_info\
                    where Room_user.user_id = User_info.user_id\
                    AND User_info.username = '%s');", your_room->username);
    
    res = make_query(query);
    while((row = mysql_fetch_row(res)) != NULL){
        memset(&room, 0, sizeof(room_info));
        room.r_id = atoi(row[0]);
        strcpy(room.r_name, row[1]);
        strcpy(room.admin_name, row[2]);
        strcpy(room.create_date, row[3]);
        send(socket, &room, sizeof(room_info), 0);
        recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);
    }
    mysql_free_result(res);


    send(socket, oke_signal, OKE_SIGNAL_LEN, 0);

    

}

void showAllYourRoom(int socket, char client_message[]){
    //opcode = 805
    show_your_room *your_room = (show_your_room*)client_message;
    // typedef struct{
    // int opcode; // 800 for stroring information of the room
    // int r_id;
    // char r_name[256];
    // char admin_name[256];
    // char create_date[16];
    // }room_info;

    char query[MAX_QUERY_LEN];
    int num_of_room;
    room_info room;
    sprintf(query, "SELECT COUNT(*) FROM Room, User_info WHERE User_info.username = '%s' AND Room.admin_id = User_info.user_id;", your_room->username);
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row = mysql_fetch_row(res);
    
    num_of_room = atoi(row[0]);
    mysql_free_result(res);

    sprintf(query, "SELECT COUNT(*) FROM User_info, Room_user WHERE User_info.username = '%s' AND Room_user.user_id = User_info.user_id;", your_room->username);
    res = make_query(query);
    row = mysql_fetch_row(res);
    
    num_of_room = num_of_room + atoi(row[0]);
    mysql_free_result(res);


    
    send(socket, &num_of_room, sizeof(int), 0);

    if(num_of_room == 0){
        return;
    }

    char oke_signal[OKE_SIGNAL_LEN];
    memset(oke_signal, 0, OKE_SIGNAL_LEN);

    recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);

    sprintf(query, "SELECT r.r_id, r.r_name, u.username, r.create_date FROM Room as r, User_info as u WHERE u.username = '%s' AND u.user_id = r.admin_id;", your_room->username);
    res = make_query(query);
    while((row = mysql_fetch_row(res)) != NULL){
        memset(&room, 0, sizeof(room_info));
        room.r_id = atoi(row[0]);
        strcpy(room.r_name, row[1]);
        strcpy(room.admin_name, row[2]);
        strcpy(room.create_date, row[3]);
        send(socket, &room, sizeof(room_info), 0);
        recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);
    }

    mysql_free_result(res);

    
    sprintf(query, "SELECT r.r_id, r.r_name, u.username, r.create_date\
                    FROM Room as r, User_info as u\
                    WHERE r.admin_id = u.user_id AND r.r_id = (select r_id from Room_user, User_info\
                    where Room_user.user_id = User_info.user_id\
                    AND User_info.username = '%s');", your_room->username);
    
    res = make_query(query);
    while((row = mysql_fetch_row(res)) != NULL){
        memset(&room, 0, sizeof(room_info));
        room.r_id = atoi(row[0]);
        strcpy(room.r_name, row[1]);
        strcpy(room.admin_name, row[2]);
        strcpy(room.create_date, row[3]);
        send(socket, &room, sizeof(room_info), 0);
        recv(socket, oke_signal, OKE_SIGNAL_LEN, 0);
    }
    mysql_free_result(res);


    
    send(socket, oke_signal, OKE_SIGNAL_LEN, 0);
}

void addUserToRoom(int socket, char client_message[]){
    //opcode = 811

    /*
    Các trường hợp có thể xảy ra:
        - user_id ko tồn tại
            -> flag = -2
        - user_id đã được add vào rồi
            -> flag = -3
        - user_id  trùng với admin_id (người tạo phòng) => cũng ko được add
            -> flag = -4
        - add user oke => 1
    */

    request_add_remove_student *request = (request_add_remove_student*)client_message;
    
    char query[MAX_QUERY_LEN];
    MYSQL_RES *res;
    MYSQL_ROW row;
    int flag;

    // user_id  ko tồn tại => -2
    {
        sprintf(query, "SELECT * FROM User_info WHERE user_id = '%d';", request->user_id);
        res = make_query(query);
        row = mysql_fetch_row(res);
        if(row == NULL){
            mysql_free_result(res);
            flag = -2;
            send(socket, &flag, sizeof(int), 0);
            return;
        }
        mysql_free_result(res);
    }

    // user_id  được add vảo rồi => -3
    {
        sprintf(query, "SELECT * from Room_user WHERE r_id = '%d' AND user_id = '%d';", request->r_id, request->user_id);
        res = make_query(query);
        row = mysql_fetch_row(res);
        if(row != NULL){
            mysql_free_result(res);
            flag = -3;
            send(socket, &flag, sizeof(int), 0);
            return;
        }
        mysql_free_result(res);
    }

    // user_id là người tạo ra phòng => -4
    {
        sprintf(query, "SELECT * from Room WHERE r_id = '%d' AND admin_id = '%d';", request->r_id, request->user_id);
        res = make_query(query);
        row = mysql_fetch_row(res);
        if(row != NULL){
            mysql_free_result(res);
            flag = -4;
            send(socket, &flag, sizeof(int), 0);
            return;
        }
        mysql_free_result(res);
    }


    //1 success: 
    {
        sprintf(query, "INSERT INTO Room_user VALUES (%d, %d);", request->r_id, request->user_id);
        make_query(query);
        flag = 1;
        send(socket, &flag, sizeof(int), 0);
    }
}

void deleteUserFromRoom(int socket, char client_message[]){
    //opcode = 812
    request_add_remove_student *request = (request_add_remove_student*)client_message;
    
    char query[MAX_QUERY_LEN];
    MYSQL_RES *res;
    MYSQL_ROW row;
    int flag;

    // user_id ko ở trong room -> -2
    {
        sprintf(query, "SELECT * from Room_user WHERE r_id = '%d' AND user_id = '%d';", request->r_id, request->user_id);
        res = make_query(query);
        row = mysql_fetch_row(res);
        if(row == NULL){
            mysql_free_result(res);
            flag = -2;
            send(socket, &flag, sizeof(int), 0);
            return;
        }
        mysql_free_result(res);
    }

    //1 success: 
    {
        sprintf(query, "DELETE FROM Room_user Where r_id = '%d' AND user_id = '%d';", request->r_id, request->user_id);
        make_query(query);
        flag = 1;
        send(socket, &flag, sizeof(int), 0);
    }
}



//!TODO: Admin request operation
void getAdminRequestInfo(int socket, char *buffer){
    //Convert buffer to approve_admin_request
    check_admin_previlege_request *request = (check_admin_previlege_request*)buffer;
    int user_id = request->user_id;
    printf("  ###Admin: %d - get admin request list\n", user_id);
    //Query database to get number of request
    char query[MAX_QUERY_LEN];
    char select[] = "SELECT count(*) FROM Request_admin;";
    MYSQL_RES *res = make_query(select);
    MYSQL_ROW row = mysql_fetch_row(res);
    int number_of_request = atoi(row[0]);
    printf("Number of request: %d\n", number_of_request);
    //Send number of request to client
    send(socket, &number_of_request, sizeof(number_of_request), 0);
    //No request found
    if(number_of_request == 0){
        mysql_free_result(res);
        return;
    }
    //Get request from database
    recv(socket, &number_of_request, sizeof(number_of_request), 0); //receive begin index
    memset(query, 0, sizeof(query));
    //char select1[] = "SELECT * FROM Request_admin limit %d;"; 
    // strcpy(query,"SELECT * FROM Request_admin;");
    sprintf(query, "Select * from Request_admin limit %d", number_of_request);
    mysql_free_result(res);
    res = make_query(query);
    int i = 0;
    admin_request **request_list;
    request_list = malloc(sizeof(admin_request*)*number_of_request);
    *request_list = malloc(sizeof(admin_request)*number_of_request);
    while((row = mysql_fetch_row(res))){
        (*request_list)[i].opcode = 303;
        (*request_list)[i].id = atoi(row[0]);
        strcpy((*request_list)[i].username, row[1]);
        strcpy((*request_list)[i].date, row[2]);
        i++;
    }
    //check request_list
    for(int j = 0; j < number_of_request; j++){
        printf("Request %d: %d\n", j+1, (*request_list)[j].id);
        printf("Username: %s\n", (*request_list)[j].username);
        printf("Request date: %s\n", (*request_list)[j].date);
    }
    //Send request list to client
    char rcv[MAX_QUERY_LEN];
    send(socket, &i, sizeof(i), 0);
    for(int j = 0; j < i; j++){
        recv(socket, rcv, sizeof(rcv)-1, 0);
        if(strcmp(rcv, "OK") == 0){
            send(socket, &((*request_list)[j]), sizeof(admin_request), 0);
        }
    }
    //Free memory
    mysql_free_result(res);
}

void approveAdminRequest(int socket, char *buffer){
    //Convert buffer to approve_admin_request
    approve_admin_request *request = (approve_admin_request*)buffer;
    int user_id = request->user_id;
    char *username = request->username;
    printf("  ###Admin: %d approve admin request from %s\n", user_id, username );
    // char *username = request->username;
    //Delete request from database
    char query[MAX_QUERY_LEN];
    sprintf(query, "DELETE FROM Request_admin WHERE username = '%s';", username);
    MYSQL_RES *res = make_query(query);
    if(res == NULL){
        printf("1\n");
        int reply = 0;
        send(socket, &reply, sizeof(reply), 0);
        return;
    } 
    mysql_free_result(res);
    //Update User_info
    memset(query, 0, sizeof(query));
    sprintf(query, "UPDATE User_info SET role = 1 WHERE username = '%s';", username);
    res = make_query(query);
    if(res == NULL) {
        printf("2\n");
        int reply = 0;
        send(socket, &reply, sizeof(reply), 0);
        return;
    } else {
        printf("3\n");
        int reply = 1;
        send(socket, &reply, sizeof(reply), 0);
    }
     mysql_free_result(res);
    //Send approve request success signal to client

}

//!####### END ADMIN FUNCTIONS #######