#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <iostream>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_bleInterface = new BLEInterface(this);

    connect(m_bleInterface, &BLEInterface::dataReceived,
            this, &MainWindow::dataReceived);
    connect(m_bleInterface, &BLEInterface::devicesNamesChanged,
            [this] (QStringList devices){
        ui->devicesComboBox->clear();
        ui->devicesComboBox->addItems(devices);
    });




    connect(m_bleInterface, &BLEInterface::servicesChanged,
            [this] (QStringList services){
        ui->servicesComboBox->clear();
        ui->servicesComboBox->addItems(services);
    });
    connect(m_bleInterface, &BLEInterface::statusInfoChanged,
            [this](QString info, bool){
        this->statusBar()->showMessage(info);
    });

    m_bleInterface->scanDevices();
    ui->plotwidget->addGraph();
//    ui->plotwidget->graph(0).
    ui->plotwidget->addGraph();
    ui->plotwidget->addGraph();
    ui->plotwidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plotwidget->xAxis->setLabel("x");
    ui->plotwidget->xAxis->setLabel("y");
    ui->plotwidget->graph(0)->setPen(QPen(QColor(255, 0, 0)));
    ui->plotwidget->graph(1)->setPen(QPen(QColor(0, 255, 0)));
    ui->plotwidget->graph(2)->setPen(QPen(QColor(0, 0, 255)));

    ui->plotwidget->graph(0)->setName("x");
    ui->plotwidget->graph(1)->setName("y");
    ui->plotwidget->graph(2)->setName("z");
    ui->plotwidget->legend->setVisible(true);
    // set axes ranges, so we see all data:
    count = 0;
    ui->plotwidget->yAxis->setRange(-3000, 3000);
    ui->plotwidget->xAxis->setRange(0, 2000);

    ui->battery_progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bleScanButton_clicked()
{
    m_bleInterface->scanDevices();

}

void MainWindow::on_connectButton_clicked()
{
    m_bleInterface->set_currentDevice(ui->devicesComboBox->currentIndex());
    m_bleInterface->connectCurrentDevice();


//    foreach (i, m_bleInterface->currentService()) {
//        printf("%s",i)
//    }
}


void MainWindow::on_sendButton_clicked()
{
    QByteArray data;
}
void MainWindow::dataReceived(QPair<QLowEnergyCharacteristic, QByteArray> data){

    thing_manager.handle(data);
//    if(data.first.uuid()==
//            QBluetoothUuid(QString("00002a19-0000-1000-8000-00805f9b34fb"))
//    ){
//        uint8_t battery_info = data.second[0];
//        printf("%d\n",battery_info);
//    }



//    if(data.first.uuid()==
//            QBluetoothUuid(QString("ef680303-9b35-4933-9b10-52ffa9740042"))
//    )
//    {
//        qDebug()<<(data.first.uuid().toString());
//        qDebug()<< data.second;
//    }
//    if(data.first.uuid()==
//            QBluetoothUuid(QString("ef680302-9b35-4933-9b10-52ffa9740042"))
//    )
//    {
//        qDebug()<<(data.first.uuid().toString());
//        qDebug()<< data.second;
//    }

//    // raw data
//    if(data.first.uuid()==
//            QBluetoothUuid(QString(Motion_Raw_Charact_UUID))
//    )
//    {
//        //qDebug()<<(data.first.uuid().toString());
//        QByteArray imu_byte_data=data.second;
//        //qDebug()<< data.second;

//        //int16_t accx = ((uint16_t)imu_byte_data[0] + ((uint16_t)imu_byte_data[1]<<8));
//        //accx &= 0xfc00;
//        //11111100 00000000
//        //std::cout<< accx<<std::endl;
//         int16_t* vec_x, *vec_y, *vec_z;
//         uint8_t lenx[2], leny[2], lenz[2] ;
//         for(unsigned int i=0;i<2;i++){
//            lenx[i] = imu_byte_data[i];
//            leny[i] = imu_byte_data[i+2];
//            lenz[i] = imu_byte_data[i+4];
//         }
//         vec_x = (int16_t*)&lenx[0];
//         vec_y = (int16_t*)&leny[0];
//         vec_z = (int16_t*)&lenz[0];
//         qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
//         //QDateTime::currentSecsSinceEpoch();
//         ui->plotwidget->graph(0)->addData(count, *vec_x);
//         ui->plotwidget->graph(1)->addData(count, *vec_y);
//         ui->plotwidget->graph(2)->addData(count, *vec_z);
//         QCPRange range;
//         bool ok=true;
//         range = ui->plotwidget->graph(0)->data()->valueRange(ok);

//         ui->plotwidget->yAxis->setRange(range);

//         count++;
//         if(count>1000){

//             ui->plotwidget->graph(0)->data()->removeBefore(count-1000);
//             ui->plotwidget->graph(1)->data()->removeBefore(count-1000);
//             ui->plotwidget->graph(2)->data()->removeBefore(count-1000);
//             ui->plotwidget->xAxis->setRange(count-1000, count+500);
//         }
//         ui->plotwidget->replot();
//         //printf("%ld, %d, %d, %d\n", timestamp, *vec_x, *vec_y, *vec_z);
//    }
//    if(data.first.uuid()==
//            QBluetoothUuid(QString("ef680405-9b35-4933-9b10-52ffa9740042"))
//    )
//    {
//        //qDebug()<<(data.first.uuid().toString());
//        QByteArray step_data=data.second;
//        uint32_t *steps, *time;
//        uint8_t len_s[4], len_t[4];

//        for(uint8_t i=0;i<4;i++){
//            len_s[i] = step_data[i];
//            len_t[i] = step_data[i+4];
//        }
//        steps = (uint32_t*)&len_s[0];
//        time = (uint32_t*)&len_t[4];
//        printf("step %d,time %d\n",*steps,*time);
//    }

//    if(data.first.uuid()==
//            QBluetoothUuid(QString("ef68040A-9b35-4933-9b10-52ffa9740042"))
//    )
//    {

//        QByteArray imu_byte_data=data.second;

//         float* vec_x, *vec_y,*vec_z;
//         uint8_t lenx[4],leny[4],lenz[4];
//         for(unsigned int i=0;i<4;i++){
//            lenx[i] = imu_byte_data[i];
//            leny[i] = imu_byte_data[i+4];
//            lenz[i] = imu_byte_data[i+8];
//         }
//         vec_x = (float*)&lenx[0];
//         vec_y = (float*)&leny[0];
//         vec_z = (float*)&lenz[0];
////         printf("x=%f, y=%f, z=%f\n", *vec_x, *vec_y, *vec_z);
//    }



}

void MainWindow::on_servicesComboBox_currentIndexChanged(int index)
{
    m_bleInterface->setCurrentService(index);
}

void MainWindow::on_list_ch_Button_clicked()
{



//    if(m_bleInterface->isConnected()){
//        m_bleInterface->list_Charateristics();
//    }
}
