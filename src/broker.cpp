#include <simple-psb/broker.hpp>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <zmq.h>

Broker::Broker(const std::string& frontend_endpoint, const std::string& backend_endpoint, bool verbose)
    : context(1), frontend(context, ZMQ_XPUB), backend(context, ZMQ_XSUB), verbose(verbose)
{
    try {
        frontend.bind(frontend_endpoint);
        std::cout << "[Broker] Frontend (XPUB) bound to " << frontend_endpoint << std::endl;
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to bind frontend (XPUB) socket: " + std::string(e.what()));
    }

    try {
        backend.bind(backend_endpoint);
        std::cout << "[Broker] Backend (XSUB) bound to " << backend_endpoint << std::endl;
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to bind backend (XSUB) socket: " + std::string(e.what()));
    }

    // Start monitoring connections if verbose mode is enabled
    if (verbose) {
        std::thread frontend_monitor_thread(&Broker::monitor_connections, this, "frontend", std::ref(frontend));
        std::thread backend_monitor_thread(&Broker::monitor_connections, this, "backend", std::ref(backend));

        frontend_monitor_thread.detach();
        backend_monitor_thread.detach();
    }
}

void Broker::start() {
    if (verbose) {
        print_verbose("Broker started with verbose output enabled.");
    }

    try {
        zmq::proxy(frontend, backend);
        std::cout << "[Broker] Proxy started between XPUB and XSUB." << std::endl;
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to start proxy: " + std::string(e.what()));
    }
}

void Broker::monitor_connections(const std::string& socket_name, zmq::socket_t& socket) {
    std::string monitor_endpoint = "inproc://" + socket_name + "_monitor";
    socket.get(zmq::sockopt::events);

    if (zmq_socket_monitor(socket.handle(), monitor_endpoint.c_str(), ZMQ_EVENT_ALL) != 0) {
        throw std::runtime_error("Failed to monitor " + socket_name + " socket: " + std::string(zmq_strerror(errno)));
    }

    zmq::socket_t monitor_socket(context, ZMQ_PAIR);
    monitor_socket.connect(monitor_endpoint);

    while (true) {
        handle_monitor_event(monitor_socket, socket_name);
    }
}

void Broker::handle_monitor_event(zmq::socket_t& monitor_socket, const std::string& socket_name) {
    zmq_event_t event;
    zmq::message_t event_message;

    try {
        monitor_socket.recv(event_message, zmq::recv_flags::none);
        memcpy(&event, event_message.data(), sizeof(event));

        std::string event_name;
        switch (event.event) {
            case ZMQ_EVENT_CONNECTED:
                event_name = "CONNECTED";
                break;
            case ZMQ_EVENT_DISCONNECTED:
                event_name = "DISCONNECTED";
                break;
            case ZMQ_EVENT_ACCEPTED:
                event_name = "ACCEPTED";
                break;
            case ZMQ_EVENT_CLOSED:
                event_name = "CLOSED";
                break;
            case ZMQ_EVENT_HANDSHAKE_SUCCEEDED:
                event_name = "HANDSHAKE SUCCEEDED";
                break;
            default:
                event_name = "UNKNOWN EVENT";
                break;
        }

        if (event_name != "UNKNOWN EVENT")
        {
            print_verbose("Event: " + event_name + " on " + socket_name + " socket.");
        }
    } catch (const zmq::error_t& e) {
        print_verbose("Error receiving monitor event: " + std::string(e.what()));
    }
}

void Broker::print_verbose(const std::string& message) {
    if (verbose) {
        std::cout << "[Broker] " << message << std::endl;
    }
}

