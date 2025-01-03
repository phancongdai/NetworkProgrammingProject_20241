#include <mysql/mysql.h>
#include "../data.h"
#include "admin_servant.c"
#include "user_servant.c"

#define MAX_QUERY_LEN 3000
MYSQL *conn;
#define MAX_INPUT_LENGTH 512

//srand(time(NULL));
int key_arr[1000];
char start_time_exam[20];
char end_time_exam[20];
char answer_chat_mode[MAX_QUERY_LEN];
char buffer[MAX_QUERY_LEN];

void getAllSubjects(int socket, char client_message[]);
void getCountOfChosenSubject(int socket, char client_message[]);
void extract_error(MYSQL *conn);
void connectToDB();
MYSQL_RES *make_query(char query[]);
int query_database_for_login(char *username, char *password, int *previlege, int *id);
void check_login(int socket, char *buffer);
int query_database_for_signup(char *username);
void addNewUser(char username[], char user_password[], char email[]);
void check_signup(int socket, char *buffer);
void *connection_handler(void *);

typedef struct {
    char name[20];
    char password[20];
    // int status;  // 1: active, 0: blocked
    char email[20];
    int socket; 
    int role; // Socket descriptor for the client (-1 if not connected)
} Account;

Account user[100];
int current_number_of_user = 0;

void delete_all_from_Logged_user() {
    char query[MAX_QUERY_LEN];
    strcpy(query, "DELETE FROM Logged_user;");
    MYSQL_RES *res = make_query(query);
    if(!res) {
        fprintf(stderr, "No row need to be deleted in Logged_user table\n");
    }
    mysql_free_result(res);
}

void list_all_user() {
    // current_number_of_user = 0;
    char query[MAX_QUERY_LEN];
    strcpy(query, "SELECT * FROM User_info;");
    MYSQL_RES *res = make_query(query);
    if(!res) {
        fprintf(stderr, "Failed to execute query.\n");
        return;
    }
    MYSQL_ROW row;
    int num_fields = mysql_num_fields(res);
    printf("Number of column in User_info table: %d\n", num_fields);
    current_number_of_user = 0;
    while((row = mysql_fetch_row(res))) {
        if(row[1]) {
            strcpy(user[current_number_of_user].name, row[1]);
        }
        if(row[2]) {
            strcpy(user[current_number_of_user].password, row[2]);
        }
        if(row[3]) {
            strcpy(user[current_number_of_user].email, row[3]);
        }
        if(row[4]) {
            user[current_number_of_user].role = atoi(row[4]);
        }
        current_number_of_user++;
        printf("%d\t%s\t%s\t%s\t%s\n", current_number_of_user, row[1], row[2], row[3], row[4]);
    }
    mysql_free_result(res);
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: ./server <PortNumber>\n");
        return -1;
    }
    //Connect to database
    connectToDB();

    //Create socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == 1){
        perror("Fail in initializing socket\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Server socket created successfully\n");
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //Bind the socket to the IP address and port
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0){
        printf("Binding fail !\n");
        exit(0);
    }
    else{
        printf("Socket successfully binded...\n");
    }
    if(listen(server_socket, 100) != 0){
        printf("Listen failed...\n");
        exit(0);
    }
    else{
        printf("Server is listening...\n");
    }
    printf("##### List of user #####\n");
    list_all_user();
    printf("########################\n");
    delete_all_from_Logged_user();
    //Create threads
    int no_threads = 0;
    pthread_t threads[100];
    while(no_threads < 100){
        // printf("Listening...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Connection accepted\n");   
        if(pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) <0 ){
            perror("Could not create thread\n");
            return 1;
        }
        if(client_socket < 0){
            printf("Server accept failed...\n");
            exit(0);
        }
        else printf("Server accept the client %d...\n", no_threads);
        // printf("Handler assigned\n");
        no_threads++;
    }
    int k = 0;
    for(k = 0; k < 100; k++){
        pthread_join(threads[k], NULL);
    }

    close(server_socket);
    mysql_close(conn);
    return 0;
}

void extract_error(MYSQL *conn) {
    fprintf(stderr, "%s\n", mysql_error(conn));
}

void connectToDB(){
    char *server = "localhost";
    char *user = "root";
    char *password = "123456"; 
    char *database = "network_db_01";

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        extract_error(conn);
        exit(1);
    }
    printf("Connect to database successfully\n");
}

// MYSQL_RES *make_query(char query[]){
//     MYSQL_RES *res;
//     if (mysql_query(conn, query)) {
//         extract_error(conn);
//         return NULL;
//     }

//     res = mysql_use_result(conn);
//     //mysql_close(conn);
//     return res;
// }
MYSQL_RES *make_query(char query[]){
    MYSQL_RES *res;
    if (mysql_query(conn, query)) {
        extract_error(conn);
        return NULL;
    }

    res = mysql_store_result(conn); 
    if (res == NULL && mysql_field_count(conn) != 0) {
        extract_error(conn);
        return NULL;
    }

    return res;
}



int query_database_for_login(char *username, char *password, int *previlege, int *id){ 
    //return 1 if login success and 0 if login fail
    //SELECT * FROM User_info WHERE username = '' AND password = '';
    char query[MAX_QUERY_LEN];
    strcpy(query, "SELECT * FROM User_info WHERE username = \'");
    strcat(query, username);
    strcat(query, "\' AND password = \'");
    strcat(query, password);
    strcat(query, "\';");
    // printf("\nquery = \'%s\'\n\n", query);
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row = mysql_fetch_row(res);
    if(row != NULL){
        *id = atoi(row[0]);
        *previlege = atoi(row[4]);
        mysql_free_result(res);
        return 1;
    }
    mysql_free_result(res);
    return 0;
}

void check_login(int socket, char *buffer){
    //Convert buffer to login_msg
    login_data *login = (login_data*)buffer;
    char *username = login->username;
    char *password = login->password;
    int previlege, id;
    int found = query_database_for_login(username, password, &previlege, &id);
    int valid = 1;
    if(found == 1){
        char query[MAX_QUERY_LEN];
        strcpy(query, "SELECT * FROM Logged_user WHERE username = \'");
        strcat(query, username);
        strcat(query, "\';");
        MYSQL_RES *res = make_query(query);
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row != NULL){
            printf("Login fail! Account is being used by another user!\n");
            valid = 2;
        }
        else{
            valid = 1;
            printf("Login successful\n");
            char query[MAX_QUERY_LEN];
            strcpy(query, "INSERT INTO `Logged_user` VALUES (\'");
            strcat(query, username);
            strcat(query, "\');");
            if (mysql_query(conn, query)) {
                extract_error(conn);
                exit(1);
            }
        }
        mysql_free_result(res);
    }
    else{
        valid = 0;
        printf("Login fail\n");
    }
    //Send login success or fail signal to client
    login_server_response response;
    response.opcode = 102;
    response.valid = valid;
    response.previlege = previlege;
    response.user_id = id;
    //Check response
    printf("Request from user: %s\n", username);
    printf("Response:\n");
    printf("Opcode: %d\n", response.opcode);
    printf("Valid: %d\n", response.valid);
    printf("Previlege: %d\n", response.previlege);
    printf("Id: %d\n", response.user_id);
    send(socket, &response, sizeof(response), 0);
}

int query_database_for_signup(char *username){ 
    //return 1 if login success and 0 if login fail
    //SELECT * FROM User_info WHERE username = '' AND password = '';
    char query[MAX_QUERY_LEN];
    strcpy(query, "SELECT * FROM User_info WHERE username = \'");
    strcat(query, username);
    strcat(query, "\';");
    MYSQL_RES *res = make_query(query);
    MYSQL_ROW row = mysql_fetch_row(res);
    mysql_free_result(res);
    int result = (row == NULL);
    return result;
}

void addNewUser(char username[], char user_password[], char email[]){
    MYSQL *conn;

    char *server = "localhost";
    char *user = "root";
    char *password = "123456"; 
    char *database = "network_db_01";

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        extract_error(conn);
        exit(1);
    }

    char query[MAX_QUERY_LEN];
    strcpy(query, "INSERT INTO");
    strcat(query, username);
    strcat(query, "\';");
    if(strcmp(email, "") == 0){
        strcpy(query, "INSERT INTO User_info(username, password, role) VALUES ( \'");
        strcat(query, username);
        strcat(query, "\', \'");
        strcat(query, user_password);
        strcat(query, "\', ");
        strcat(query, "0");
        strcat(query, " );");
    }
    else{
        strcpy(query, "INSERT INTO User_info(username, password, email, role) VALUES ( \'");
        strcat(query, username);
        strcat(query, "\', \'");
        strcat(query, user_password);
        strcat(query, "\', \'");
        strcat(query, email);
        strcat(query, "\', ");
        strcat(query, "0");
        strcat(query, " );");
    }
    //query
    if (mysql_query(conn, query)) {
        extract_error(conn);
        exit(1);
    }

}

void check_signup(int socket, char *buffer){
    //Convert buffer to register_msg
    register_data *register_msg = (register_data*)buffer;

    //Testcase
    char *username = register_msg->username;
    char *password = register_msg->password;
    char *email = register_msg->email;
    printf("Username: %s\n", username);
    printf("Password: %s\n", password);
    printf("Email: %s\n", email);
    int valid = query_database_for_signup(username);
    if(valid == 1){
        addNewUser(username, password, email);
        printf("Add new user successfully\n");
        printf("##### Updated list of user #####\n");
        list_all_user();
        printf("################################\n");
    }
    else{
        printf("Sign up failed!\n");
    }
    send(socket, &valid, 4, 0);
}

char* call_ollama(const char *prompt) {
    char command[MAX_INPUT_LENGTH + 100]; // Space for command and the prompt
    snprintf(command, sizeof(command), "ollama run llama3.1:8b \"%s\"", prompt);
    
    // Run the command and capture the output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run Ollama command");
        exit(1);
    }

    char response[MAX_INPUT_LENGTH];
    strcpy(answer_chat_mode, "Assistant: ");
    while (fgets(response, sizeof(response), fp) != NULL) {
        strcat(answer_chat_mode, response);
    }
    fclose(fp);
    return answer_chat_mode;
}

void answer_question(int socket, char*client_message) {
    ask_ai_data *question = (ask_ai_data*)client_message;
    int user_id = question->user_id;
    char *prompt = question->question;
    // prompt[strlen(prompt)] = '\0';
    // printf("okok: %s\n", prompt);
    if(strcmp(prompt, "exit") == 0) {
        printf("User %d quits AI assistance in practice mode\n", user_id);
        return;
    }
    strcat(prompt, "\nLet's choose one of the given options and explain your choice.");
    char *answer = call_ollama(prompt);
    printf("Question from user id %d to AI assistance: %s\n", user_id, prompt);
    printf("%s\n", answer);
    send(socket, answer, MAX_QUERY_LEN, 0);
}

void chat_mode(int socket) {
    printf("### Assistance mode ###\n");
    char question[MAX_INPUT_LENGTH];
    int chunk_size = 512;
    while(1) {
        memset(buffer, 0, MAX_QUERY_LEN);
        recv(socket, buffer, MAX_QUERY_LEN, 0);
        if(strcmp(buffer, "exit")==0) {
            break;
        }
        printf("Question: %s\n", buffer);
        strcpy(question, buffer);
        strcpy(buffer, call_ollama(question));
        // buffer[strlen(buffer)] = '\0';
        printf("%s\n", buffer);
        // printf("Answer from AI: %s\n", buffer);
        send(socket, buffer, MAX_QUERY_LEN, 0);
    }
    printf("### Assistance mode terminated ###\n");
}


void *connection_handler(void *client_socket){
    int socket = *(int*)client_socket;
    char client_message[MAX_QUERY_LEN];
    int len;
    while((len = recv(socket, client_message, sizeof(client_message), 0)) > 0){
    client_message[len] = '\0';
    char opcode[4];
    strncpy(opcode, client_message, 4);
    //printf("%c\n",opcode[0]);
    int code = *(int*)opcode;
    printf("\nOpcode: %d\n", code);
    switch (code) {

    case 0:
        break;
        // printf("Opcode 0 received\n");
        // break;
    //########## User request ##########
    case 100:
        check_login(socket, client_message);
        break;
    case 101:
        check_signup(socket, client_message);
        break;
    case 103:
        requestUserInfo(socket, client_message);
        break;

    case 111:
        logout_data *logout_sig = (logout_data*)client_message;
        printf("Logout successful\n");
        printf("Username %s logout the system\n", logout_sig->username);
        break;

    case 203:
        requestExamList(socket, (request_exam_list*)client_message);
        break;
    case 204:
        requestQuestionList_PublicExam(socket, client_message);
        break;
    case 205:
        evaluateExam(socket, client_message);
        break;
    case 207:
        getUserHistory(socket, client_message);
        break;
    case 301:
        requestAdmin(socket, client_message);
        break;
    //########## Admin request ##########
    case 302:
        getAdminRequestInfo(socket, client_message);
        break;
    // case 304:
    //     approveAdminRequest(socket, client_message);
    //     break;
    case 601:
        searchQuestionById(socket, client_message);
        break;
    case 602:
        searchQuestionByContent(socket, client_message);
        break;
    case 603:
        advanceQuestionSearch(socket, client_message);
        break;
    case 604:
        addNewQuestion(socket, client_message);
        break;
    case 605:
        deleteQuestion(socket, client_message);
        break;
        case 607:
        getAllSubjects(socket, client_message);
        break;
    case 608:
        getCountOfChosenSubject(socket, client_message);
        break;
    case 701:
        searchExamById(socket, client_message);
        break;
    case 702:
        createRandomExam(socket, client_message);
        break;
    case 703:
        createExamByQuestion(socket, client_message);
        break;
    case 704:
        deleteExam(socket, client_message);
        break;
    case 801:
        createRoom(socket, client_message);
        break;
    case 802:
        deleteRoom(socket, client_message);
        break;
    case 803:
        showRoomYouCreated(socket, client_message);
        break;
    case 804:
        showRoomYouWereAdded(socket, client_message);
        break;
    case 805:
        showAllYourRoom(socket, client_message);
        break;
    case 811:
        addUserToRoom(socket, client_message);
        break;
    case 812:
        deleteUserFromRoom(socket, client_message);
        break;
    case 999:
        chat_mode(socket);
        break;
    case 998:
        answer_question(socket, client_message);
        break;
    default:
        break;
    }
    memset(client_message, 0, sizeof(client_message));
    }
    close(socket);
}