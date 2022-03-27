#ifndef THINGY_MANAGER_H
#define THINGY_MANAGER_H
#include "bleinterface.h"
#include "thingy_service.h"
#include "motion_service.h"
#include "ui_service.h"
#include "config_service.h"
#include "environment_service.h"




class thingy_manager
{
public:
    thingy_manager();
    void handle(QPair<QLowEnergyCharacteristic, QByteArray> data);
private:
    motion_service *motion_serv;
    ui_service *ui_serv;
    config_service *config_serv;
    environment_service *environment_serv;
    thingyServiceType parseCharactType(QLowEnergyCharacteristic);
    QList<thingy_service*> thingyServiceList;
    QList<QLowEnergyService> bleServiceList;
};

#endif // THINGY_MANAGER_H
