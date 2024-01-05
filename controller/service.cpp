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
    char answers[4][300];
    while (fscanf(file, "%d %d %s %s %s %s %s\n", &id, &true_answer, question, answers[0], answers[1], answers[2], answers[3]) == 7)
    {
        char *answerPointers[4];
        for (int i = 0; i < 4; ++i)
        {
            answerPointers[i] = answers[i];
        }
        questions.push_back(new Question(id, question, answerPointers, true_answer));
    }
    fclose(file);
}

void getChatDatabase()
{
    FILE *file = fopen(CHAT_DATABASE, "r");

    if (file == NULL)
    {
        printf("Can't open file : %s\n", CHAT_DATABASE);
        return;
    }
    int game_id;
    int user_id;
    char message[300];
    while (fscanf(file, "%d %d %299[^\n]", &user_id, &game_id, message) == 3)
    {
        chats.push_back(new Chat(user_id, game_id, message));
    }
    fclose(file);
}

void *handle_question(void *socket_fd)
{
    int client_socket = *(int *)socket_fd;
}

void sendQuestion(int *client_socket)
{
    pthread_t thread_id;
    // Tạo luồng mới
    if (pthread_create(&thread_id, NULL, handle_question, &client_socket) < 0)
    {
        perror("Could not create thread");
        return;
    }

    // Tách luồng để xử lý riêng biệt
    pthread_detach(thread_id);
}

