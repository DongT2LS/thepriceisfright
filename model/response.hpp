#include<bits/stdc++.h>

using namespace std;

enum RESPONSE_TYPE 
{
    ERROR,
    SUCCESS,
};

struct Response
{
    RESPONSE_TYPE status;
    char message[500];
};
