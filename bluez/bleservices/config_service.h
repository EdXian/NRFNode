#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include "thingy_service.h"


class config_service :public thingy_service
{
public:
    config_service(QString Name, thingyServiceType thingType_);
};

#endif // CONFIG_SERVICE_H
