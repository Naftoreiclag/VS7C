#include "Player.h"

Player::Player(std::string username, sf::Uint64 id, sf::IpAddress address, unsigned short port)
: username(username),
id(id),
address(address),
port(port)
{
    //ctor
}

Player::~Player()
{
    //dtor
}
