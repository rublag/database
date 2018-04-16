#include "lexer.h"
#include "parser.h"
#include "engine.h"
#include "database.h"
#include <iostream>
#include <string>
#include <sstream>

#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    std::stringstream iss;
    std::stringstream oss;
    Lexer l(iss);
    Parser p(l);
    std::ifstream istr;
    istr.open(argv[1], std::ifstream::in);
    Database db(istr);
    Engine eng(db, oss);
    Command c = {};

    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s read_file port\n", argv[0]);
        return 2;
    }

    struct addrinfo hints = addrinfo();
    struct addrinfo *result, *rp;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int s = getaddrinfo(NULL, argv[2], &hints, &result);
    if(s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return 1;
    }

    int sock;
    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock == -1)
            continue;


        if(bind(sock, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }

        close(sock);
        sock = -1;
    }

    if(rp == NULL)
    {
        fprintf(stderr, "Could not bind.\n");
        close(sock);
        return 1;
    }

    freeaddrinfo(result);

//    if(listen(sock, 1) != 0)
//    {
//        perror("Failed to listen");
//        return 1;
//    }

    while(true)
    {
//        socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
//        struct sockaddr_storage peer_addr;
//        int client = accept(sock, (struct sockaddr*) &peer_addr, &peer_addr_len);
//        if(client == -1)
//        {
//            perror("failed to accept");
//            continue;
//        }

        char buf[1024] = "";
        while(true)
        {
            socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
            struct sockaddr_storage peer_addr;
            ssize_t nread = recvfrom(sock, buf, 1024, 0, (struct sockaddr*) &peer_addr, &peer_addr_len);
            if(nread == -1)
            {
                continue;
            }

            iss << buf;
            c = p.parseStatement();
            if(c.type == Command::ErrParser)
            {
                std::cout << "  Invalid command!" << std::endl;
            }
            else if(c.type == Command::Stop)
            {
                close(sock);
                return 0;
            }
            else if(c.type == Command::Quit)
            {
                break;
            }
            else
                eng.runStatement(c);
            if(iss.eof())
                iss.clear();

            char host[NI_MAXHOST], service[NI_MAXSERV];

            int s = getnameinfo((struct sockaddr*) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
            if(s != 0)
            {
                fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
            }

            const std::string &tmp = oss.str();
            const char *str = tmp.c_str();
            size_t len = strlen(str);
            while(len)
            {
                const static size_t msl = 1020;
                size_t sl = 0;
                if(len > msl)
                {
                    sl = msl;
                    len -= sl;
                }
                else
                {
                    sl = len;
                    len -= sl;
                }

                if(sendto(sock, str, sl, 0, (struct sockaddr*) &peer_addr, peer_addr_len) != (ssize_t)sl)
                {
                    fprintf(stderr, "Error sending response\n");
                }
            }
            if(sendto(sock, str, 0, 0, (struct sockaddr*) &peer_addr, peer_addr_len) != 0)
            {
                fprintf(stderr, "Error sending response\n");
            }
            oss.str("");
            iss.str("");
        }
    }
    close(sock);
    return 0;
}
