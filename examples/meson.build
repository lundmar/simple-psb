# Get the libzmq dependency
zmq = dependency('libzmq', version: '>=4.3.5', required: true)

# Include the src directory for headers
inc_dir = include_directories('../src')

# Build the example applications by linking them with the shared library from the src directory
executable('publisher1', 'publisher1.cpp', link_with: libbroker_pub_sub, include_directories: inc_dir, dependencies: zmq)
executable('publisher2', 'publisher2.cpp', link_with: libbroker_pub_sub, include_directories: inc_dir, dependencies: zmq)
executable('subscriber1', 'subscriber1.cpp', link_with: libbroker_pub_sub, include_directories: inc_dir, dependencies: zmq)
executable('subscriber2', 'subscriber2.cpp', link_with: libbroker_pub_sub, include_directories: inc_dir, dependencies: zmq)
