#pragma once

#include <zmq.hpp>
#include <string>

class Broker {
public:
    // Create broker
    Broker(const std::string& frontend_endpoint, const std::string& backend_endpoint, bool verbose = false);

    // Start broker
    void start();

private:
    zmq::context_t context;
    zmq::socket_t frontend;
    zmq::socket_t backend;
    bool verbose;

    void monitor_connections(const std::string& socket_name, zmq::socket_t& socket);
    void handle_monitor_event(zmq::socket_t& monitor_socket, const std::string& socket_name);
    void print_verbose(const std::string& message);
};

