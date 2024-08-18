#pragma once

#include <zmq.hpp>
#include <string>
#include <vector>

class Subscriber {
public:
    // Create connected subscriber
    Subscriber(const std::string& endpoint);

    // Subscribe to topic
    void subscribe(const std::string& topic);

    // Start listening for messages
    void start();

protected:
    // Callback called upon receiving messages
    virtual void receive_callback(const std::string& topic, const std::vector<uint8_t>& data) = 0;

private:
    zmq::context_t context;
    zmq::socket_t socket;

    void receive();
};

