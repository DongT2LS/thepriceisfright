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
    response.type = RESPONSE_JOIN;
    int game_id = atoi(request->message);
    Game *game = find_game(game_id, games);
    User *user = find_user(request->client_id, users);
    if (game == nullptr)
    {
        response.status = ERROR;
        cout << "Not found !" << endl;
        strcpy(response.message, "Not found !");
        return response;
    }
    if (user == nullptr || user->status == USER_OFFLINE)
    {
        cout << "User offline !" << endl;
        response.status = ERROR;
        strcpy(response.message, "User offline !");
        return response;
    }

    // Check game status
    switch (game->status)
    {
    case GAME_END:
        cout << "Game end" << endl;
        response.status = ERROR;
        strcpy(response.message, "Game end !");
        return response;
    case GAME_INPROGRESS:
        cout << "Game started" << endl;
        response.status = ERROR;
        strcpy(response.message, "Game started !");
        return response;
    case GAME_READY:
        cout << "Join game ..." << endl;
        user->status = USER_READY;
        user->game_id = game->getId();
        game->addMembers(request->client_id);
        response.status = SUCCESS;
        strcpy(response.message, "Join game !");
        int length = game->getMembers().size();
        for (int i = 0; i < length; i++)
        {
            cout << "Member : " << game->getMembers()[i] << endl;
        }
        return response;
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
    response.type = RESPONSE_NEW_ROOM;
    strcpy(response.message, "room_id owner_id");
    return response;
}

struct Response invite(struct Request *request)
{
    struct Response response;
    response.type = RESPONSE_INVITE;
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

    // Check user exist or user online ?
    if (user == nullptr || user->status != USER_ONLINE)
    {
        response.status = ERROR;
        strcpy(response.message, "User offline !");
        return response;
    }

    // Send invite to another user
    struct Response send_mes_response;
    send_mes_response.status = SUCCESS;
    send_mes_response.type = RESPONSE_SEND_INVITE;
    char room_id_string[10];
    sprintf(room_id_string, "%d", game_id);
    strcpy(send_mes_response.message, room_id_string);
    send(user->getClientSocket(), &send_mes_response, sizeof(struct Response), 0);

    response.status = SUCCESS;
    strcpy(response.message, game_id_string);
    return response;
}

struct Response leave(struct Request *request)
{
    User *user = find_user(request->client_id, users);
    user->game_id = 0;
    user->status = USER_ONLINE;
    cout << user->getUsername() << "LEAVE ...\n";

    struct Response response;
    response.type = RESPONSE_LEAVE;
    response.status = SUCCESS;
    strcpy(response.message, "leave ...");
    return response;
}

struct Response choose(struct Request *request)
{
    // Push choice
    User *user = find_user(request->client_id, users);
    Game *game = find_game(user->game_id, games);
    int user_id = request->client_id;
    int choice = atoi(request->message);
    game->setChoice(user_id, choice);

    // Send response
    struct Response response;
    response.type = RESPONSE_CHOOSE;
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
    response.type = RESPONSE_READY;
    strcpy(response.message, "Ready ...");
    return response;
}

struct Response chat(struct Request *request)
{
    User *user = find_user(request->client_id, users);
    Game *game = find_game(user->game_id, games);
    cout << "User " << request->client_id << " : " << request->message << endl;
    for (int member_id : game->getMembers())
    {
        if (member_id != request->client_id)
        {
            cout << "Send : " << member_id << endl;
            user = find_user(member_id, users);
            struct Response mes;
            mes.status = SUCCESS;
            mes.type = RESPONSE_SEND_MESSAGE;
            strcpy(mes.message, user->getUsername());
            strcat(mes.message," ");
            strcat(mes.message, request->message);
            send(user->getClientSocket(), &mes, sizeof(struct Response), 0);
        }
    }
    Chat chat(request->client_id, game->getId(), request->message);
    chat.store();
    struct Response response;
    response.type = RESPONSE_CHAT;
    response.status = SUCCESS;
    strcpy(response.message, "Sent message !");
    return response;
}

struct Response start(struct Request *request)
{
    struct Response response;
    response.type = RESPONSE_START;
    int game_id = atoi(request->message);
    Game *game = find_game(game_id, games);
    game->addQuestions(1);
    game->addQuestions(2);
    game->order = 1;
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

struct Response end(struct Request *request)
{
    User *user = find_user(request->client_id,users);
    Game *game = find_game(user->game_id, games);
    game->status = GAME_END;
    game->store();

    for (int user_id : game->getMembers())
    {
        User *user = find_user(user_id, users);
        user->status = USER_ONLINE;
        if (user_id != request->client_id)
        {
            struct Response memberResponse;
            memberResponse.status = SUCCESS;
            memberResponse.type = RESPONSE_SEND_END;
            strcpy(memberResponse.message, "End game ...\n");
            send(user->getClientSocket(), &memberResponse, sizeof(struct Response), 0);
        }
    }
    // Send status to owner
    struct Response response;
    response.type = RESPONSE_END;
    response.status = SUCCESS;
    strcpy(response.message,"End game, owner leaving ...\n");
    return response;
}