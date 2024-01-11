#ifndef CHAT_HPP
#define CHAT_HPP

#include <semaphore.h>
#include<bits/stdc++.h>
#include "../config/config.hpp"
using namespace std;

extern sem_t chat_semaphore;

class Chat
{
private:
    int user_id,
        game_id;
    char chat[300];
public:
    // Constructor
    Chat(int _user_id, int _game_id, const char* _chat)
        : user_id(_user_id), game_id(_game_id)
    {
        // Sử dụng strncpy để tránh tràn bộ nhớ
        strncpy(chat, _chat, sizeof(chat) - 1);
        chat[sizeof(chat) - 1] = '\0'; // Đảm bảo kết thúc chuỗi
    }

    // Getter for user_id
    int getUserId() const
    {
        return user_id;
    }

    // Getter for game_id
    int getGameId() const
    {
        return game_id;
    }

    // Getter for chat
    const char* getChat() const
    {
        return chat;
    }

    void store()
    {
        sem_wait(&chat_semaphore);
        FILE *file = fopen(CHAT_DATABASE, "a");
        if (file == NULL)
        {
            printf("Can't open file : %s\n", USER_DATABASE);
            return;
        }
        fprintf(file, "%d %d %s\n", user_id,game_id,chat);
        fclose(file);
        sem_post(&chat_semaphore);
    }
};


#endif