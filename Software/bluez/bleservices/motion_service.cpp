#include "motion_service.h"




motion_service::motion_service(QString Name, thingyServiceType thingType_) :
    thingy_service(Name,thingType_)
{

    this->set_service_uuid(QBluetoothUuid(QString(Motion_Srv_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Config_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Tap_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Orient_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Quat_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Step_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Raw_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Euler_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Rotation_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Heading_Charact_UUID)));
    this->append_charact(QBluetoothUuid(QString(Motion_Gravity_Charact_UUID)));

}

Points3D motion_service::getAccValue(){
    return Acc;
}


void motion_service::read_handler(QLowEnergyCharacteristic ch, QByteArray bytes){
    if(ch.uuid()==
            QBluetoothUuid(QString(Motion_Raw_Charact_UUID))
    )
    {
        QByteArray imu_byte_data=bytes;
         int16_t* vec_x, *vec_y, *vec_z;
         uint8_t lenx[2], leny[2], lenz[2] ;
         for(unsigned int i=0;i<2;i++){
            lenx[i] = imu_byte_data[i];
            leny[i] = imu_byte_data[i+2];
            lenz[i] = imu_byte_data[i+4];
         }
         vec_x = (int16_t*)&lenx[0];
         vec_y = (int16_t*)&leny[0];
         vec_z = (int16_t*)&lenz[0];

         //uint32_t timestamp=0;
         //printf("%ld, %d, %d, %d\n", timestamp, *vec_x, *vec_y, *vec_z);

         Acc.x = *vec_x;
         Acc.y = *vec_y;
         Acc.z = *vec_z;

    }
}



