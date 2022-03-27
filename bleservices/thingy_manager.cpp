#include "thingy_manager.h"

thingy_manager::thingy_manager(QObject *parent):
     QObject(parent)
{

    motion_serv = new motion_service("motionService",motionService);
    ui_serv = new ui_service("uiService",uiService);
    config_serv = new config_service("configService",configService);
    environment_serv = new environment_service("EnvironmentService",environmentService);

    thingyServiceList.append(motion_serv);
    thingyServiceList.append(ui_serv);
    thingyServiceList.append(config_serv);
    thingyServiceList.append(environment_serv);

}

thingyServiceType thingy_manager::parseCharactType(QLowEnergyCharacteristic ch){
    //service_list
    for(int i=0;i<thingyServiceList.length();i++){
        thingy_service* Srv = thingyServiceList[i];
        QList<QBluetoothUuid> ch_list = Srv->get_charactUUID_list();
        for (int i = 0; i < ch_list.length(); i++) {
            if(ch_list[i] == ch.uuid()){
                //return the service type;
                return Srv->get_thingType();
            }
        }
    }
    return NoneService;

}


void thingy_manager::handle(QPair<QLowEnergyCharacteristic, QByteArray> data){
    uint8_t handleServiceType=0;
    Points3D acc;
    //determine which service used
    QLowEnergyCharacteristic charact = data.first;
    QByteArray bytes = data.second;
    handleServiceType = parseCharactType(charact);

    switch (handleServiceType) {
    case configService:

        break;

    case environmentService:
        environment_serv->read_handler(charact, bytes);
        break;

    case uiService:
        break;

    case motionService:

        motion_serv->read_handler(charact, bytes);
        acc = motion_serv->getAccValue();
        emit onAccChanged(acc);

        break;

    default:
        break;
    }

}
