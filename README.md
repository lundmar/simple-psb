# simple-psb - A Simple Pub/Sub Library And Service

## 1. Introduction

This project serves as a simple example on how to create an interprocess
communication framework based on the classic publish/subscribe pattern model
using the ZeroMQ messaging library (see https://zeromq.org).

The project includes a library "libsimple-psb" from which publisher,
subscriber, and broker applications can be made. The project also includes a
broker service named "simple-psb-broker".

### 1.1 Motivation

To demonstrate:

 * The flexibility and benefit of using ZeroMQ to easily create a pub/sub IPC
 framework based on UNIX domain sockets (file sockets).

 * Best practices for structuring a software project, especially one to be
 deployed on GNU/Linux systems.

### 1.2 Architecture

The simple-psb architecture works on the principles of the classic pub/sub
(publish/subscribe) pattern model.

The pub/sub architecture offers a unique alternative to traditional
client-server (request-response) models. In the request-response approach, the
client directly communicates with the server endpoint, creating a bottleneck
that slows down performance. On the other hand, the pub/sub model decouples the
publisher of the message from the subscribers. The publisher and subscriber are
unaware that the other exists. As a third component, a broker, handles the
connection between them and filters all incoming messages and distributes them
to subscribers correctly. This decoupling combined with the filtering mechanism
produces a faster and more efficient communication process.


       ┌──────────────────────────────────────────────────────────────────────┐
       │                                                                      │
       │                               Broker                                 │
       │                                                                      │
       └───────┬───────────────┬──────────────────┬──────────────────▲────────┘
               │               │                  │                  │
               │ Data          │ Data             │ Data             │ Data
               │               │                  │                  │
        ┌──────▼───────┐┌──────▼───────┐   ┌──────▼───────┐   ┌──────┼───────┐
        │              ││              │   │              │   │              │
        │ Subscriber 1 ││ Subscriber 2 │...│ Subscriber N │   │ Publisher 1  │
        │              ││              │   │              │   │              │
        └──────────────┘└──────────────┘   └──────────────┘   └──────────────┘

 * Publishers and subscribers connect to the broker
 * Publishers publish data with topic names
 * Subscribers subscribe to data by topic names
 * Broker filters and distributes data accordingly

## 2. libsimple-psb API

### 2.1 Broker Class
```cpp
class Broker
{
    public:
        // Create broker
        Broker(const std::string& frontend_endpoint,
               const std::string& backend_endpoint, bool verbose = false);

        // Start broker
        void start();
};
```

### 2.2 Publisher Class
```cpp
class Publisher
{
    public:
        // Create connected publisher
        Publisher(const std::string& backend_endpoint);

        // Publish a message with a topic and binary data as message
        void publish(const std::string& topic, const std::vector<uint8_t>& data);
};
```

### 2.3 Subscriber Class
```cpp
class Subscriber
{
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
};
```

### 2.4 API usage examples

#### Broker example
```cpp
#include <simple-psb/broker.hpp>
#include <iostream>
#include <getopt.h>

int main(int argc, char* argv[])
{
    bool verbose = false;

    std::cout << "Starting broker service..." << std::endl;

    // IPC endpoints for the broker
    Broker broker("ipc:///tmp/broker_frontend.ipc", "ipc:///tmp/broker_backend.ipc", verbose);
    broker.start();

    return 0;
}
```

#### examples/publisher1.cpp

```cpp
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
```

#### examples/subscriber1.cpp
```cpp
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
```

## 3. Installation

Before running the install steps make sure you have the ZeroMQ libraries
installed. For example:

```
$ sudo apt install meson libzmq5 libzmq3-dev
```

Install steps:
```
$ meson setup build
$ meson compile -C build
$ meson install -C build
```

See meson\_options.txt for build options.

Note: The meson install steps may differ depending on your specific system.

## 4. Developers Guide

Setup and build broker service and publish/subscribe examples:
```
$ meson setup build -Dexamples=true
$ meson build -C build
```

Run broker serice:
```
./build/src/simple-psb-broker --verbose
```

Run subscriber 1:
```
./build/examples/subscriber1
```

Run subscriber 2:
```
./build/examples/subscriber2
```

Run publisher 1:
```
./build/examples/publisher1
```

Run publisher 2:
```
./build/examples/publisher2
```

Note: This implementation offers very high robustness. Because we are using the
ZMQ messaging sockets we can start or restart the various steps out of order.
For example, one can restart the broker and the subscribers and publishers will
automatically continue operation. Likewise, one can start the publishers and
subscribers first, and last start the broker.

## 5. Known Issues

TBD - Put any known issues here.

## 6. Notes

Notice the directory organization:

```
simple-psb
├── examples
│   ├── meson.build
│   ├── publisher1.cpp
│   ├── publisher2.cpp
│   ├── subscriber1.cpp
│   └── subscriber2.cpp
├── man
│   ├── meson.build
│   └── simple-psb-broker.8.in
├── meson.build
├── meson_options.txt
├── README.md
├── src
│   ├── broker.cpp
│   ├── git-version.h.in
│   ├── meson.build
│   ├── publisher.cpp
│   ├── simple-psb
│   │   ├── broker.hpp
│   │   ├── publisher.hpp
│   │   └── subscriber.hpp
│   ├── simple-psb-broker.cpp
│   └── subscriber.cpp
└── systemd
    ├── meson.build
    └── simple-psb-broker.service
```

Notice the conventional directory organization of the installed files:

```
usr
├── include
│   └── simple-psb
│       ├── broker.hpp
│       ├── publisher.hpp
│       └── subscriber.hpp
├── lib
│   ├── systemd
│   │   └── system
│   │       └── simple-psb-broker.service
│   └── x86_64-linux-gnu
│       └── libsimple-psb.so
├── sbin
│   └── simple-psb-broker
└── share
    └── man
        └── man8
            └── simple-psb-broker.8
```

ZeroMQ provides a quite comprehensive guide on how to implement the pub/sub
pattern model.

See https://zguide.zeromq.org/docs/chapter5
