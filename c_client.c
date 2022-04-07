#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


//function that deletes the newline character
char *NL(char *string)
{
    while(*string != '\0')
    {
        if(*string == '\n')
        {
            *string = '\0';
        }
        string++;
    }
    return string;
}

int main()
{
    int client_socket;
    char buffer[300];
    char name[330];
    int fd[2];
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_adress;

    //filling the infos of the server
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(9002);
    server_adress.sin_addr.s_addr = INADDR_ANY;//htonl("ip") for custom ip

    //sends a connect request to the server
    int connect_status = connect(client_socket, (struct sockaddr*)&server_adress, sizeof server_adress);

    //save the name of the user
    printf("Bitte geben sie ihren Namen ein: ");
    fgets(name, sizeof name, stdin);
    NL(name);

    //send name to the server
    send(client_socket, name, sizeof name, 0);

    //logical while loop, for input and sending
    while(1)
       {
            printf(": ");
            fgets(buffer, sizeof buffer, stdin);
            NL(buffer);

            if(strcmp(buffer, ":quit") == 0)
            {
                send(client_socket, buffer, sizeof buffer, 0);
                close(client_socket);
                exit(0);
            }
            send(client_socket, buffer, sizeof buffer, 0);
        }

    //you can see what close is doing:)
    close(client_socket);
    return 0;
}