#ifndef GAME_HPP
#define GAME_HPP

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
#include "user.hpp"
#include "question.hpp"
using namespace std;

enum GameStatus
{
    GAME_READY,
    GAME_INPROGRESS,
    GAME_END
};

class Game
{
private:
    int id;
    int owner_id;
    vector<int> members;
    vector<int> choices[20];
    vector<int> questions;
    vector<int> score;

public:
    GameStatus status;
    int turn;
    // Constructor
    Game() {}
    Game(int _id, int _owner_id)
        : id(_id), owner_id(_owner_id), status(GAME_READY), turn(0) , score(20,0)
    {
        members.push_back(_owner_id);
    }

    // Getter for id
    int getId() const
    {
        return id;
    }

    // Setter for id
    void setId(int newId)
    {
        id = newId;
    }

    // Getter for owner_id
    int getOwnerId() const
    {
        return owner_id;
    }

    // Setter for owner_id
    void setOwnerId(int newOwnerId)
    {
        owner_id = newOwnerId;
    }

    // Getter for members
    vector<int> getMembers()
    {
        return members;
    }

    // get index of member
    int getMemberIndex(int user_id)
    {
        for (int i = 0; i < members.size(); i++)
        {
            if (members[i] == user_id)
            {
                return i;
            }
        }
        return -1;
    }

    // Setter for members
    void addMembers(int member_id)
    {
        for (int member : members)
        {
            if (member == member_id)
            {
                cout << "User in this room" << endl;
            }
        }
        members.push_back(member_id);
    }

    // Getter for questions
    int getQuestion(int index)
    {
        cout << index << endl;
        return questions[index];
    }

    const vector<int> &getQuestions() const
    {
        return questions;
    }

    // Setter for questions
    void addQuestions(int newQuestions)
    {
        questions.push_back(newQuestions);
        cout << "New question : " << newQuestions << endl;
    }

    void setChoice(int user_id, int choice)
    {
        for (int i = 0; i < members.size(); i++)
        {
            if (members[i] == user_id)
            {
                choices[i].push_back(choice);
                cout << user_id << " choose " << choice << endl;
                break;
            }
        }
    }

    int getScore(int index)
    {
        return score[index];
    }

    void addScore(int user_id)
    {
        cout << " OKE " << endl;
        for(int i=0;i<members.size();i++)
        {
            cout << "OKE " <<  i << endl;
            cout << score[i] << endl;
            if(user_id == members[i])
            {
                ++score[i];
                return;
            }
        }
    }

    // int getWinner()
    // {

    // }

    void store()
    {
        FILE *file = fopen(GAME_DATABASE, "a");
        if (file == NULL)
        {
            printf("Can't open file : %s\n", GAME_DATABASE);
            return;
        }
        fprintf(file, "%d\n", id);
        for (int member_id : members)
        {
            fprintf(file, "%d ", member_id);
        }
        fprintf(file, "\n");
        for (int question : questions)
        {
            fprintf(file, "%d ", question);
        }
        fprintf(file, "\n");
        for (int i = 0; i < members.size(); i++)
        {
            fprintf(file, "%d ", members[i]);
            for (int choice : choices[i])
            {
                fprintf(file, "%d ", choice);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
};

#endif