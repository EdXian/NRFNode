TEMPLATE = app
CONFIG += console c++11 debug
CONFIG -= app_bundle
CONFIG -= qt



VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 0


SOURCES += \
        main.cpp


#DEPLOY_OPTIONS += "--debug"
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

DEPENDPATH += \
    libble/nrf-ble-driver-4.1.4-linux_x86_64/include/sd_api_v5 \

INCLUDEPATH += \
    libble/nrf-ble-driver-4.1.4-linux_x86_64/include/sd_api_v5 \

LIBS += \
    -L$${PWD}/nrf-ble-driver-4.1.4-linux_x86_64/lib libnrf-ble-driver-sd_api_v5.a
 






