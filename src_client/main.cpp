
#include <iostream>

#include "irrlicht.h"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

int main() {
    std::cout << "SK Client" << std::endl;

    sf::UdpSocket clientSocket;

    unsigned short port = 25566;
    sf::IpAddress receiver = sf::IpAddress::LocalHost;

    bool runClient = true;
    while(runClient) {
        sf::Packet chatPacket;

        std::string message;
        std::cin >> message;
        chatPacket << message;

        if(clientSocket.send(chatPacket, receiver, port) != sf::Socket::Done) {
            // Explode
        }

        if(message == "stop") {
            runClient = false;
        }
    }
}
