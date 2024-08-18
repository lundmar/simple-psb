#include <simple-psb/publisher.hpp>
#include <stdexcept>
#include <iostream>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::milliseconds

Publisher::Publisher(const std::string& backend_endpoint)
    : context(1), socket(context, ZMQ_PUB)
{
    try {
        socket.connect(backend_endpoint);
        std::cout << "[Publisher] Connected to backend endpoint: " << backend_endpoint << std::endl;

        // Ensure a brief delay to allow connections to stabilize
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to connect publisher socket: " + std::string(e.what()));
    }
}

void Publisher::publish(const std::string& topic, const std::vector<uint8_t>& data) {
    zmq::message_t topic_msg(topic.data(), topic.size());
    zmq::message_t message_msg(data.data(), data.size());

    try {
        socket.send(topic_msg, zmq::send_flags::sndmore);
        socket.send(message_msg, zmq::send_flags::none);
        std::cout << "[Publisher] Sent message on topic: " << topic << " with data size: " << data.size() << std::endl;
    } catch (const zmq::error_t& e) {
        throw std::runtime_error("Failed to send message: " + std::string(e.what()));
    }
}

