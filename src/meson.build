# Generate git version header
git_version_h = vcs_tag(command : ['git', 'describe', '--tags', '--always', '--dirty'],
                        input : 'git-version.h.in',
                        output :'git-version.h',
                        replace_string:'@GIT_VERSION@')

# Get the libzmq dependency
zmq = dependency('libzmq', version: '>=4.2', required: true)

# List of source files
src_files = [
  'broker.cpp',
  'publisher.cpp',
  'subscriber.cpp',
  git_version_h
]

# Create a shared library from the source files, linking with libzmq
libbroker_pub_sub = shared_library('simple-psb', src_files, dependencies: zmq,
                                   cpp_args: ['-Wno-unused-result'],
                                   install: true)  # Install the shared library

# Broker service application
broker_service = executable('simple-psb-broker', 'simple-psb-broker.cpp',
                            link_with: libbroker_pub_sub,
                            dependencies: zmq,
                            install: true,
                            install_dir: get_option('sbindir'))  # Install in sbin

# Install the headers
install_headers(['simple-psb/broker.hpp', 'simple-psb/publisher.hpp', 'simple-psb/subscriber.hpp'], subdir: 'simple-psb')
