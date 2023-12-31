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

class Game {
private:
    int id;
    int owner_id;
    vector<int> members;
    vector<vector<int>> chooses;
    vector<Question> questions;
    
public:
    GameStatus status;
    // Constructor
    Game(){}
    Game(int _id,int _owner_id)
        : id(_id), owner_id(_owner_id) , status(GAME_READY)
    {
        members.push_back(owner_id);
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
    const std::vector<int>& getMembers()
    {
        return members;
    }

    // Setter for members
    void addMembers(int member_id)
    {
        for(int i=0;i<20;i++)
        {
            if(members[i] == member_id)
            {
                cout << "User is in this room " << endl;
                break;
            }
            if(members[i] == 0){
                members[i] = member_id;
                break;
            }
        }
    }

    // Getter for questions
    Question getQuestions(int index)
    {
        return questions[index];
    }

    // Setter for questions
    void addQuestions(Question newQuestions)
    {
        questions.push_back(newQuestions);
    }

    void store()
    {
        
    }


};

#endif