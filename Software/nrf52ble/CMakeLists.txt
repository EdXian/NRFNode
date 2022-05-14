cmake_minimum_required(VERSION 3.16.0)

set( CMAKE_BUILD_TYPE Debug)
project(nrfble)





set(CMAKE_CXX_STANDARD 11)



set(TCP_SOCKET_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tcpsocket/tcpsocket.c )



add_executable(nrf52ble main.cpp ${TCP_SOCKET_FILE})
message(STATUS ${CMAKE_C_COMPILER})
message(STATUS ${CMAKE_CXX_COMPILER})
target_compile_definitions(
    nrf52ble PUBLIC
    -DNRF_SD_BLE_API=5
    )

if (WIN32)

#    add_library(manager BleManager/manager.cpp)

set(DEFINES "-DUNICODE -D_UNICODE -DWIN32 -DMINGW_HAS_SECURE_API=1")

#set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} fno-keep-inline-dllexport -g -Wall -W -Wextra ${DEFINES}")
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fno-keep-inline-dllexport -g -std=gnu++11 -Wall -W -Wextra -fexceptions -mthreads ${DEFINES}")


message(STATUS ${CMAKE_EXE_LINKER_FLAGS})
    target_include_directories(
            nrf52ble PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/tcpsocket
            ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/include/sd_api_v5
    )

    target_link_options(
        nrf52ble PUBLIC
        -Wl,--start-group
        -L${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/lib
        -lnrf-ble-driver-sd_api_v5-mt-4_1_4
        -lnrf-ble-driver-sd_api_v5-mt-static-4_1_4
        -Wl,-subsystem,console
        -mthreads
        -Wl,--end-group
        )

target_link_libraries(
    nrf52ble PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/lib/fmt.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/libble/nrf-ble-driver-4.1.4-win_x86_64/lib/nrf-ble-driver-sd_api_v5-mt-4_1_4.dll
   wsock32 ws2_32
    )

endif (WIN32)


if (UNIX)

    target_include_directories(
            nrf52ble PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/tcpsocket
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


