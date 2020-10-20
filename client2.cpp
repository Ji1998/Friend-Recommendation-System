// ECE450 clientA
//10/05/2020
//compile: g++ -o excutable xxx.cpp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include<iostream>
//macros
#define TCPPORT "33031"     //TCP port
#define MAXDATASIZE 1000     //define maximum # of bytes we can get at once
#define HOST "localhost"
using namespace :: std;
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char* argv[])        //argc: number of argvs; argv: vector of arguments passed into main
{
    cout << "EE450 Project: Guangsen Ji @2020 Copyright Reserved" << endl;
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

//    if (argc != 2) {
//        fprintf(stderr,"usage: client hostname\n"); exit(1);
//    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    if ((rv = getaddrinfo(HOST, TCPPORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket"); continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {

            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if(p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
     cout << "Clinet is up and running" << endl;
    freeaddrinfo(servinfo); // all done with this structure

    //send data to mainserver
    while(1)
    {

        string inputName;
        string inputContry;
        cout << "please enter your user ID: " << endl;
        cin >> inputName;
        cout << "please enter your country name: " << endl;
        cin >> inputContry;
        string protocol = "client2=query:";
        string sendMessage = protocol + inputContry + "+" + inputName;
        //char sendBuf[100] = "query:jZbO+108";
        // cout << sendBuf << endl;
        send(sockfd, sendMessage.c_str(), sendMessage.length(), 0);
        cout << "Client1 has sent " << inputName << " and " << inputContry << " to main server using TCP" << endl;
        char result[1000];
        memset(result, 0, sizeof(result));
        recv(sockfd, result, sizeof result, 0);
        string resultStr = result;
        auto seperator = resultStr.find(":");                                                           //replyFriendInfo:625 not_country no_id
        auto afterSeprator = resultStr.substr(seperator+1);
        if(afterSeprator == "not_country") cout << inputContry << " not found " << endl;
        else if(afterSeprator == "no_id") cout << "User " << inputName << " not found" << endl;
        else if(afterSeprator == "no_country") cout << inputContry << " not found" << endl;
        else
        {
            cout << "client2 has received results from Main Server: " << endl;
            cout << afterSeprator << " is possible friend of " << inputName << " in " << inputContry << endl;
        }

    }

}

