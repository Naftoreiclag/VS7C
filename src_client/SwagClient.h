#ifndef SWAGCLIENT_H
#define SWAGCLIENT_H

#include "SFML/Network.hpp"
#include "SFML/Network/Socket.hpp"

#include "ThreadQueue.h"
#include "Packet.h"

#include <thread>
#include <mutex>

namespace skm {

class SwagClient {
public:
    typedef unsigned short Port;

    enum Status {
        SUCCESS,
        NOTREADY,
        DISCONNECTED,
        ERROR
    };

private:
    bool listening;
    void listenThreadStart();
    ThreadQueue<Packet> data;
public:
    SwagClient();
    virtual ~SwagClient();
public:
    Status contactServer(sf::IpAddress serverAddress, Port port);
    ThreadQueue<Packet>& startListening();

    void stopListening();
protected:
private:
};

}

#endif // SWAGCLIENT_H
