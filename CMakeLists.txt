cmake_minimum_required(VERSION 3.0)

project(bluez-gatt)

include_directories(includes)
include_directories(includes/src/shared)
include_directories(includes/lib)

add_subdirectory(libbluetooth)
add_subdirectory(libshared)

# btgatt-client
add_executable(btgatt-client
    btgatt-client.c)

target_link_libraries(btgatt-client bluetooth shared)


# btgatt-server
add_executable(btgatt-server
    btgatt-server.c device_adv.c util.c)

target_link_libraries(btgatt-server bluetooth shared)


# thingy.c
add_executable(thingy
    thingy.c)

target_link_libraries(thingy bluetooth shared)

