#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

int main()
{
    //defines all uses variables
    char buffer[300];
    char name[30];
    char dc[10];
    int counter = 0;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_adress;

    //fills the infos of the server
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(9002);
    server_adress.sin_addr.s_addr = INADDR_ANY;
    int addrlen = sizeof(server_adress);

    //binds the socket
    bind(server_socket, (struct sockaddr *)&server_adress, sizeof server_adress);

    //listen for incoming connect requests(max 5)
    //change the 5 to whatever you want
    listen(server_socket, 5);
    int conn;


    //logical while loop, that everytime a new connection is coming forks itself
    while(conn = accept(server_socket, (struct sockaddr *)&server_adress, (socklen_t*)&addrlen))
    {
        int id;
        if((id = fork()) == 0)
        {
            recv(conn, name, sizeof name, 0);            

            //print the name that connectet to the server
            printf("%s connected to the server\n", name);
            send(conn, name, sizeof name, 0);

            
            while(recv(conn, buffer, sizeof buffer, 0))
            {
                counter++;
                if(counter > 1)
                {
                    //if the message is ":quit", than end the prozess and send the disconnect message to the server
                    if(strcmp(buffer, ":quit") == 0)
                    {
                        printf("%s disconnected\n", name);
                        close(server_socket);
                        return 0;
                    }
                    printf("%s: %s\n", name, buffer);
                }
            }
        }
    }
    

    close(server_socket);
    return 0;
}