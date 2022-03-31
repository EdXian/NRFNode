#ifndef UI_SERVICE_H
#define UI_SERVICE_H
#include "thingy_service.h"

class ui_service : public thingy_service
{
public:
    ui_service(QString Name, thingyServiceType thingType_);
};

#endif // UI_SERVICE_H
