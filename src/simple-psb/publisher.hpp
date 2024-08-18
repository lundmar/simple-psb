#pragma once

#include <zmq.hpp>
#include <string>
#include <vector>

class Publisher {
public:
    // Create connected publisher
    Publisher(const std::string& backend_endpoint);

    // Publish a message with a topic and binary data as message
    void publish(const std::string& topic, const std::vector<uint8_t>& data);

private:
    zmq::context_t context;
    zmq::socket_t socket;
};

