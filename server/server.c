#include <mysql/mysql.h>
#include "../data.h"
#include "admin_servant.c"
#include "user_servant.c"

#define MAX_QUERY_LEN 3000
MYSQL *conn;

//srand(time(NULL));
int key_arr[1000];
char start_time_exam[20];
char end_time_exam[20];

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

MYSQL_RES *make_query(char query[]){
    MYSQL_RES *res;
    if (mysql_query(conn, query)) {
        extract_error(conn);
        return NULL;
    }

    res = mysql_use_result(conn);
    //mysql_close(conn);
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
    int attempts = 0;
    // while(attempts<3) {
    //     int 
    // }
    int valid = query_database_for_login(username, password, &previlege, &id);

    if(valid == 1){
        printf("Login success\n");
    }
    else{
        printf("Login fail\n");
    }
    //Send login success or fail signal to client
    login_server_response response;
    response.opcode = 102;
    response.valid = valid;
    response.previlege = previlege;
    response.user_id = id;
    //Check response
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
    switch (code){
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
    case 203:
        requestExamList(socket, client_message);
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
    case 304:
        approveAdminRequest(socket, client_message);
        break;
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
    //########## Advanced request ##########
    case 1001:
        getImageQuestion(socket, client_message);
        break;
    case 1002:
        getSoundQuestion(socket, client_message);
        break;
    case 1003:
        getUserStatistic(socket, client_message);
        break;
    default:
        break;
    }
    memset(client_message, 0, sizeof(client_message));
    }
    close(socket);
}