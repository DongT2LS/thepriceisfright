#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include "../config/config.hpp"
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

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

    // static void getListUser()
    // {
    //     FILE *file = fopen(USER_DATABASE, "r");

    //     if (file == NULL)
    //     {
    //         printf("Can't open file : %s\n", USER_DATABASE);
    //         return;
    //     }
    //     int id;
    //     char *username;
    //     char *password;

    //     while (fscanf(file, "%d %s %s", &id, username, password) == 3) // Nếu đọc đủ 3 trường của 1 account thì tiếp tục đọc
    //     {
    //         User user(id, username, password);
    //         listUser.push_back(user);
    //         cout << user.getId() << " ---- " << user.getUsername() << " ---- " << user.getPassword() << endl;

    //         // delete(user);
    //     }
    //     currentLastId = id;
    //     fclose(file);
    // }

    // static int autoIncreaseId()
    // {
    //     return ++currentLastId;
    // }

    static void store(int id,char *username, char *password)
    {
        FILE *file = fopen(USER_DATABASE, "a");
        if (file == NULL)
        {
            printf("Can't open file : %s\n", USER_DATABASE);
            return;
        }
        fprintf(file, "%d %s %s\n", id, username, password);
        fclose(file);
    }

    // static vector<User> getOnlineUsers()
    // {
    //     for (User onlineUser : onlineUsers)
    //     {
    //         cout << "User : " << onlineUser.getUsername() << endl;
    //     }
    //     return onlineUsers;
    // }

    // static void login(char *username, char *password, int client_socket)
    // {
        // for (User user : listUser)
        // {
        //     if (strcmp(username, user.getUsername()) == 0)
        //     {
        //         for (User onlineUser : onlineUsers)
        //         {
        //             if (strcmp(username, onlineUser.getUsername()) == 0)
        //             {
        //                 printf("Account logged !\n");
        //                 return;
        //             }
        //         }
        //         user.setClientSocket(client_socket);
        //         user.status = USER_ONLINE;
        //         onlineUsers.push_back(user);
        //         printf("%s online !\n", user.getUsername());
        //         return;
        //     }
        // }
        // printf("Account not exist ! \n");
    // }

    // static void logout(int client_id)
    // {
        // auto it = std::find_if(onlineUsers.begin(), onlineUsers.end(),
        //                        [client_id](User &user)
        //                        {
        //                            return user.getId() == client_id;
        //                        });

        // if (it != onlineUsers.end())
        // {
        //     onlineUsers.erase(it);
        //     printf("User with id %d logged out.\n", client_id);
        // }
        // else
        // {
        //     printf("User with id %d not found in onlineUsers.\n", client_id);
        // }
    // }
};

#endif