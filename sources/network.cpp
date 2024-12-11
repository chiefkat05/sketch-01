#include "../headers/network.h"
#include "../headers/system.h"

connector::connector() {}
connector::connector(sf::IpAddress _i, unsigned short _p) : host_ip(_i), port(_p) {}

void connector::bind(unsigned short _port)
{
    if (socket.bind(_port) != sf::Socket::Done)
    {
        std::cout << "network error: Failed to bind socket to port!\n";
        return;
    }
    std::cout << "socket binded\n";
    port = _port;
    socket.setBlocking(false);
    binded = true;
}

void connector::speak(sf::IpAddress listener, std::string str)
{
    sf::Packet packet;
    packet << MES_STRING;
    packet << str;

    if (socket.send(packet, listener, port) != sf::Socket::Done)
    {
        std::cout << "network error: Failed to send data to " << listener << "on port " << port << "\n";
        return;
    }

    std::cout << "sent packet to " << listener << ", on port " << port << "\n";
}

void connector::addSprite(sf::IpAddress listener, std::string _sprPath, float x, float y)
{
    sf::Packet packet;
    packet << MES_SPRITE_ADD;
    packet << _sprPath << x << y;

    if (socket.send(packet, listener, port) != sf::Socket::Done)
    {
        std::cout << "network error: Failed to send data to " << listener << "on port " << port << "\n";
        return;
    }

    std::cout << "sent packet to " << listener << ", on port " << port << "\n";
}

game_system game;
void connector::listen(unsigned int ip_index)
{
    sf::Socket::Status messageStatus = socket.receive(message, ip_list[ip_index], message_port);
    if (messageStatus == sf::Socket::NotReady)
    {
        return;
    }
    else if (messageStatus != sf::Socket::Done)
    {
        std::cout << "network error: Failed to receive data\n";
        return;
    }

    int mes_type;
    std::string mes_string;
    float mes_x, mes_y;

    message >> mes_type;
    switch (mes_type)
    {
    case MES_STRING:
        message >> mes_string;
        std::cout << "received message: " << mes_string << "\n";
        break;
    case MES_SPRITE_ADD:
        message >> mes_string >> mes_x >> mes_y;
        game.characters[game.characterCount]->visual = sprite(mes_string.c_str(), mes_x, mes_y, 1, 1);
        ++game.characterCount;
        std::cout << "sprite added to world\n";
        break;
    case MES_SPRITE_POS:
        int spriteIndex;
        float spriteX, spriteY;
        message >> spriteIndex;
        message >> spriteX;
        message >> spriteY;

        // game.characters[spriteIndex]->posX = spriteX;
        // game.characters[spriteIndex]->posY = spriteY;
        break;
    default:
        break;
    }
};