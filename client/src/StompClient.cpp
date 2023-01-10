#include "../include/StompClient.h"
#include "../include/ConnectionHandler.h"
#include "../include/ServerInputManager.h"
#include "../include/KeyboardInputManager.h"
#include "Frame.h"
#include <thread>

//Termination condition


int main (int argc, char *argv[]) {
    while(1){
        if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
        }
        std::string host = argv[1];
        short port = atoi(argv[2]);
    
   
        ConnectionHandler connectionHandler(host, port);
        if (!connectionHandler.connect()) {
            std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
            return 1;
        }

        //Initialize objects to later start as threads
	    KeyboardInputManager readFromUser(connectionHandler);
	    ServerInputManager readFromServer(connectionHandler);

    //Perform login
    // while(!connect){
    //     string input ="";
    //     getline(std::cin, input);
    //     int index = input.find(" ");
    //     if(input.substr(0,index).compare("login") == 0){

    //         //connectionHandler.protosend(input);
    //         //if(connectionHandler.getLine() == CONNECTED)
    //             //disconnect = fasle
                
    //         //else
    //             //Tend to the ERROR
    //     }
    //     else{
    //         std::cout << "Please login before commiting any actions" << std::endl;
    //     }
    // }

	

        //while(!connectionHandler.protocol.should_terminate ){
        //This order of lines should make both threads operate concurrently while the main thread is waiting for them both to finish
        std::thread thread_readFromKeyboard(&KeyboardInputManager::run, &readFromUser);
		std::thread thread_readFromServer(&ServerInputManager::run, &readFromServer);
        thread_readFromKeyboard.join();
		thread_readFromServer.join();
        //}
        connectionHandler.close();
        connectionHandler.~ConnectionHandler();
        std::cout << "Socket has been closed and the connection was terminated" << std::endl;

        return 0;
    }
	
}