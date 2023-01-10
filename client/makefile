CFLAGS:=-c -Wall -Weffc++ -g -std=c++17 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: clean StompWCIClient

EchoClient: bin/ConnectionHandler.o bin/echoClient.o
	g++ -o bin/EchoClient bin/ConnectionHandler.o bin/echoClient.o $(LDFLAGS)

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/Frame.o bin/KeyboardInputManager.o bin/ServerInputManager.o bin/StompProtocol.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/Frame.o bin/KeyboardInputManager.o bin/ServerInputManager.o bin/StompProtocol.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/StompProtocol.o:	src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/KeyboardInputManager.o: src/KeyboardInputManager.cpp
	g++ $(CFLAGS) -o bin/KeyboardInputManager.o src/KeyboardInputManager.cpp	

bin/ServerInputManager.o: src/ServerInputManager.cpp
	g++ $(CFLAGS) -o bin/ServerInputManager.o src/ServerInputManager.cpp


.PHONY: clean
clean:
	rm -f bin/*
	