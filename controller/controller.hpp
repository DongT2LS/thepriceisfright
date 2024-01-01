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
struct Response signup(struct Request *request);
struct Response login(struct Request *request);
struct Response logout(struct Request *request);

// gamecontroler

struct Response join(struct Request *request);
struct Response newroom(struct Request *request);
struct Response invite(struct Request *request);
struct Response leave(struct Request *request);
struct Response choose(struct Request *request);
struct Response ready(struct Request *request);
struct Response chat(struct Request *request);
struct Response start(struct Request *request);

// service
User* find_user(int _id, vector<User*> users);
void set_status_user(int _id,UserStatus status,vector<User*> users);
Game* find_game(int _id, vector<Game*> games);
void create_game(int owner_id);
void getUserDatabase();
void getGameDatabase();
void getQuestionDatabase();
void getChatDatabase();
#endif