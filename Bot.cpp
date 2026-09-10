#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(6667);
    server_address.sin_addr.s_addr = INADDR_ANY;

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    const char* msg = "test";
    send(client_socket, msg, strlen(msg), 0);

    close(client_socket);
    return (0);
}