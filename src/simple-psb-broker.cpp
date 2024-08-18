#include <simple-psb/broker.hpp>
#include <iostream>
#include <getopt.h>
#include "git-version.h"

void print_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n"
              << "Options:\n"
              << "  --verbose       Enable verbose output\n"
              << "  --help          Display this help message\n"
              << "  --version       Display the version of the broker service\n";
}

void print_version() {
    std::cout << "simple-psb-broker " << GIT_VERSION << std::endl;
}

int main(int argc, char* argv[]) {
    bool verbose = false;

    // Define the long options
    static struct option long_options[] = {
        {"verbose", no_argument, 0, 'v'},
        {"help",    no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    // Parse the command-line options
    while ((c = getopt_long(argc, argv, "vhV", long_options, &option_index)) != -1) {
        switch (c) {
            case 'v':
                verbose = true;
                break;
            case 'h':
                print_help(argv[0]);
                return 0;
            case 'V':
                print_version();
                return 0;
            case '?':
                // getopt_long will already have printed an error message
                return 1;
            default:
                print_help(argv[0]);
                return 1;
        }
    }

    std::cout << "Starting broker service..." << std::endl;

    // IPC endpoints for the broker
    Broker broker("ipc:///tmp/broker_frontend.ipc", "ipc:///tmp/broker_backend.ipc", verbose);
    broker.start();

    return 0;
}

