#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include "controller/controller.hpp"
#include "model/request.hpp"
#include "model/user.hpp"
#include <vector>
#define MAX_CLIENTS 20

using namespace std;


// Tách nhánh để xử lý logic
void handle_request(struct Request *request)
{
    switch (request->type)
    {
    case SIGNUP:
        signup(request);
        break;
    case LOGIN:
        login(request);
        break;
    case LOGOUT:
        logout(request);
        break;
    case NEW_ROOM:
        newroom(request);
        break;
    case JOIN:
        join(request);
        break;
    case INVITE:
        invite(request);
        break;
    case LEAVE:
        leave(request);
        break;
    case CHOOSE:
        choose(request);
        break;
    case CHAT:
        chat(request);
        break;
    case READY:
        ready(request);
        break;
    case START:
        start(request);
        break;
    default:
        printf("Not found ! \n");
        break;
    }
}

// Xử lý request từ client
void *handle_client(void *socket_fd)
{
    int client_socket = *(int *)socket_fd;
    // cout << client_socket << " OKE" << endl;

    // char buffer[1024];
    size_t read_size;
    struct Request *request = (struct Request *)malloc(sizeof(struct Request));
    
    while ((read_size = recv(client_socket, request, sizeof(struct Request), 0)) > 0)
    {
        request->client_socket = client_socket;
        handle_request(request);
    }
    free(request);

    if (read_size == 0)
    {
        printf("Client disconnected: %d\n", client_socket);
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // Close the socket and free the thread's socket file descriptor
    close(client_socket);
    // free(socket_fd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // User::getListUser();   
    getUserDatabase();
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t thread_id;
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Could not create socket");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", atoi(argv[1]));

    // Accept incoming connections
    while (1)
    {
        // Nhận một kết nối với client mới
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("Accept failed");
            return 1;
        }
        printf("Connection accepted: %d\n", client_socket);

        // Tạo luồng mới
        if (pthread_create(&thread_id, NULL, handle_client, &client_socket) < 0)
        {
            perror("Could not create thread");
            return 1;
        }

        // Tách luồng để xử lý riêng biệt
        pthread_detach(thread_id);
    }

    close(server_socket);

    return 0;
}
