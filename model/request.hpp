#include<stdio.h>
#include<string.h>
#include<iostream>
#include<string>

enum REQUEST_TYPE {
    SIGNUP = 0,
    LOGIN = 1, 
    LOGOUT = 2,
    NEW_ROOM = 3,
    JOIN = 4,
    INVITE = 5,
    LEAVE = 6,
    CHOOSE = 7,
    CHAT = 8,
    READY = 9,
    START = 10,
    END = 11,
    REPLAY = 12,
    GET_ONLINE_USER = 13,
    GET_READY_ROOM
};

struct Request {
    REQUEST_TYPE type;
    int client_socket;
    int client_id;
    char message[500];
};
