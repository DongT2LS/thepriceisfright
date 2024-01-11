#include "controller.hpp"
#include "../config/config.hpp"
#include "../model/response.hpp"
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <unistd.h>
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

Question *find_question(int _id, vector<Question *> questions)
{
    for (Question *question : questions)
    {
        if (question->getId() == _id)
        {
            return question;
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

void *handle_question(void *id)
{
    // Tách luồng để xử lý riêng biệt
    pthread_detach(pthread_self());
    int client_id = *(int *)id;
    cout << "Client id : " << client_id << endl;
    User *user = find_user(client_id, users);
    Game *game = find_game(user->game_id, games);

    // Sau 2s sẽ bắt đầu gửi câu hỏi
    sleep(2);
    for (int question_id : game->getQuestions())
    {
        Question *question = find_question(question_id, questions);
        cout << question->getQuestion() << " : " << question->getAnswers()[0] << " " << question->getAnswers()[1] << " " << question->getAnswers()[2] << " " << question->getAnswers()[3] << endl;
        for (int user_id : game->getMembers())
        {
            User *member = find_user(user_id, users);
            struct Response sendQuestion;
            sendQuestion.status = SUCCESS;
            sendQuestion.type = RESPONSE_SEND_QUESTION;
            strcpy(sendQuestion.message, question->getQuestion());
            strcat(sendQuestion.message, " ");
            strcat(sendQuestion.message, question->getAnswers()[0]);
            strcat(sendQuestion.message, " ");
            strcat(sendQuestion.message, question->getAnswers()[1]);
            strcat(sendQuestion.message, " ");
            strcat(sendQuestion.message, question->getAnswers()[2]);
            strcat(sendQuestion.message, " ");
            strcat(sendQuestion.message, question->getAnswers()[3]);

            send(member->getClientSocket(), &sendQuestion, sizeof(struct Response), 0);
        }
        game->turn++;
        cout << "Turn : " << game->turn << endl;

        // 5s sẽ gửi 1 câu hỏi mới và tăng turn lên
        sleep(5);
    }
    int max_score = 0;
    for (int i = 0; i < game->getMembers().size(); i++)
    {
        if (game->getScore(i) > max_score)
        {
            max_score = game->getScore(i);
        }
    }
    for (int i = 0; i < game->getMembers().size(); i++)
    {
        int user_id = game->getMembers()[i];
        User *member = find_user(user_id, users);
        game->status = GAME_END;
        game->store();

        member->status = USER_ONLINE;
        struct Response sendEndGame;
        sendEndGame.status = SUCCESS;
        sendEndGame.type = RESPONSE_END;
        if (game->getScore(i) == max_score)
        {
            strcpy(sendEndGame.message, "Winner \n");
        }
        else
        {
            strcpy(sendEndGame.message, "Loser \n");
        }
        send(member->getClientSocket(), &sendEndGame, sizeof(struct Response), 0);
    }
    sleep(0.5);
    update_list_ready_room();
    pthread_exit(0);
}

void detachGame(int client_id, pthread_t thread_id)
{
    // Tạo luồng mới
    if (pthread_create(&thread_id, NULL, handle_question, &client_id) < 0)
    {
        perror("Could not create thread");
        return;
    }
}

// Cập nhật câu trả lời của người dùng , nếu đúng + 1 điểm , sai thì ko sao
int checkAnswer(int user_id, int question_id, int answer)
{
    User *user = find_user(user_id, users);
    Game *game = find_game(user->game_id, games);
    for (int i = 0; i < game->getMembers().size(); i++)
    {
        if (game->getMembers()[i] == user_id)
        {
            Question *ques = find_question(question_id, questions);
            cout << "OKE !!!! " << question_id << endl;
            if (answer == ques->getTrueAnswer())
            {
                return 1; // Cau tra loi dung tra ve 1
            }
            return 0;
        }
    }
    return 0;
}

void update_list_online_user(int client_id)
{
    struct Response response;
    response.status = SUCCESS;
    response.type = RESPONSE_ONLINE_USER;
    strcpy(response.message, "");
    for (User *user : users)
    {
        if (user->status != USER_OFFLINE)
        {
            char user_id[5];
            sprintf(user_id, "%d", user->getId());
            strcat(response.message, user_id);
            strcat(response.message, " ");
            strcat(response.message, user->getUsername());
            strcat(response.message, " ");
        }
    }
    for (User *user : users)
    {
        if (user->status != USER_OFFLINE && user->getId() != client_id)
        {
            send(user->getClientSocket(), &response, sizeof(struct Response), 0);
        }
    }
}

void update_list_ready_room()
{
    struct Response response;
    response.status = SUCCESS;
    response.type = RESPONSE_READY_ROOM;
    strcpy(response.message,"");
    for (Game *game : games)
    {
        if (game->status == GAME_READY)
        {
            char game_id[5];
            sprintf(game_id, "%d", game->getId());
            strcat(response.message, game_id);
            strcat(response.message, " ");
        }
    }
    for (User *user : users)
    {
        if (user->status == USER_ONLINE)
        {
            send(user->getClientSocket(), &response, sizeof(struct Response), 0);
        }
    }
}

void get_ready_game(int client_socket)
{
    struct Response response;
    response.status = SUCCESS;
    response.type = RESPONSE_READY_ROOM;
    strcpy(response.message, "");

    for (Game *game : games)
    {
        if (game->status = GAME_READY)
        {
            char game_id[5];
            sprintf(game_id, "%d", game->getId());
            strcat(response.message, game_id);
            strcat(response.message, " ");
        }
    }

    send(client_socket, &response, sizeof(struct Response), 0);
}