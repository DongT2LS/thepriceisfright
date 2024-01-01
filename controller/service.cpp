#include "controller.hpp"
#include "../config/config.hpp"

vector<Game *> games;
vector<User *> users;
vector<Question *> questions;
vector<Chat *> chats;
User *find_user(int _id, vector<User *> users)
{
    for (User *user : users)
    {
        if (user->getId() == _id)
        {
            return user;
        }
    }
    return nullptr;
}

void set_status_user(int _id, UserStatus status, vector<User *> users)
{
    for (User *user : users)
    {
        if (user->getId() == _id)
        {
            user->status = status;
        }
    }
}

Game *find_game(int _id, vector<Game *> games)
{
    for (Game *game : games)
    {
        if (game->getId() == _id)
        {
            cout << game->getId() << " " << game->getOwnerId() << endl;
            return game;
        }
    }
    return nullptr;
}

void create_game(int owner_id)
{
    int game_id = games.size() + 1;
    cout << "Game : " << game_id << "owner_id :" << owner_id << endl;
    User *user = find_user(owner_id, users);
    user->status = USER_READY;
    user->game_id = game_id;
    games.push_back(new Game(game_id, owner_id));
}

void getGameDatabase()
{
    FILE *file = fopen(GAME_DATABASE, "r");

    if (file == NULL)
    {
        printf("Can't open file : %s\n", GAME_DATABASE);
        return;
    }

    Game *game = new Game();
    while (feof(file))
    {
        /* code */
    }

    fclose(file);
}

void getUserDatabase()
{
    FILE *file = fopen(USER_DATABASE, "r");

    if (file == NULL)
    {
        printf("Can't open file : %s\n", USER_DATABASE);
        return;
    }
    int id;
    char username[50];
    char password[50];

    while (fscanf(file, "%d %s %s", &id, username, password) == 3)
    {
        users.push_back(new User(id, username, password));
    }
    fclose(file);
}

void getQuestionDatabase()
{
    FILE *file = fopen(QUESTION_DATABASE, "r");

    if (file == NULL)
    {
        printf("Can't open file : %s\n", QUESTION_DATABASE);
        return;
    }
    int id;
    int true_answer;
    char question[300];
    char *answers[4];

    while (fscanf(file, "%d %d %s %s %s %s %s\n", &id, &true_answer, question, answers[0], answers[1], answers[2], answers[3]) == 7)
    {
        questions.push_back(new Question(id, question, answers, true_answer));
    }
    fclose(file);
}
