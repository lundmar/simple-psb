#include <simple-psb/publisher.hpp>
#include <iostream>
#include <vector>
#include <cstring>

int main()
{
    std::cout << "Starting Publisher 2" << std::endl;

    // Create publisher connected to broker
    Publisher publisher("ipc:///tmp/broker_backend.ipc");

    // Prepare data
    std::string string1 = "Hello from Publisher 2 on Topic 3!";
    std::vector<uint8_t> data1(string1.begin(), string1.end());

    std::string string2 = "Hello from Publisher 2 on Topic 4!";
    std::vector<uint8_t> data2(string2.begin(), string2.end());

    // Publish data via topics
    publisher.publish("topic3", data1);
    publisher.publish("topic4", data2);

    std::cout << "Publisher 2 finished" << std::endl;

    return 0;
}
