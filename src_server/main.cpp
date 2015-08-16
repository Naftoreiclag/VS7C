
#include <iostream>

#include "irrlicht.h"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

#include "Player.h"

sf::Uint64 lastPlayId = 0;
sf::Uint64 nextId() {
    return lastPlayId ++;
}

int main() {
    std::cout << "SK Server" << std::endl;

    sf::UdpSocket serverSocket;

    unsigned short port = 25566;

    if(!serverSocket.bind(port)) {
        // Explode
    }


    std::vector<Player> players;

    bool runServer = true;
    while(runServer) {

        sf::Packet receivedPacket;
        sf::IpAddress senderAddress;
        unsigned short senderPort;
        if(serverSocket.receive(receivedPacket, senderAddress, senderPort) != sf::Socket::Done) {
            // Explode
        }


        sf::Uint8 type;

        receivedPacket >> type;

        if(type == 0) {
            std::string username;
            receivedPacket >> username;

            std::cout << username << " joined." << std::endl;

            Player newPlay(username, nextId(), senderAddress, senderPort);
            players.push_back(newPlay);
        }
        else {
            const Player* playSender = 0;
            for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++ it) {
                const Player& player = *it;

                if(player.address == senderAddress && player.port == senderPort) {
                    playSender = &player;
                    break;
                }
            }

            if(type == 1) {
                std::string message;
                receivedPacket >> message;

                std::cout << playSender->username << ": "  << message << std::endl;

                if(message == "stop") {
                    runServer = false;
                }
            }
        }
    }
}
