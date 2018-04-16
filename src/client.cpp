#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        perror("Invalid arguments");
        return 2;
    }

    int port = atoi(argv[2]);
    if(port <= 0)
    {
        perror("Invalid port");
        return 2;
    }
   
    int sock; 
    struct addrinfo hints = addrinfo();
    struct addrinfo *result, *rp;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;

    int s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if(s != 0)
    {
        fprintf(stderr, "Error: %s\n", gai_strerror(s));
        return 1;
    }

    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock == -1)
        {
            continue;
        }

        if(connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            break;
        }

        close(sock);
    }

    if(rp == NULL)
    {
        fprintf(stderr, "Could not connect.\n");
        return 1;
    }

    freeaddrinfo(result);

    char buf[1024];
    while(fgets(buf, 1024, stdin))
    {
        if(strncmp(buf, "quit;", 5) == 0)
        {
            close(sock);
            return 0;
        }
        size_t len = strlen(buf) + 1;

        if(write(sock, buf, len) != (ssize_t)len)
        {
            perror("IO error");
            close(sock);
            return 1;
        }

        if(strncmp(buf, "stop;", 5) == 0)
        {
            close(sock);
            return 0;
        }

        for(ssize_t nread = read(sock, buf, 1024); nread; nread = read(sock, buf, 1024))
        {
            buf[1023] = '\0';
            buf[nread] = '\0';
            if(nread == -1)
            {
                perror("Read");
                close(sock);
                return 1;
            }

            printf("%s", buf);
        }
    }
    close(sock);
    return 0;
}

