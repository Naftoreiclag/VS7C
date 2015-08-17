#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "SFML/Network.hpp"

class Player {
public:
    unsigned short port;
    sf::IpAddress address;
    std::string username;
    sf::Uint32 id;

    Player(std::string username, sf::Uint64 id, sf::IpAddress address, unsigned short port);
    virtual ~Player();
protected:
private:
};

#endif // PLAYER_H
