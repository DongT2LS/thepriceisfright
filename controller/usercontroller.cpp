#include <string>
#include <stdio.h>
#include <iostream>
#include "controller.hpp"
#include <vector>
#include "../model/request.hpp"
#include "../model/user.hpp"

using namespace std;

void signup(struct Request *request)
{
    char *username, *password;
    username = strtok(request->message, " ");
    password = strtok(NULL, "\0");
    for (User *user : users)
    {
        if (strcmp(username, user->getUsername()) == 0)
        {
            printf("Username invalid");
            return;
        }
    }
    int id = users.size() + 1;
    User::store(id,username,password);
    users.push_back(new User(id,username,password));
}

void login(struct Request *request)
{
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
                return;
            }
            printf("User logged !\n");
            return;
        }
    }
    printf("Account not exist !\n");
}

void logout(struct Request *request)
{
    cout << "Logout !" << endl;
    for (User *user : users)
    {
        if (user->getId() == request->client_id)
        {
            user->setClientSocket(0);
            user->status = USER_OFFLINE;
            printf("%s offline !\n", user->getUsername());
            return;
        }
        printf("User offline !\n");
        return;
    }
}