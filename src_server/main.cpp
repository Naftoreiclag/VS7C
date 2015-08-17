
#include <iostream>
#include <thread>

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
        sf::IpAddress clientAddress;
        unsigned short clientPort;
        if(serverSocket.receive(receivedPacket, clientAddress, clientPort) != sf::Socket::Done) {
            // Explode
        }


        sf::Uint8 type;

        receivedPacket >> type;

        if(type == 0) {
            std::string username;
            receivedPacket >> username;

            std::cout << username << " joined." << std::endl;
            std::cout << clientAddress << ":" << clientPort << std::endl;


            Player newPlay(username, nextId(), clientAddress, clientPort);
            players.push_back(newPlay);

            newPlay.id;

            sf::Packet handlePack;
            irr::u8 packetType = 4;
            handlePack << packetType;
            handlePack << newPlay.id;
            serverSocket.send(handlePack, clientAddress, clientPort);

            std::cout << "Entityhandle: " << newPlay.id << std::endl;
        }
        else {
            const Player* playSender = 0;
            for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++ it) {
                const Player& player = *it;

                if(player.address == clientAddress && player.port == clientPort) {
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
