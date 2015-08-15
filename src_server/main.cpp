
#include <iostream>

#include "irrlicht.h"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

int main() {
    std::cout << "SK Server" << std::endl;

    sf::UdpSocket serverSocket;

    unsigned short port = 25566;

    if(!serverSocket.bind(port)) {
        // Explode
    }

    sf::Packet chatPacket;
    sf::IpAddress sender;

    bool runServer = true;
    while(runServer) {
        if(serverSocket.receive(chatPacket, sender, port) != sf::Socket::Done) {
            // Explode
        }

        std::string message;
        chatPacket >> message;

        std::cout << sender << ":" << port << std::endl;
        std::cout << "> " << message << std::endl;

        if(message == "stop") {
            runServer = false;
        }
    }
}
