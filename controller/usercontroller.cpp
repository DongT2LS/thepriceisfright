#include <string>
#include <stdio.h>
#include <iostream>
#include "controller.hpp"
#include <vector>
#include "../model/request.hpp"
#include "../model/user.hpp"
#include "../model/response.hpp"
#include <unistd.h>
using namespace std;

struct Response signup(struct Request *request)
{
    struct Response response;
    response.type = RESPONSE_SIGNUP;
    
    char *username, *password;
    username = strtok(request->message, " ");
    password = strtok(NULL, "\0");
    for (User *user : users)
    {
        if (strcmp(username, user->getUsername()) == 0)
        {
            printf("Username invalid");
            strcpy(response.message, "Username invalid");
            response.status = ERROR;
            return response;
        }
    }
    int id = users.size() + 1;
    User::store(id, username, password);
    users.push_back(new User(id, username, password));
    response.status = SUCCESS;
    strcpy(response.message, "Username valid");
    return response;
}

struct Response login(struct Request *request)
{
    struct Response response;
    request->message[strlen(request->message) - 1] == '\n' ? request->message[strlen(request->message) - 1] = '\0' : 0;
    cout << request->message << endl;
    response.type = RESPONSE_LOGIN;
    char *username, *password;
    username = strtok(request->message, " ");
    password = strtok(NULL, "\0");
    for (User *user : users)
    {
        if (strcmp(username, user->getUsername()) == 0)
        {
            cout << "Pass :" << password << "Correct pass : " << user->getPassword() << endl;
            if (strcmp(password, user->getPassword()) != 0)
            {
                response.status = ERROR;
                strcpy(response.message, "Wrong password");
                return response;
            }
            else
            {
                if (user->status == USER_OFFLINE)
                {
                    // Update client socket va trang thai cua nguoi dung
                    user->setClientSocket(request->client_socket);
                    user->status = USER_ONLINE;
                    printf("%s online !\n", user->getUsername());
                    
                    // Thong bao den tat ca user cap nhat nguoi dung online va lay nhung game dang ready
                    update_list_online_user(user->getId());
                    char id_string[5];
                    sprintf(id_string, "%d", user->getId());
                    strcpy(response.message, id_string);
                    strcat(response.message," ");
                    strcat(response.message,user->getUsername());
                    cout << response.message << endl;
                    response.status = SUCCESS;
                    return response;
                }
                strcpy(response.message, "User logged !\n");
                response.status = ERROR;
                return response;
            }
        }
    }
    strcpy(response.message, "Account not exist !\n");
    response.status = ERROR;
    return response;
}

struct Response logout(struct Request *request)
{
    struct Response response;
    response.type = RESPONSE_LOGOUT;
    cout << "Logout ! " << request->client_id << endl;
    for (User *user : users)
    {
        if (user->getId() == request->client_id)
        {
            user->setClientSocket(0);
            user->status = USER_OFFLINE;
            printf("%s offline !\n", user->getUsername());
            response.status = SUCCESS;
            strcpy(response.message, "Logout");

            update_list_online_user(request->client_id);
            return response;
        }
    }
    response.status = ERROR;
    strcpy(response.message, "Not found !");
    return response;
}