#include <simple-psb/subscriber.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

class Subscriber1 : public Subscriber
{
    public:
        using Subscriber::Subscriber;

        void receive_callback(const std::string& topic, const std::vector<uint8_t>& data) override
        {
            std::string message(data.begin(), data.end());
            std::cout << "[Subscriber1] Received Topic: " << topic << ", Message: " << message << std::endl;
        }
};

int main()
{
    std::cout << "Starting Subscriber 1" << std::endl;

    // Create subscriber connected to broker
    Subscriber1 subscriber("ipc:///tmp/broker_frontend.ipc");

    // Subscribe to topics
    subscriber.subscribe("topic1");
    subscriber.subscribe("topic3");

    // Let subscriber start receiving messages
    std::thread subscriber_thread([&subscriber]()
    {
        subscriber.start();
    });

    subscriber_thread.join();

    std::cout << "Subscriber 1 finished" << std::endl;

    return 0;
}
