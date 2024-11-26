#include "rclcpp/rclcpp.hpp"
#include <stdio.h>        // For perror()
#include <stdlib.h>       // For EXIT_FAILURE
#include <string.h>       // For memset()
#include <unistd.h>       // For close()
#include <arpa/inet.h>    // For sockaddr_in, inet_pton(), htons()
#include <sys/socket.h>   // For socket(), connect()
#include <netinet/in.h>   // For struct sockaddr_in
#include <netdb.h> // For gethostbyname()
#include <netinet/tcp.h>
#include <asm-generic/fcntl.h>
//#include <fcntl.h>
namespace Ntrip {

class NtripClient
{
private:
    // include node handle
    std::shared_ptr<rclcpp::Node> nh;
    int clientSocket;
    struct sockaddr_in serverAddress;

public:
    NtripClient(/* args */);
    ~NtripClient() {};

    bool connect();
};

}
