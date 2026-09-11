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

    send(client_socket, "PASS 1234\r\n", 13, 0);
    send(client_socket, "PASS 1234\r\n", 11, 0);
    send(client_socket, "NICK bot\r\n", 10, 0);
    send(client_socket, "USER bot 0 * :Bot\r\n", 19, 0);
    send(client_socket, "JOIN #test\r\n", 12, 0);
    /*
    while()
    {
        to do ;
    } 
    */

    close(client_socket);
    return (0);
}