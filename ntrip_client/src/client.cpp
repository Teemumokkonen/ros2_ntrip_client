#include "client.hpp"

Ntrip::NtripClient::NtripClient()
{   
    nh = std::make_shared<rclcpp::Node>("ntrip_node");
    // make the node handle and relevant topics and parameters
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        RCLCPP_ERROR(nh->get_logger(), "Creating a socket interface failed");
        exit(EXIT_FAILURE);
    }
    
    // Get the server address from the hostname
    struct hostent *host = gethostbyname("rtk2go.com");
    if (host == NULL) {
        fprintf(stderr, "Error: Could not resolve hostname.\n");
        exit(EXIT_FAILURE);
    }
    RCLCPP_INFO(nh->get_logger(), "Hostname resolved %s", host->h_addr);

    memset(&serverAddress, 0, sizeof(serverAddress)); // Zero out the structure
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2101);
    memcpy(&serverAddress.sin_addr.s_addr, host->h_addr, host->h_length);

}


/**
 * Create a socket and connect to the set client
 * 
 */
bool Ntrip::NtripClient::connect()
{
    struct timeval tv;
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(clientSocket, &writefds);

    tv.tv_sec = 5;  // Timeout for 5 seconds
    tv.tv_usec = 0;
    // Set non-blocking.
    //int flags = fcntl(clientSocket, F_GETFL);
    //fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

    RCLCPP_INFO(nh->get_logger(), "Binding to host");
    if (::connect(clientSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0) {
        if (select(clientSocket + 1, NULL, &writefds, NULL, &tv) > 0) {
        // The socket is ready for writing (i.e., the connection was successful)
        RCLCPP_INFO(nh->get_logger(), "Binded to the target server successfully");
        }
        else {
            perror("Connection failed to host");
            close(clientSocket);
            return false;
        }
    }

    RCLCPP_INFO(nh->get_logger(), "Binded to the target server succesfully");

    //connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    
}
