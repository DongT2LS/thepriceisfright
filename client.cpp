#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "model/request.hpp"
#include <stdio_ext.h>
#include <cstdio>
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Could not create socket");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server\n");
    int choice;
    while (1)
    {
        cout << "Nhap lua chon : ";
        cin >> choice;
        __fpurge(stdin);
        struct Request request;
        switch (choice)
        {
        case 1:
            request.client_id = 0;
            request.client_socket = 0;
            request.type = SIGNUP;
            cout << "Nhap tai khoan + mat khau : ";
            fgets(request.message,101,stdin);
            request.message[strlen(request.message)-1] = '\0';
            cout << request.message << endl;  
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "LOGIN ... " << endl;
            break;
        case 2:
            request.client_id = 0;
            request.client_socket = 0;
            request.type = LOGIN;
            cout << "Nhap tai khoan + mat khau : ";
            cin >> request.message; 
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "LOGIN ..." << endl;
            break;
        case 3:
            request.client_id = 3;
            request.client_socket = 0;
            request.type = SIGNUP;
            strcpy(request.message, "dong221122 22112002");
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << request.message << endl;
            break;
        case 4:
            request.client_id = 3;
            request.client_socket = 0;
            request.type = START;
            strcpy(request.message, "1");
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << request.message << endl;
            break;
        default:
            break;
        }
    }
    // Send and receive messages
    // while (1) {
    // printf("Enter message: ");
    // fgets(message, sizeof(message), stdin);
    // struct Request *request = malloc(100);
    // request->message = "Hello";
    // request->status = SIGNUP;
    // Send the message to the server

    // struct Request request2;
    // request2.client_id = 3;
    // request2.client_socket = 2;
    // request2.type = CHAT;
    // cout << "chat" <<endl;
    // send(client_socket, &request2, sizeof(struct Request), 0);
    // Receive the server's response
    // if (recv(client_socket, message, sizeof(message), 0) == 0) {
    //     printf("Server disconnected\n");
    //     break;
    // }

    // printf("Server response : OKE\n");
    // memset(message, 0, sizeof(message));
    // usleep(200000);
    // }

    close(client_socket);

    return 0;
}
