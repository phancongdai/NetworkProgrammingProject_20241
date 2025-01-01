#ifndef CLIENT_H
#define CLIENT_H
void UIHomePage(int client_sockfd);
void UILogin(int client_sockfd); // Handle UI for login page
login_server_response login(int client_sockfd); // Handle logic for login page

void UISignUp(int client_sockfd); // Handle UI for signup page
int signup(int client_sockfd, register_data data2); // Handle logic for signup page

void UIMainAppMenu(int client_sockfd);
void UIMainAppMenuAdmin(int client_sockfd);
extern login_data data;

extern int id;
#endif