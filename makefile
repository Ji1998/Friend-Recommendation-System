all: backendA backendB servermain client1 client2
backendA: backend-serverA.cpp
	g++ backend-serverA.cpp -std=c++14 -o backendA
backendB: backend-serverB.cpp
	g++ backend-serverB.cpp -std=c++14 -o backendB
servermain: servermain.cpp
	g++ servermain.cpp -std=c++14 -o servermain
client1: client1.cpp
	g++ client1.cpp -std=c++14 -o client1
client2: client2.cpp
	g++ client2.cpp -std=c++14 -o client2


serverA:
	./backendA
serverB:
	./backendB
mainserver:
	./servermain

