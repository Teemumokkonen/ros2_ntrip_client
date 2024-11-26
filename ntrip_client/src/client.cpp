#include "client.hpp"
#include "utils.hpp"
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
    struct hostent *host = gethostbyname("gps1.geod.agh.edu.pl");
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
    int buffer_size = 4096;
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
    std::string name_password = "template@gmail.com:none";
    std::string base64name_password = base64_encode(name_password);
    std::unique_ptr<char[]> buffer(
            new char[buffer_size], std::default_delete<char[]>());
    
    int ret = snprintf(buffer.get(), buffer_size - 1,
                        "GET /%s HTTP/1.1\r\n"
                       "User-Agent: %s\r\n"
                       "Authorization: Basic %s\r\n"
                       "\r\n",
                        "Test", "NTRIP NTRIPClient/1.1.0.", base64name_password.c_str());
    //connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    RCLCPP_INFO(nh->get_logger(), "send %s", buffer.get());
    if (send(clientSocket, buffer.get(), ret, 0) < 0)
        {
            RCLCPP_ERROR(rclcpp::get_logger("NtripClient"), "Send request failed!!!");
            close(clientSocket);
            return false;
    }

    int timeout = 300;
    while (timeout--) {
        ret = recv(clientSocket, buffer.get(), buffer_size, 0);
        if (ret > 0) {
            std::string result(buffer.get(), ret);
            if ((result.find("HTTP/1.1 200 OK") != std::string::npos) || (result.find("ICY 200 OK") != std::string::npos))
                { 
                    //RCLCPP_INFO(rclcpp::get_logger("NtripClient"), "Send gpgga data ok");
                    break;
                }
                else
                {
                    // The request and authorization were accepted, but the mountpoint was incorrect.
                    // Print the list of mountpoints for the user
                    RCLCPP_INFO(rclcpp::get_logger("NtripClient"), "Request result: %s", result.c_str());
                }
        }
    }    
}
