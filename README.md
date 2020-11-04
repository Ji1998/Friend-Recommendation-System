Friend Recommendation System -- EE450 peoject 
Guangsen Ji
student id: 4595-9500-31
10/31/2020

Introduction:
In this project I build a friend recommendation system based on graph search. The user will have to type in the "user id" and the "country" they want to query
for,then the main server will send the query to backend server. After the backend server processed the query it will send the result back to the client through 
the main server. There are 5 files included in the projects, below is description of the funcationality of each of them.

Instruction on how to open the project:
1. open backend-serverA.cpp
2. open backend-serverB.cpp
3. open mainserver.cpp
4. open client1.cpp & client2.cpp

-----Or you can use make file to compile all the .cpp file using make -all command and then run as the sequence mentioned above-----


File Functionality Description:
client1.cpp: This is where the user will run and type in user id and contry name for query. It connect with main server through TCP
client2.cpp: This is where the user will run and type in user id and contry name for query. It connect with main server through TCP
mainserver.cpp: This is where the main server run. The main server is connectd with client through TCP and UDP with backend-server. 
backendserverA.cpp: This is one of the backendserver. This server will read data1.txt and re-assemble the data into a graph for the purpose of recommendation
backendserverB.cpp: This is one of the backendserver. This server will read data2.txt and re-assemble the data into a graph for the purpose of recommendation
makefile: This is the file will output executale file of all the source files

Message exchange format:
The message exchanges in C++ string. The head of each string is seperated by sepcial symbols. Below are the string sent and received by each component in this
project:

client
  send: client1=query:country+userid
  receive:replyFriendInfo:userid 
          replyFriendInfo:not_country
          replyFriendInfo:no_id
main server:
  receive from client: same as sent from client
  send to backendserver:countryList:
                        query:country_name+id;
  receive from backendserver:replyList:countryA+countryB+countryC+
                             replyFriendInfo:message
  send to client: replyFriendInfo:message

backendserver:
  send to main server:replyList:countryA+countryB+countryC+
                      replyFriendInfo:message
  receive from main server: query:country_name+id
 
Running Condition:
The mainserver won't deal with concurrent client request. i.e. if both client send to the main server simmutaneously, the server may reply the answer in the 
wrong sequence to the client.

Reused code(all from Beej's):
    servermain.cpp: build UDP connnection, build TCP connection(as shown in the comment in the file)
    backend-serverA.cpp: UDP connection(as shown in the comment in the file)
    backend-serverB.cpp: UDP connection(as shown in the comment in the file)
    client1.cpp: UDP connection(as shown in the comment in the file)
    client2.cpp: UDP connection(as shown in the comment file)
    
  
  
  


