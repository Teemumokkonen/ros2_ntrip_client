#include "iostream"
#include "client.hpp"



int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    std::shared_ptr<Ntrip::NtripClient> client = std::make_shared<Ntrip::NtripClient>();
    client->connect();
    return 0;
}