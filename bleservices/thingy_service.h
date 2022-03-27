#ifndef THINGY_SERVICE_H
#define THINGY_SERVICE_H
#include <bleinterface.h>
#include <QString>

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#define _CONCAT(x,y) x##y
#define CONCAT(x,y) _CONCAT(x,y)

#define Battery_Srv_UUID QBluetoothUuid::serviceClassToString(QBluetoothUuid::ServiceClassUuid::BatteryService)
#define Battery_Ch_UUID "00002a19-0000-1000-8000-00805f9b34fb"


//motion service
#define Motion_Srv_UUID                 "EF680400-9B35-4933-9B10-52FFA9740042"
#define Motion_Config_Charact_UUID      "EF680401-9B35-4933-9B10-52FFA9740042"
#define Motion_Tap_Charact_UUID         "EF680402-9B35-4933-9B10-52FFA9740042"
#define Motion_Orient_Charact_UUID      "EF680403-9B35-4933-9B10-52FFA9740042"
#define Motion_Quat_Charact_UUID        "EF680404-9B35-4933-9B10-52FFA9740042"
#define Motion_Step_Charact_UUID        "EF680405-9B35-4933-9B10-52FFA9740042"
#define Motion_Raw_Charact_UUID         "EF680406-9B35-4933-9B10-52FFA9740042"
#define Motion_Euler_Charact_UUID       "EF680407-9B35-4933-9B10-52FFA9740042"
#define Motion_Rotation_Charact_UUID    "EF680408-9B35-4933-9B10-52FFA9740042"
#define Motion_Heading_Charact_UUID     "EF680409-9B35-4933-9B10-52FFA9740042"
#define Motion_Gravity_Charact_UUID     "EF68040A-9B35-4933-9B10-52FFA9740042"


//Environment service

#define Env_Srv_UUID                    "EF680200-9B35-4933-9B10-52FFA9740042"

#define Env_Temperature_Charat_UUID     "EF680201-9B35-4933-9B10-52FFA9740042"
#define Env_Pressure_Charat_UUID        "EF680202-9B35-4933-9B10-52FFA9740042"
#define Env_Humidity_Charat_UUID        "EF680203-9B35-4933-9B10-52FFA9740042"
#define Env_Air_Charat_UUID             "EF680204-9B35-4933-9B10-52FFA9740042"
#define Env_Color_UUID                  "EF680205-9B35-4933-9B10-52FFA9740042"
#define Env_Configuration_UUID          "EF680206-9B35-4933-9B10-52FFA9740042"


enum thingyServiceType{
    NoneService=0,
    configService,
    motionService,
    uiService,
    environmentService
};

class thingy_service
{
public:
    thingy_service(QString data, thingyServiceType thinType_):
    serviceName(data),
    thinType(thinType_)
    {

    }
    virtual void read_handler(){

    }
    virtual void write_handler(){

    }
    virtual void notify_handler(){

    }
    void service_configuration();
    void append_charact(QBluetoothUuid uuid){
        charact_uuid_list.append(uuid);
    }
    void set_service_uuid(QBluetoothUuid uuid){
        this->service_uuid = uuid;
    }
    QBluetoothUuid get_service_uuid(){
        return service_uuid;
    }
    QList<QBluetoothUuid> get_charactUUID_list(){
        return charact_uuid_list;
    }
    thingyServiceType get_thingType(){
        return thinType;
    }
    QString serviceName;
private:
    thingyServiceType thinType;
    QBluetoothUuid service_uuid;
    QList<QLowEnergyCharacteristic> charact_list;
    QList<QBluetoothUuid>  charact_uuid_list;

};

#endif // THINGY_SERVICE_H
