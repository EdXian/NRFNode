#ifndef THINGY_MANAGER_H
#define THINGY_MANAGER_H

#include "bleinterface.h"
#include "thingy_service.h"
#include "motion_service.h"
#include "ui_service.h"
#include "config_service.h"
#include "environment_service.h"

#include <QObject>



class thingy_manager : public QObject
{
    Q_OBJECT
public:

    explicit thingy_manager(QObject *parent);

    void handle(QPair<QLowEnergyCharacteristic, QByteArray> data);
    motion_service *motion_serv;

signals:
    void onAccChanged(Points3D data);


private:

    ui_service *ui_serv;
    config_service *config_serv;
    environment_service *environment_serv;
    thingyServiceType parseCharactType(QLowEnergyCharacteristic);
    QList<thingy_service*> thingyServiceList;
    QList<QLowEnergyService> bleServiceList;
};

#endif // THINGY_MANAGER_H
