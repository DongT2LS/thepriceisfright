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
#include <pthread.h>
#include "model/response.hpp"
#define BUFFER_SIZE 1024

using namespace std;

int socket_id;

void *receive_messages(void *arg)
{
    int socket = *((int *)arg);
    char buffer[BUFFER_SIZE];
    int bytes_received;
   
    while (1)
    {
        struct Response *response = (struct Response *)malloc(sizeof(struct Response));
        bytes_received = recv(socket, response, BUFFER_SIZE, 0);
        if (bytes_received <= 0)
        {
            printf("Server disconnected.\n");
            close(socket);
            exit(EXIT_SUCCESS);
        }
        cout << "Received from server: " << response->message << endl;
        if(response->type == RESPONSE_LOGIN)
        {
            socket_id = atoi(response->message);
        }
        free(response);
    }
}

int main(int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];
    pthread_t thread_id;
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

    // Tạo một thread để nhận tin nhắn từ server
    if (pthread_create(&thread_id, NULL, receive_messages, &client_socket) != 0)
    {
        perror("Error creating thread");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    int choice;
    while (1)
    {
        __fpurge(stdin);
        cout << "Nhap lua chon : ";
        cin >> choice;
        __fpurge(stdin);
        struct Request request;
        request.client_id = socket_id;
        cout << "Client_id : " << request.client_id << endl;
        switch (choice)
        {
        case 1:
            request.client_socket = 0;
            request.type = SIGNUP;
            cout << "Nhap tai khoan + mat khau : ";
            fgets(request.message,100,stdin);
            request.message[strlen(request.message)-1] = '\0';
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "SIGNUP ... " << endl;
            break;
        case 2:
            request.client_socket = 0;
            request.type = LOGIN;
            cout << "Nhap tai khoan + mat khau : ";
            fgets(request.message,100,stdin);
            request.message[strlen(request.message)-1] = '\0';
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "LOGIN ..." << endl;
            break;
        case 3:
            request.client_socket = 0;
            request.type = SIGNUP;
            cout << "Nhap tai khoan + mat khau : ";
            fgets(request.message,100,stdin);
            request.message[strlen(request.message)-1] = '\0';
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "SIGN UP" << endl;
            break;
        case 4:
            request.client_socket = 0;
            request.type = CHAT;
            cout << "Nhap tin nhan : ";
            fgets(request.message,500,stdin);
            request.message[strlen(request.message)-1] = '\0';
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "CHAT ..." << endl;
            break;
        case 5:
            request.client_socket = 0;
            request.type = START;
            strcpy(request.message, "1");
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "START ..." << endl;
            break;
        case 6:
            request.client_socket = 0;
            request.type = LOGOUT;
            strcpy(request.message, "");
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "LOG OUT ..." << endl;
            break;
        case 7:
            request.client_socket = 0;
            request.type = NEW_ROOM;
            strcpy(request.message, "");
            send(client_socket, &request, sizeof(struct Request), 0);
            cout << "NEW ROOM ..." << endl;
            break;
        case 8:
            request.client_socket = 0;
            request.type = JOIN;
            cout << "Nhap id : " ;
            cin >> request.message;
            send(client_socket, &request, sizeof(struct Request), 0);
            break;
        case 9:
            request.client_socket = 0;
            request.type = INVITE;
            cout << "Nhap game_id + user_id: " ;
            fgets(request.message,500,stdin);
            request.message[strlen(request.message)-1] = '\0';
            send(client_socket, &request, sizeof(struct Request), 0);
            break;
        default:
            break;
        }
    }

    close(client_socket);

    return 0;
}
