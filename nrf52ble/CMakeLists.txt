cmake_minimum_required(VERSION 3.16.0)

set( CMAKE_BUILD_TYPE Debug)
project(nrfble)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")



#set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

add_executable(nrf52ble main.cpp)

target_compile_definitions(
    nrf52ble PUBLIC
    -DNRF_SD_BLE_API=5
    )
set_property(TARGET nrf52ble PROPERTY C_STANDARD 99)
if (WIN32)
    target_include_directories(
            nrf52ble PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/include/sd_api_v5
    )

    target_link_directories(
         nrf52ble PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/lib
        )

    target_link_libraries(
       nrf52ble PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/lib/nrf-ble-driver-sd_api_v5-mt-4_1_4.lib
       ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/lib/nrf-ble-driver-sd_api_v5-mt-static-4_1_4.lib
    )
endif (WIN32)


if (UNIX)
    target_include_directories(
            nrf52ble PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-linux_x86_64/include/sd_api_v5
    )
    link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-linux_x86_64/lib
    )
    target_link_directories(
         nrf52ble PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-linux_x86_64/lib
        )
    target_link_libraries(
       nrf52ble PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-linux_x86_64/lib/libnrf-ble-driver-sd_api_v5.a
       pthread
       udev
    )
endif (UNIX)


