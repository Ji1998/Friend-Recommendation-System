// ECE450 main server
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
#include <iostream>
#include<fstream>
#include<iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include<queue>

#define UDPPORTA "31031"
#define MAXBUFLEN 1000
#define HOST "localhost"

using namespace :: std;

//function declaration
string convertToString(char* a, int size);
int recommendation(int idQuery, string countryQuery,unordered_map<string, unordered_map<int, unordered_set<int>>>countryDict);


void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



int main()
{
    // set up UDP
    int sockfd;
    struct addrinfo hints, *serverinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    //set up file reader
    unordered_map<string, unordered_map<int, unordered_set<int>>> countryDict;      //country->(input if -> friends)
    unordered_set<string> countryInBackendA;                                        // return the countries contained in backendserver
    string countryName;                                                             //will be updated when found a new country
    int newlineUserId;                                                           //will be updated when found a newline of user
    string line;

    //set up UDP
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // UDP dgram sockets
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, UDPPORTA, &hints, &serverinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    //loop through all structs to find the one can be connected
    for (p = serverinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
            == -1) {
            perror("serverA: socket");
            continue;
        }
        if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("serverA: bind");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "serverA: failed to bind socket\n");
        return 2;
    }
    freeaddrinfo(serverinfo);
    cout << "The Server B is up and running using UDP on port 31031" << endl;


    //read file
    ifstream myfile("data2.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line)) {
            istringstream is(line);
            string stringWithoutSpace;
            string firstString;
            is >> firstString;
            if (isalpha(firstString[0])) {
                countryInBackendA.insert(firstString);
                countryName = firstString;
            }
            if (isdigit(firstString[0])) {
                newlineUserId = stoi(firstString);
                string followString = "";
                countryDict[countryName][newlineUserId].insert(
                        1234);                   /*insert a value then clear the value to keep the value with no friend*/
                countryDict[countryName][newlineUserId].clear();                           /*                                                                   */
                while (is >> followString) {
                    countryDict[countryName][newlineUserId].insert(stoi(followString));
                }
            }
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    //send and receive UDP
    addr_len = sizeof their_addr;
    while(1)                                                                                        //once build the UDP connection, keep receving and sending
    {
        char buf[MAXBUFLEN];
        size_t n = recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *) &their_addr, &addr_len);
        //cout << n << endl;
        //int buf_size = sizeof(buf) / sizeof(char);
        string received = convertToString(buf, n);
        auto it = received.find(":");
        string receivedProtocol = received.substr(0, it);
        cout << buf << endl;
        cout << received << endl;
        if(receivedProtocol == "countryList")                                                      //countryList:
        {
            string replyCountryList = "replyList:";
            for(auto a : countryInBackendA)
            {
                string temp = a + "+";
                replyCountryList += temp;
            }
            int n = replyCountryList.length();
            char sendCountryList [n+1];
            strcpy(sendCountryList, replyCountryList.c_str());                                  //replyList:countryA+countryB+countryC+
            sendto(sockfd, sendCountryList, sizeof sendCountryList, 0, (struct sockaddr *) &their_addr, addr_len);
            cout << "The Server B has sent a country list to Main Server" << endl;
        }
        else if(receivedProtocol == "query")                                                    //query:country_name+id
        {
            string replyFriendInfo = "replyFriendInfo:";                                        //replyFriendInfo:message
            string afterReceived = received.substr(it+1);                                  //country_name+id
            auto seperator = afterReceived.find("+");
            string countryName = afterReceived.substr(0, seperator);                       //country_name
            //cout << afterReceived.substr(seperator+1) << endl;
            int idForQuery = stoi(afterReceived.substr(seperator+1));                 //id
            cout << "The server B has received request for finding possible frineds of User: " << idForQuery << " in " << countryName << endl;
            cout << "The server B is searching possible frineds for User: " << idForQuery << endl;
            int friendID = recommendation(idForQuery, countryName, countryDict);
            if(friendID == -1)
            {
                replyFriendInfo += "no_id";                                      //replyFriendInfo:no_id
                cout << "The server B has sent<" << idForQuery << ">" << "not found to Main Server" << endl;
            }
            else if(friendID == -2) replyFriendInfo += "no_country";
            else if(friendID == -3) replyFriendInfo += "no_friend_None";                                 //connected with all nodes || only itself in the country
            else if(friendID == -4) replyFriendInfo += "no_friend_NULL";                                //no recommendation as quried id has nothing coneccted  to all other users
            else replyFriendInfo += to_string(friendID);
            int n = replyFriendInfo.length();
            char sendreplyFrinedInfo [n+1];
            strcpy(sendreplyFrinedInfo, replyFriendInfo.c_str());
            sendto(sockfd, sendreplyFrinedInfo, sizeof sendreplyFrinedInfo, 0, (struct sockaddr *) &their_addr, addr_len);
            cout << "Here are the results: " << friendID << endl;
            cout << "The server B has sent the results to Main Server" << endl;
        }

    }
    close(sockfd);
    return 0;
}
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; a[i] != '\0'; i++)
    {
        s = s + a[i];
    }
    return s;
}



int recommendation(int idQuery, string countryQuery,unordered_map<string, unordered_map<int, unordered_set<int>>>countryDict)
{
    auto comp  = [](pair<int, int> &a, pair<int, int>&b)
    {
        if(a.second == b.second) return a.first>b.first;
        else return a.second < b.second;
    };

    unordered_map<int, unordered_set<int>> countryMap;                               //map of countryQuery(2nd layer)
    unordered_set<int> idQueryFriends;                                              //friends of idQuery
    unordered_map<int, int> counter;                                                //count # of repeats of 1st level connection
    unordered_map<int,int> counter_mostPopular;                                      //for the purpose of counting the most popular node
    if(!countryDict.count(countryQuery)) return -2;                                 //didn't find country, error code = -2
    countryMap = countryDict[countryQuery];
    if(!countryMap.count(idQuery)) return -1;                                       //didn't find user, error code = -1;
    else if(countryMap.size() <= 1) return -3;                                      //there is only one user in the coutry, return None
    idQueryFriends = countryMap[idQuery];
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(comp)> findMostPopular(comp);
    if(idQueryFriends.empty())                                                          //the userid queried has no connection with any other node
    {
        for(auto node : countryMap)
        {
            findMostPopular.push({node.first, node.second.size()});
        }
        auto top = findMostPopular.top();
        return top.first;
    }
    for(auto a : idQueryFriends)
    {
        for(auto b : countryMap[a])
        {
            if(idQuery == b || idQueryFriends.count(b)) continue;
            ++counter[b];
        }
    }
    if(counter.empty()) return -3;

    priority_queue<pair<int,int>, vector<pair<int, int>>, decltype(comp)> friendsComparator(comp);

    for(auto a : counter)
    {
        if(a.first == idQuery) continue;
        friendsComparator.push({a.first, a.second});
    }
    auto it = friendsComparator.top();
    if(friendsComparator.empty()) return -3;                                    //the userid queried has connected with all friends return None
    return it.first;
}







