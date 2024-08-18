#include <simple-psb/subscriber.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

class Subscriber2 : public Subscriber
{
    public:
        using Subscriber::Subscriber;

        void receive_callback(const std::string& topic, const std::vector<uint8_t>& data) override
        {
            std::string message(data.begin(), data.end());
            std::cout << "[Subscriber2] Received Topic: " << topic << ", Message: " << message << std::endl;
        }
};

int main()
{
    std::cout << "Starting Subscriber 2" << std::endl;

    // Create subscriber connected to broker
    Subscriber2 subscriber("ipc:///tmp/broker_frontend.ipc");

    // Subscribe to topics
    subscriber.subscribe("topic2");
    subscriber.subscribe("topic4");

    // Let subscriber start receiving messages
    std::thread subscriber_thread([&subscriber]()
    {
        subscriber.start();
    });

    subscriber_thread.join();

    std::cout << "Subscriber 2 finished" << std::endl;

    return 0;
}

