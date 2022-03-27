#ifndef ENVIRONMENT_SERVICE_H
#define ENVIRONMENT_SERVICE_H
#include "thingy_service.h"


class environment_service : public thingy_service
{
public:
    environment_service(QString Name, thingyServiceType thingType_);
    void read_handler(QLowEnergyCharacteristic ch,QByteArray bytes);
};

#endif // ENVIRONMENT_SERVICE_H
