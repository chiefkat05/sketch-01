#ifndef NETWORK_H
#define NETWORK_H

#include "setup.h"
#include <SFML/Network.hpp>

const unsigned int packet_size = 100;
const unsigned int max_connections = 12;

enum MESSAGE_TYPE
{
    MES_STRING,
    MES_SPRITE_POS,
    MES_SPRITE_ADD,
    MES_SOUND
};

struct connector
{
    sf::UdpSocket socket;
    sf::IpAddress host_ip = "127.0.0.1";

    sf::IpAddress ip_list[max_connections];
    unsigned short port = 0;

    bool binded = false;

    connector();
    connector(sf::IpAddress _i, unsigned short _p);

    // handling incoming traffic
    sf::Packet message;
    size_t message_size;
    unsigned int message_ip_index;
    unsigned short message_port;

    void bind(unsigned short _port);

    void speak(sf::IpAddress listener, std::string _mes);
    void addSprite(sf::IpAddress listener, std::string _sprPath, float x, float y);
    void listen(unsigned int ip_index);
};

#endif