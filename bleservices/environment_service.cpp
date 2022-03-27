#include "environment_service.h"

environment_service::environment_service(QString Name, thingyServiceType thingType_) :
    thingy_service(Name,thingType_)
{
    this->set_service_uuid(QBluetoothUuid(QString(Env_Srv_UUID)));
    this->append_charact(QBluetoothUuid(QString(Env_Temperature_Charat_UUID)));
    this->append_charact(QBluetoothUuid(QString(Env_Pressure_Charat_UUID)));
    this->append_charact(QBluetoothUuid(QString(Env_Humidity_Charat_UUID)));
    this->append_charact(QBluetoothUuid(QString(Env_Air_Charat_UUID)));
    this->append_charact(QBluetoothUuid(QString(Env_Color_UUID)));
    this->append_charact(QBluetoothUuid(QString(Env_Configuration_UUID)));

}

void environment_service::read_handler(QLowEnergyCharacteristic ch, QByteArray bytes){
    if(ch.uuid()==
            QBluetoothUuid(QString(Env_Temperature_Charat_UUID))
    )
    {
          QByteArray  temp_bytes = bytes;
          int8_t* tmp;
          uint8_t tmp_buf[2];
         for(unsigned int i=0;i<2;i++){
            tmp_buf[i] = temp_bytes[i];
         }
         tmp = (int8_t*)&tmp_buf[0];
         printf("%d, %d\n",tmp_buf[0],tmp_buf[1]);
    }
    else if(ch.uuid()==
            QBluetoothUuid(QString(Env_Pressure_Charat_UUID))
    )
    {
          QByteArray  temp_bytes = bytes;
          int32_t* pressure;
          float p=0.0;
          uint8_t tmp_buf[5];
         for(unsigned int i=0;i<5;i++){
            tmp_buf[i] = temp_bytes[i];
         }
         pressure = (int32_t*)&tmp_buf[0];
         p = (float)*pressure + (float)tmp_buf[4]*0.01;
         printf("%d \n",*pressure);
    }
    else if(ch.uuid()==
            QBluetoothUuid(QString(Env_Humidity_Charat_UUID))
    )
    {
          QByteArray  temp_bytes = bytes;
          uint8_t* humidity;
          float p=0.0;
          uint8_t tmp_buf[1];
          tmp_buf[0] = temp_bytes[0];
          humidity = (uint8_t*)&tmp_buf[0];

         printf("%d \n",*humidity);
    }
    else if(ch.uuid()==
            QBluetoothUuid(QString(Env_Air_Charat_UUID))
    )
    {
          QByteArray  temp_bytes = bytes;
          uint16_t *eCO2, *TVOC;
          float p=0.0;
          uint8_t tmp_buf[4];
          for(unsigned int i=0;i<4;i++){
            tmp_buf[i] = temp_bytes[i];
          }
            eCO2 = (uint16_t*)&tmp_buf[0];
            TVOC = (uint16_t*)&tmp_buf[2];
         printf("%d \n",*eCO2, *TVOC);
    }
    else if(ch.uuid()==
            QBluetoothUuid(QString(Env_Color_UUID))
    )
    {
          QByteArray  temp_bytes = bytes;
          uint16_t* red, *ble, *green, *clear;

          uint8_t tmp_buf[8];
          for(unsigned int i=0;i<8;i++){
            tmp_buf[i] = temp_bytes[i];
          }
            red = (uint16_t*)&tmp_buf[0];
            green = (uint16_t*)&tmp_buf[2];
            ble = (uint16_t*)&tmp_buf[4];
            clear = (uint16_t*)&tmp_buf[6];
         printf("%d, %d, %d, %d\n",*red, *green,*ble,*clear);
    }
}
