#include <string>
#include <iostream>
#include <cstring>
#include "../config/config.hpp"
using namespace std;

class Question
{
private:
    int id;
    char question[300];
    char answers[4][50];
    int true_answer;

public:
    Question(int _id, char *_question, char *_answers[4], int _true_answer)
        : id(_id), true_answer(_true_answer)
    {
        // Sử dụng strncpy để tránh tràn bộ nhớ cho câu hỏi
        strncpy(question, _question, sizeof(question) - 1);
        question[sizeof(question) - 1] = '\0'; // Đảm bảo kết thúc chuỗi

        // Sử dụng strncpy để tránh tràn bộ nhớ cho câu trả lời
        for (int i = 0; i < 4; ++i)
        {
            strncpy(answers[i], _answers[i], sizeof(answers[i]) - 1);
            answers[i][sizeof(answers[i]) - 1] = '\0'; // Đảm bảo kết thúc chuỗi
        }
    }

    ~Question() {}

    void store()
    {
        FILE *file = fopen(QUESTION_DATABASE, "a");
        if (file == NULL)
        {
            printf("Can't open file : %s\n", QUESTION_DATABASE);
            return;
        }
        fprintf(file, "%d %d %s %s %s %s %s\n", id, true_answer, question, answers[0], answers[1], answers[2], answers[3]);
        fclose(file);
    }

    char *getQuestion()
    {
        return question;
    }

    char (*getAnswers())[50]
    {
        return answers;
    }

    int getId()
    {
        return id;
    }

    int getTrueAnswer()
    {
        return true_answer;
    }
};