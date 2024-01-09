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
struct Response end(struct Request *request);
struct Response replay(struct Request *request);
struct Response getOnlineUser(struct Request *request);
struct Response getReadyRoom(struct Request *request);
// service
User* find_user(int _id, vector<User*> users);
void set_status_user(int _id,UserStatus status,vector<User*> users);
Game* find_game(int _id, vector<Game*> games);
void create_game(int owner_id);
void getUserDatabase();
void getGameDatabase();
void getQuestionDatabase();
void getChatDatabase();
void detachGame(int client_socket,pthread_t thread_id);
int checkAnswer(int user_id , int question_id , int answer);
void update_list_online_user(int client_id);
void update_list_ready_room();
void get_ready_game(int client_socket);
#endif