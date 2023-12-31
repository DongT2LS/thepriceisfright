#include<bits/stdc++.h>
#include "../model/request.hpp"
#include<iostream>
#include "controller.hpp"
#include "../model/chat.hpp"
using namespace std;

void join(struct Request *request)
{
    int game_id = atoi(request->message);
    Game *game = find_game(game_id,games); 
    User *user = find_user(request->client_id,users);
    if(game == nullptr){
        cout << "Not found !" << endl;
        return;
    }
    if(user == nullptr || user->status == USER_OFFLINE) {
        cout << "User offline !" << endl;
        return;
    }
    switch (game->status)
    {
    case GAME_END:
        cout << "GAME ended !" << endl;
        break;
    case GAME_INPROGRESS:
        cout << "GAME started !" << endl;
        break;
    case GAME_READY:
        user->status = USER_READY;
        user->game_id = game->getId();
        game->addMembers(request->client_id);
        cout << "JOIN ... " << user->getUsername() << endl;
    default:
        break;
    }
    printf("JOIN ... \n");
}

void newroom(struct Request *request)
{
    create_game(request->client_id);
    cout << "NEW ROOM " << request->client_id << endl;
}

void invite(struct Request *request)
{
    char *game_id_string,*user_id_string;
    game_id_string = strtok(request->message," ");
    user_id_string = strtok(NULL,"\0");
    int game_id = atoi(game_id_string);
    int user_id = atoi(user_id_string);
    Game *game = find_game(game_id,games); 
    User *user = find_user(user_id,users);
    if(game == nullptr){
        cout << "Not found !" << endl;
        return;
    }
    if(user == nullptr) {
        cout << "User offline !" << endl;
        return;
    }
    switch (game->status)
    {
    case GAME_END:
        cout << "GAME ended !" << endl;
        break;
    case GAME_INPROGRESS:
        cout << "GAME started !" << endl;
        break;
    case GAME_READY:
        user->status = USER_READY;
        user->game_id = game->getId();
        game->addMembers(user_id);
        cout << "INVITE ... " << user->getId()<< endl;
    default:
        break;
    }
}

void leave(struct Request *request)
{
    User *user = find_user(request->client_id,users);
    user->game_id = 0;
    user->status = USER_ONLINE;
    cout << user->getUsername() << "LEAVE ...\n";
}

void choose(struct Request *request)
{
    
    printf("CHOOSE ...\n");
}

void ready(struct Request *request)
{
    User *user = find_user(request->client_id,users);
    user->status = USER_PLAYING;
    cout << user->getUsername() <<" READY ...\n" ;
}

void chat(struct Request *request)
{
    Chat chat(request->client_id,1,request->message);
    User *user = find_user(2,users);
    printf("%s --- %s \n",user->getUsername(),user->getPassword());
    chat.store();
    cout<<"CHAT ..." <<endl;
}

void start(struct Request *request)
{
    int game_id = atoi(request->message);
    Game *game = find_game(game_id,games);
    cout << game_id << " " << game->getId() << endl;
    if(game == nullptr) {
        cout << "Not found " << game_id << endl;
        return;
    }
    User *user = find_user(request->client_id,users);
    user->status = USER_PLAYING;
    if(game->getOwnerId() != request->client_id)
    {
        cout << "Owner_id " << game->getOwnerId() << " Your_id : " << request->client_id << " Game_id : " << game->getId() << endl;
        cout << "You are not the owner of this game" << endl;
        return;
    }else{
        for(int member_id : game->getMembers())
        {
            User *user = find_user(member_id,users);
            if(user->status != USER_PLAYING){
                cout << "Co nguoi choi chua san sang" << endl;
                return;
            }
        }
    }
    game->status = GAME_INPROGRESS;

    printf("START ...\n");
}