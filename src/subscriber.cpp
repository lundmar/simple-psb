#include <simple-psb/subscriber.hpp>
#include <iostream>
#include <stdexcept>

Subscriber::Subscriber(const std::string& endpoint)
    : context(1), socket(context, ZMQ_SUB)
{
    try {
        socket.connect(endpoint);
        std::cout << "[Subscriber] Connected to endpoint: " << endpoint << std::endl;
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to connect subscriber socket: " + std::string(e.what()));
    }
}

void Subscriber::subscribe(const std::string& topic) {
    try {
        socket.set(zmq::sockopt::subscribe, topic);
        std::cout << "[Subscriber] Subscribed to topic: " << topic << std::endl;
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to subscribe to topic: " + std::string(e.what()));
    }
}

void Subscriber::start() {
    std::cout << "[Subscriber] Starting to receive messages..." << std::endl;
    receive();
}

void Subscriber::receive() {
    zmq::message_t topic_msg;
    zmq::message_t message_msg;

    while (true) {
        try {
            socket.recv(topic_msg, zmq::recv_flags::none);
            socket.recv(message_msg, zmq::recv_flags::none);

            std::string received_topic(static_cast<char*>(topic_msg.data()), topic_msg.size());
            std::vector<uint8_t> received_message(static_cast<uint8_t*>(message_msg.data()), static_cast<uint8_t*>(message_msg.data()) + message_msg.size());

            std::cout << "[Subscriber] Received message on topic: " << received_topic << std::endl;

            receive_callback(received_topic, received_message);
        } catch (const zmq::error_t& e) {
            std::cerr << "[Subscriber] Error receiving message: " << e.what() << std::endl;
            continue;
        }
    }
}

