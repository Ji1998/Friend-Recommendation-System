// ECE450 main server
//10/05/2020
//compile: g++ -o excutable xxx.cpp
/**This version of mainserver will receive a string from client, send it to the backend server, then receive the results from backendserver
 * and then send the result to the client.
**/
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
#include <string>
#include<iostream>
#include <unordered_set>

using namespace :: std;

//MACROS
#define TCPPORT "33031"     //TCP port
//#define UDPPORT "32031"		//UDP port
#define HOST "localhost"
#define BACKLOG 10          //# of pending connecions queue

//two global variables
char PORTA[] = "30031";
char PORTB[] = "31031";

using namespace :: std;

//function declration
string reconmendation(char *dataIn, char ch,  size_t dataInSize);
string convertToString(char* a, int size);

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

int main()
{
    //build TCP connection from Beej's
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    unordered_set<string> countrysetA;
    unordered_set<string> countrysetB;
    char serverChoice;

    if ((rv = getaddrinfo(NULL, TCPPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
            == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
            == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo); // all done with this structure
    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
    //listen
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchld_handler; // reap all dead processes sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1); }
    printf( "The main server is up and running \n");

    //load balancing

    char countrylist[] = "countryList:";                                                                        //send countryList: to backend server
    size_t countylist_requst_size = sizeof(countrylist);
    string countryA = reconmendation(countrylist, 'A',countylist_requst_size);
    //string countryB = reconmendation(countrylist, 'B',countylist_requst_size);
    auto seperatorList = countryA.find(":");
    string listBeforeSep_A = countryA.substr(seperatorList+1);
    //string listBeforeSep_B = countryB.substr(seperatorList+1);
    string delimiter = "+";
    size_t posA = 0;
    size_t posB = 0;

    string tokenA;
    while ((posA = listBeforeSep_A.find(delimiter)) != string::npos) {
        tokenA = listBeforeSep_A.substr(0, posA);
        countrysetA.insert(tokenA);
        listBeforeSep_A.erase(0, posA + delimiter.length());
    }

//    string tokenB;
//    while ((posB = listBeforeSep_B.find(delimiter)) != string::npos) {
//        tokenB = listBeforeSep_B.substr(0, posB);
//        countrysetB.insert(tokenB);
//        listBeforeSep_A.erase(0, posB + delimiter.length());
//    }
    cout << "A contains: ";
    for(auto a : countrysetA) cout << a << " ";
    cout << endl;
    cout << "B contains: ";
    for(auto b : countrysetB) cout << b << " ";
    cout << endl;

    //accpet() new client
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);       //return socket descriptor of new socket
    if(new_fd == -1)
    {
        perror("accept error");
        return 1;
    }
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("server: got connection from %s\n", s);
    struct sockaddr_in clientInfo;                   //a struct saves the information of the other side
    memset(&clientInfo, 0, sizeof(clientInfo));
    int len = sizeof(clientInfo);
    getpeername(new_fd, (struct sockaddr *) &clientInfo, (socklen_t *) &len);   //return the information of the other side
    int clientPort = clientInfo.sin_port;

    while(1)
    {

        //receive data from the client and process it to grab country name
        char clientData [1000];
        memset(clientData, 0, sizeof(clientData));
        cout << clientData << " :clientData before recv" << endl;
        recv(new_fd, clientData, sizeof (clientData), 0);
        //size_t clientDataSize = sizeof clientData / sizeof (char);
        string clientDataStr = clientData;                                                                          //cast char array sent by client to string
        auto colonSperator = clientDataStr.find(":");                                                           //query:country_name+id;
        string afterQuery = clientDataStr.substr(colonSperator+1);                                             //country_name+id;
        auto plusSeprator = afterQuery.find("+");
        string countryQuery = afterQuery.substr(0,plusSeprator);                                                //country_name
        if(countrysetA.count(countryQuery)) serverChoice = 'A';
        else if(countrysetB.count(countryQuery)) serverChoice = 'B';
        else {
            string noCountryReply = countryQuery + " not found ";
            send(new_fd, noCountryReply.c_str(),noCountryReply.length(),0);
            continue;
        }
        cout << clientData << " :clientData" << endl;

        //do the recommendation
        string resultA = reconmendation(clientData,serverChoice, sizeof(clientData));
        cout << resultA <<" :resultA" << endl;
        auto it = resultA.find(":");
        string protocol = resultA.substr(0, it);
        string messageAfterProtocol = resultA.substr(it+1);
//        if(protocol == "replyList")                                                                               //splitof(+) between country names
//        {
//            string delimiter = "+";
//            size_t pos = 0;
//            string token;
//            while ((pos = messageAfterProtocol.find(delimiter)) != string::npos) {
//                token = messageAfterProtocol.substr(0, pos);
//                cout << token << " ";
//                messageAfterProtocol.erase(0, pos + delimiter.length());
//            }
//            cout << messageAfterProtocol << std::endl;
//        }
        if(protocol == "replyFriendInfo")
        {
            cout << resultA.c_str() << " :resultA.c_str()" << endl;

            cout << send(new_fd, resultA.c_str(), resultA.length(), 0) << endl;
        }
        cout << "mainserver has send the data to client"<<endl;

        //send hello world to the client
//        if (!fork()) { // this is the child process close(sockfd);
//           close(sockfd); // child doesn't need listener
//           if(send(new_fd, "hello, world!", 13, 0) == -1) perror("send error");
//           close(new_fd);
//           exit(0);
//        }
//        close(new_fd); // parent doesn't need this
    }
    close(new_fd);
    return 0;

}
string reconmendation(char dataIn[], char ch, size_t dataInSize)
{
    int UDPsock;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char* backendServerPort;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;

    if(ch == 'A')  backendServerPort = PORTA;
    else if(ch == 'B') backendServerPort = PORTB;

    cout <<backendServerPort << endl;
    //setup UDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(HOST, backendServerPort, &hints, &servinfo))
        != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((UDPsock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
            == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        exit(2);
    }
    cout <<  dataInSize << endl;
    cout << sendto(UDPsock, dataIn, dataInSize, 0, p->ai_addr, p->ai_addrlen) << endl;
    char result[1000];
    memset(result, 0, sizeof(result));
    addr_len = sizeof their_addr;
    recvfrom(UDPsock, result, sizeof result, 0, (struct sockaddr *) &their_addr, &addr_len);
    cout << result << endl;
    return result;
}
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++)
    {
        s = s + a[i];
    }
    return s;
}

























