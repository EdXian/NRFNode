#ifndef MOTION_SERVICE_H
#define MOTION_SERVICE_H
#include "thingy_service.h"




class motion_service : public thingy_service
{
public:
    motion_service(QString Name, thingyServiceType thingType_);
    void read_handler(QLowEnergyCharacteristic ch,QByteArray bytes);
    Points3D getAccValue();
signals:


private:
    Points3D Acc;
//    void set_service_uuid();
//    void get_service_uuid();
};

#endif // MOTION_SERVICE_H
