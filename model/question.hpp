#include <string>
#include <iostream>
#include <cstring>
using namespace std;

class Question
{
private:
    int id;
    char question[300];
    char answers[4][300];
    int true_answer;

public:
    Question(int _id, const char *_question, const char *_answers[4], int _true_answer)
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
};