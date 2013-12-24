// ----- The client -----

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
    
    // Create a socket and connect it to localhost on port 1488
    sf::TcpSocket socket;
    socket.connect(sf::IpAddress::LocalHost, 1488);
    // Send a message to the connected host
    std::string message = "Hi, I am a client";
    socket.send(message.c_str(), message.size() + 1);
    // Receive an answer from the server
    char buffer[1024];
    std::size_t received = 0;
    socket.receive(buffer, sizeof(buffer), received);
    std::cout << "The server said: " << buffer << std::endl;
    
    
    return 0;
}