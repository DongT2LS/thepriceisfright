#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include "../config/config.hpp"
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <bits/stdc++.h>
#include <semaphore.h>

using namespace std;

extern sem_t user_semaphore;

enum UserStatus {
    USER_ONLINE,
    USER_OFFLINE,
    USER_READY,
    USER_PLAYING
};

class User
{
private:
    int id;
    char username[50];
    char password[50];
    int client_socket;
    
public:
    UserStatus status;
    int game_id;
    // static int currentLastId;
    // static vector<User> listUser;
    // static vector<User> onlineUsers;
    User():id(0){}
    User(int _id, const char *_username, const char *_password)
        : id(_id), client_socket(0) , status(USER_OFFLINE) , game_id(0)
    {   
        // Sử dụng strncpy để tránh tràn bộ nhớ
        strncpy(username, _username, sizeof(username) - 1);
        username[sizeof(username) - 1] = '\0'; // Đảm bảo kết thúc chuỗi

        strncpy(password, _password, sizeof(password) - 1);
        password[sizeof(password) - 1] = '\0'; // Đảm bảo kết thúc chuỗi
    }
    ~User() {}

    int getId()
    {
        return id;
    }

    char *getUsername()
    {
        return username;
    }

    char *getPassword()
    {
        return password;
    }

    void setClientSocket(int _client_socket)
    {
        client_socket = _client_socket;
    }

    int getClientSocket()
    {
        return client_socket;
    }

    static void store(int id,char *username, char *password)
    {
        sem_wait(&user_semaphore);
        FILE *file = fopen(USER_DATABASE, "a");
        if (file == NULL)
        {
            printf("Can't open file : %s\n", USER_DATABASE);
            return;
        }
        fprintf(file, "%d %s %s\n", id, username, password);
        fclose(file);
        sem_post(&user_semaphore);
    }

};

#endif