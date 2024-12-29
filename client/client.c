#include "../data.h"
#include "admin.c"
#include "user.c"

void UIHomePage(int client_sockfd);
void UILogin(int client_sockfd); // Handle UI for login page
login_server_response login(int client_sockfd); // Handle logic for login page

void UISignUp(int client_sockfd); // Handle UI for signup page
int signup(int client_sockfd, register_data data2); // Handle logic for signup page

void UIMainAppMenu(int client_sockfd);
void UIMainAppMenuAdmin(int client_sockfd);

login_data data;
int id;
//Client functions

void UILogin(int client_sockfd){
    int check = 0;
    //login_data data;
    printf("Login to the system!\n");
    for (int attempt = 0; attempt < 5; attempt++){
        memset(&data, 0, sizeof(login_data));
        printf("Enter the username: ");
        __fpurge(stdin);
        fgets(data.username, sizeof(data.username), stdin);
        data.username[strlen(data.username)-1] = '\0';
        __fpurge(stdin);
        printf("Enter the password: ");
        fgets(data.password, sizeof(data.password), stdin);
        __fpurge(stdin);
        data.password[strlen(data.password)-1] = '\0';
        data.opcode = 100; //Code for login

        // Call login handler
        login_server_response response = login(client_sockfd);

        if(response.valid == 1) {
            if(response.previlege == 0){
                check = 1;
                printf("Login successfully!\n");
                id = response.user_id;
                data.user_id = id;
                UIMainAppMenu(client_sockfd);
                break;
            } else {
                check = 1;
                printf("Admin, login successfully!\n");
                id = response.user_id;
                data.user_id = id;
                UIMainAppMenuAdmin(client_sockfd);
                break;
            }
        }
        else if (response.valid == 2){
            printf("Login failed! Account is being used by another user!\n");
        }
        else{
            printf("Login failed! Please retry\n");
        }
    }
    if (check == 0){
        printf("Attempt login failed 5 times.\n");
        UIHomePage(client_sockfd);
    }
}

login_server_response login(int client_sockfd){
    send(client_sockfd, &data, sizeof(data), 0);//Send username and password
    login_server_response response;
    recv(client_sockfd, &response, sizeof(response), 0);//Receive check login flag from server
    return response;
}

void UISignUp(int client_sockfd){
    register_data data2;
    printf("Sign up to the system!\nEnter the username: ");
    scanf(" %s", data2.username);
    printf("Enter the password: ");
    scanf(" %s", data2.password);
    printf("Enter the email: ");
    scanf(" %s", data2.email);
    data2.opcode = 101; //Code for signup

    // Call signup handler
    int check = signup(client_sockfd, data2);

    if(check == 1){
        printf("Sign up successfully!\n");
        UIHomePage(client_sockfd);
    }
    else{
        printf("Sign up failed! Please retry\n");
        UIHomePage(client_sockfd);
    }
}

int signup(int client_sockfd, register_data data2){
    int check = 0;
    send(client_sockfd, &data2, sizeof(register_data), 0);//Send username and password
    recv(client_sockfd, &check, sizeof(check), 0);//Receive check login flag from server
    return check;
}

//UI functions
void UIHomePage(int client_sockfd){
    printf("Welcome to the system!\n");
    printf("1. Login\n");
    printf("2. Sign up\n");
    printf("3. Exit\n");
    printf("Please choose your option: ");
    int option;
    scanf(" %1d", &option);
    __fpurge(stdin);
    switch(option){
        case 1:
            UILogin(client_sockfd);
            return;
        case 2:
            UISignUp(client_sockfd);
            return;
        case 3:
            printf("Exit successfully!\n");
            return;
        default:
            printf("Invalid option!\n");
            break;
        }
    UIHomePage(client_sockfd);
}

//Main function
int main(int argc, char const *argv[]){
    int client_sockfd;
    struct sockaddr_in serv_addr;
    char buff[BUFF_SIZE];
    //Construct socket
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //Specify server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //Request to connect server
    if(connect(client_sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0){
        printf("Cannot connecting to server...Please retry!");
        return 0;
    }

    //Start application
    UIHomePage(client_sockfd);
    return 0;
}