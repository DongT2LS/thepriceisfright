#include<bits/stdc++.h>

using namespace std;

enum RESPONSE_STATUS
{
    ERROR,
    SUCCESS,
};

enum RESPONSE_TYPE
{
    RESPONSE_SIGNUP,
    RESPONSE_LOGIN, 
    RESPONSE_LOGOUT,
    RESPONSE_NEW_ROOM,
    RESPONSE_JOIN,
    RESPONSE_INVITE,
    RESPONSE_LEAVE,
    RESPONSE_CHOOSE,
    RESPONSE_CHAT,
    RESPONSE_READY,
    RESPONSE_START,
    RESPONSE_SEND_MESSAGE
};

struct Response
{
    RESPONSE_STATUS status;
    RESPONSE_TYPE type;
    char message[500];
};
