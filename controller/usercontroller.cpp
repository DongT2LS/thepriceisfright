#include <string>
#include <stdio.h>
#include <iostream>
#include "controller.hpp"
#include <vector>
#include "../model/request.hpp"
#include "../model/user.hpp"
#include "../model/response.hpp"

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
            strcpy(response.message,"Username invalid");
            return response;
        }
    }
    int id = users.size() + 1;
    User::store(id, username, password);
    users.push_back(new User(id, username, password));
    strcpy(response.message,"Username valid"); 
    return response;
}

struct Response login(struct Request *request)
{
    struct Response response;
    response.type = RESPONSE_LOGIN;
    char *username, *password;
    username = strtok(request->message, " ");
    password = strtok(NULL, "\0");
    // User::login(username,password,request->client_socket);
    for (User *user : users)
    {
        if (strcmp(username, user->getUsername()) == 0)
        {
            if (user->status == USER_OFFLINE)
            {
                user->setClientSocket(request->client_socket);
                user->status = USER_ONLINE;
                printf("%s online !\n", user->getUsername());
                strcpy(response.message,"User online !\n");
                response.status = SUCCESS;
                return response;
            }
            strcpy(response.message,"User logged !\n");
            response.status = ERROR;
            return response;
        }
    }
    strcpy(response.message,"Account not exist !\n");
    response.status = ERROR;
    return response;
}

struct Response logout(struct Request *request)
{
    struct Response response;
    response.type = RESPONSE_LOGOUT;
    cout << "Logout !" << endl;
    for (User *user : users)
    {
        if (user->getId() == request->client_id && user->status != USER_OFFLINE)
        {
            user->setClientSocket(0);
            user->status = USER_OFFLINE;
            printf("%s offline !\n", user->getUsername());
            response.status = SUCCESS;
            strcpy(response.message,"Logout");
            return response;
        }
    }
    response.status = ERROR;
    strcpy(response.message,"Not found !");
    return response;
}