#include <bits/stdc++.h>
#include "../model/request.hpp"
#include <iostream>
#include "controller.hpp"
#include "../model/chat.hpp"
#include <sys/socket.h>
#include "../model/response.hpp"
using namespace std;

struct Response join(struct Request *request)
{
    struct Response response;
    int game_id = atoi(request->message);
    Game *game = find_game(game_id, games);
    User *user = find_user(request->client_id, users);
    if (game == nullptr)
    {
        response.status = ERROR;
        strcpy(response.message, "Not found !");
        return response;
    }
    if (user == nullptr || user->status == USER_OFFLINE)
    {
        response.status = ERROR;
        strcpy(response.message, "User offline !");
        return response;
    }
    switch (game->status)
    {
    case GAME_END:
        response.status = ERROR;
        strcpy(response.message, "Game end !");
        return response;
    case GAME_INPROGRESS:
        response.status = ERROR;
        strcpy(response.message, "Game started !");
        return response;
    case GAME_READY:
        user->status = USER_READY;
        user->game_id = game->getId();
        game->addMembers(request->client_id);
        response.status = SUCCESS;
        strcpy(response.message, "Join game !");
        return response;
    default:
        break;
    }
    response.status = SUCCESS;
    strcpy(response.message, "Join game !");
    return response;
}

struct Response newroom(struct Request *request)
{
    create_game(request->client_id);
    cout << "NEW ROOM " << request->client_id << endl;
    struct Response response;
    response.status = SUCCESS;
    strcpy(response.message, "Create new room !");
    return response;
}

struct Response invite(struct Request *request)
{
    struct Response response;
    char *game_id_string, *user_id_string;
    game_id_string = strtok(request->message, " ");
    user_id_string = strtok(NULL, "\0");
    int game_id = atoi(game_id_string);
    int user_id = atoi(user_id_string);
    Game *game = find_game(game_id, games);
    User *user = find_user(user_id, users);
    if (game == nullptr)
    {
        response.status = ERROR;
        strcpy(response.message, "Not found !");
        return response;
    }
    if (user == nullptr)
    {
        response.status = ERROR;
        strcpy(response.message, "User offline !");
        return response;
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
        cout << "INVITE ... " << user->getId() << endl;
    default:
        break;
    }
}

struct Response leave(struct Request *request)
{
    User *user = find_user(request->client_id, users);
    user->game_id = 0;
    user->status = USER_ONLINE;
    cout << user->getUsername() << "LEAVE ...\n";
    struct Response response;
    response.status = SUCCESS;
    strcpy(response.message, "leave ...");
    return response;
}

struct Response choose(struct Request *request)
{
    struct Response response;
    response.status = SUCCESS;
    strcpy(response.message, "Choose ...");
    return response;
}

struct Response ready(struct Request *request)
{
    User *user = find_user(request->client_id, users);
    user->status = USER_PLAYING;
    cout << user->getUsername() << " READY ...\n";
    struct Response response;
    response.status = SUCCESS;
    strcpy(response.message, "Ready ...");
    return response;
}

struct Response chat(struct Request *request)
{
    User *user = find_user(request->client_id, users);
    Game *game = find_game(user->game_id, games);
    cout << request->message << endl;
    vector<int> listmem = game->getMembers();
    for (int member_id : listmem)
    {
        if (member_id != request->client_id)
        {
            user = find_user(member_id, users);
            struct Response mes;
            mes.status = SUCCESS;
            strcpy(mes.message, request->message);
            send(user->getClientSocket(), &mes, strlen(request->message), 0);
            cout << user->getUsername() << " " << user->game_id << endl;
        }
    }
    Chat chat(request->client_id, game->getId(), request->message);
    chat.store();
    struct Response response;
    response.status = SUCCESS;
    strcpy(response.message, "Sent message !");
    return response;
}

struct Response start(struct Request *request)
{
    struct Response response;
    int game_id = atoi(request->message);
    Game *game = find_game(game_id, games);
    cout << game_id << " " << game->getId() << endl;
    if (game == nullptr)
    {
        response.status = ERROR;
        strcpy(response.message, "not found ...");
        return response;
    }
    User *user = find_user(request->client_id, users);
    user->status = USER_PLAYING;
    if (game->getOwnerId() != request->client_id)
    {
        response.status = ERROR;
        strcpy(response.message, "You are not the owner of this game");
        return response;
    }
    else
    {
        for (int member_id : game->getMembers())
        {
            User *user = find_user(member_id, users);
            if (user->status != USER_PLAYING)
            {
                response.status = ERROR;
                strcpy(response.message, "Players not ready");
                return response;
            }
        }
    }
    game->status = GAME_INPROGRESS;

    response.status = SUCCESS;
    strcpy(response.message, "Start game");
    return response;
}