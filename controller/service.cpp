#include "controller.hpp"
#include "../config/config.hpp"

vector<Game *> games;
vector<User *> users;

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

void getGameFromDatabase()
{
    FILE *file = fopen(GAME_DATABASE, "r");

    if (file == NULL)
    {
        printf("Can't open file : %s\n", GAME_DATABASE);
        return;
    }

    Game game();
    while (feof(file))
    {
        /* code */
    }

    fclose(file);
}

void create_game(int owner_id)
{
    int game_id = games.size() + 1;
    cout << "Game : " << game_id << "owner_id :" << owner_id << endl;
    User *user = find_user(owner_id,users);
    user->status = USER_READY;
    user->game_id = game_id;
    games.push_back(new Game(game_id, owner_id));
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
    
    while (fscanf(file, "%d %s %s", &id, username, password) == 3) // Nếu đọc đủ 3 trường của 1 account thì tiếp tục đọc
    {

        users.push_back(new User(id, username, password));
        // delete(user);
    }
    cout << "Chan vcl" << endl;
    fclose(file);
}

