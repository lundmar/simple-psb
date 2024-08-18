#include <simple-psb/publisher.hpp>
#include <iostream>
#include <vector>
#include <cstring>

int main()
{
    std::cout << "Starting Publisher 1" << std::endl;

    // Create publisher connected to broker
    Publisher publisher("ipc:///tmp/broker_backend.ipc");

    // Prepare data
    std::string string1 = "Hello from Publisher 1 on Topic 1!";
    std::vector<uint8_t> data1(string1.begin(), string1.end());

    std::string string2 = "Hello from Publisher 1 on Topic 2!";
    std::vector<uint8_t> data2(string2.begin(), string2.end());

    // Publish data via topics
    publisher.publish("topic1", data1);
    publisher.publish("topic2", data2);

    std::cout << "Publisher 1 finished" << std::endl;

    return 0;
}
