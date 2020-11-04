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
#define UDPPORT_MAIN "32031"		//UDP port
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
    //bind UDP port 32031 from BeeJ's
    int UDPsockfd;
    struct addrinfo UDPhints, *UDPserverinfo, *UDPp;
    int UDPrv;
    struct sockaddr_storage UDPtheir_addr;
    socklen_t UDPaddr_len;

    //set up UDP
    memset(&UDPhints, 0, sizeof UDPhints);
    UDPhints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    UDPhints.ai_socktype = SOCK_DGRAM; // UDP dgram sockets
    UDPhints.ai_flags = AI_PASSIVE; // use my IP

    if ((UDPrv = getaddrinfo(NULL, UDPPORT_MAIN, &UDPhints, &UDPserverinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(UDPrv));
        return 1;
    }

    //loop through all structs to find the one can be connected
    for (UDPp = UDPserverinfo; UDPp != NULL; UDPp = UDPp->ai_next) {
        if ((UDPsockfd = socket(UDPp->ai_family, UDPp->ai_socktype, UDPp->ai_protocol))
            == -1) {
            perror("serverA: socket");
            continue;
        }
        if (::bind(UDPsockfd, UDPp->ai_addr, UDPp->ai_addrlen) == -1) {
            close(UDPsockfd);
            perror("serverA: bind");
            continue;
        }
        break;
    }
    if (UDPp == NULL) {
        fprintf(stderr, "serverA: failed to bind socket\n");
        return 2;
    }
    freeaddrinfo(UDPserverinfo);
    /*----------------------------------------------------------------------------------*/

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
    cout <<  "The main server is up and running " << endl;

    //load balancing

    char countrylist[] = "countryList:";                                                                        //send countryList: to backend server
    size_t countylist_requst_size = sizeof(countrylist);
    string countryA = reconmendation(countrylist, 'A',countylist_requst_size);
    string countryB = reconmendation(countrylist, 'B',countylist_requst_size);
    auto seperatorList = countryA.find(":");
    string listBeforeSep_A = countryA.substr(seperatorList+1);
    string listBeforeSep_B = countryB.substr(seperatorList+1);
    string delimiter = "+";
    size_t posA = 0;
    size_t posB = 0;

    string tokenA;
    cout << "The main server has received the country list from server A using UDP over port 32031" << endl;
    while ((posA = listBeforeSep_A.find(delimiter)) != string::npos) {
        tokenA = listBeforeSep_A.substr(0, posA);
        countrysetA.insert(tokenA);
        listBeforeSep_A.erase(0, posA + delimiter.length());
    }

    string tokenB;
    cout << "The main server has received the country list from server B using UDP over port 32031" << endl;
    while ((posB = listBeforeSep_B.find(delimiter)) != string::npos) {
        tokenB = listBeforeSep_B.substr(0, posB);
        countrysetB.insert(tokenB);
        listBeforeSep_B.erase(0, posB + delimiter.length());
    }
    cout << "server A contains: ";
    for(auto a : countrysetA) cout << a << " ";
    cout << endl;
    cout << "server B contains: ";
    for(auto b : countrysetB) cout << b << " ";
    cout << endl;

    // reap all dead processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    printf("server: waiting for connections...\n");

    //accpet() new client
    while(1)
    {
        sin_size = sizeof their_addr;
        memset(&their_addr, 0, sin_size);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);       //return socket descriptor of new socket
        if(new_fd == -1)
        {
            perror("accept error");
            return 1;
        }

        // enter child process
        if(!fork())
        {
            cout << "enter child process: " << new_fd << endl;
            while(1)
            {

                //receive data from the client and process it to grab country name
                char clientData [1000];
                memset(clientData, 0, sizeof(clientData));
                if (recv(new_fd, clientData, sizeof (clientData), 0) <= 0) {
                    cout << "recv end\n";
                    break;
                }
                //size_t clientDataSize = sizeof clientData / sizeof (char);
                string clientDataStr_cliendid = clientData;
                auto clientid_sep = clientDataStr_cliendid.find("=");
                string clientID = clientDataStr_cliendid.substr(0, clientid_sep);
                string clientDataStr = clientDataStr_cliendid.substr(clientid_sep+1);                                 //cast char array sent by client to string
                auto colonSperator = clientDataStr.find(":");                                                           //query:country_name+id;
                string afterQuery = clientDataStr.substr(colonSperator+1);                                             //country_name+id;
                auto plusSeprator = afterQuery.find("+");
                string countryQuery = afterQuery.substr(0,plusSeprator);                                                //country_name
                string useridQueery = afterQuery.substr(plusSeprator+1);                                                //id
                cout << "The Main server has received the request on User: " << useridQueery << " in " << countryQuery << endl;
                if(countrysetA.count(countryQuery))
                {
                    serverChoice = 'A';
                    cout << "country name" << countryQuery << " show up in server A" << endl;
                }
                else if(countrysetB.count(countryQuery))
                {
                    serverChoice = 'B';
                    cout << "country name" << countryQuery << " show up in server B" << endl;
                }
                else {
                    string noCountryReply = "replyFriendInfo:not_country";
                    cout << "Country name doesn't show up" << endl;
                    cout << "the main server has sent " << countryQuery << " :NOT FOUND" << " to " << clientID << " using TCP over port 33031" << endl;
                    send(new_fd, noCountryReply.c_str(),noCountryReply.length(),0);
                    continue;
                }
                if(clientID == "client1")
                    cout << "The main server has received the request on User " << useridQueery << " in " << countryQuery << " from client1 using TCP over port 33031 " << endl;
                if(clientID == "client2")
                    cout << "The main server has received the request on User " << useridQueery << " in " << countryQuery << " from client2 using TCP over port 33031 " << endl;

                //do the recommendation
                cout << "The main server has sent request from " << useridQueery << " to server " << serverChoice << " USING UDP over port 32031" << endl;
                string resultA = reconmendation(clientData+clientid_sep+1,serverChoice, sizeof(clientData));
                auto it = resultA.find(":");
                string protocol = resultA.substr(0, it);
                string messageAfterProtocol = resultA.substr(it+1);
                if(protocol == "replyFriendInfo")
                {

                    send(new_fd, resultA.c_str(), resultA.length(), 0);
                    string resultStr = resultA;
                    auto seperator = resultStr.find(":");                                                           //replyFriendInfo:625 not_country no_id
                    auto afterSeprator = resultStr.substr(seperator+1);
                    if(afterSeprator == "no_id")
                    {
                        cout << "The main server has received " << useridQueery << " not found " << "from server" << serverChoice << endl;
                        cout << "The main server has sent the error to client using TCP over 33031" << endl;
                    }
                    else if(afterSeprator == "no_friend_None")
                    {
                        cout << "There is no new frined recommendation as it's the only user or it has connected with all other users" << endl;
                        cout << "The main server has sent the error to client using TCP over 33031" << endl;
                    }
                    else
                    {
                        cout << "The main server has sent the searching results to client using TCP over 33031" << endl;
                    }

                } else break;

            } // EOF while loop
            cout << "process end\n";
            close(new_fd);
            exit(0);
        } //EOF fork
    }
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
    sendto(UDPsock, dataIn, dataInSize, 0, p->ai_addr, p->ai_addrlen);
    char result[1000];
    memset(result, 0, sizeof(result));
    addr_len = sizeof their_addr;
    recvfrom(UDPsock, result, sizeof result, 0, (struct sockaddr *) &their_addr, &addr_len);
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


























