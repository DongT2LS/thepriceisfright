#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../model/game.hpp"
#include "../model/chat.hpp"
// global var
extern vector<Game*> games;
extern vector<User*> users;
extern vector<Question*> questions;
extern vector<Chat*> chats;
// usercontroller 
void signup(struct Request *request);
void login(struct Request *request);
void logout(struct Request *request);

// gamecontroler

void join(struct Request *request);
void newroom(struct Request *request);
void invite(struct Request *request);
void leave(struct Request *request);
void choose(struct Request *request);
void ready(struct Request *request);
void chat(struct Request *request);
void start(struct Request *request);

// service
User* find_user(int _id, vector<User*> users);
void set_status_user(int _id,UserStatus status,vector<User*> users);
Game* find_game(int _id, vector<Game*> games);
void create_game(int owner_id);
void getUserDatabase();
#endif