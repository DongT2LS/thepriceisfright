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
        cout << "Turn : " << game->turn <<endl;
        sleep(5);
    }
    
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
            cout << "OKE !!!! " << question_id  << endl;
            if (answer == ques->getTrueAnswer())
            {
                return 1; // Cau tra loi dung tra ve 1
            }
            return 0;
        }
    }
    return 0;
}