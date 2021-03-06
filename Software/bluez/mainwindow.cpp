#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <iostream>
#include <QDateTime>

//Q_DECLARE_METATYPE(Points3D_t)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //qRegisterMetaType<Points3D>();
    //qRegisterMetaType<Points3D_t>( "Points3D_t" );

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


    saveFileEnable = false;
    saveFilePath = QString(QCoreApplication::applicationDirPath()); //get the current path
    qDebug()<<saveFilePath ;
    m_bleInterface->scanDevices();
    thing_manager= new thingy_manager(0);
    plot_t = new plotthread();
    thread = new QThread();
    plot_t->moveToThread(thread);
    thread->start();

    plot_t->setPlotter(ui->plotwidget);
    plot_t->configPlotter();

    count = 0;
    ui->battery_progressBar->setValue(0);

//    connect(thing_manager,SIGNAL(thingy_manager::onAccChanged(Points3D)),
//            plot_t,SLOT(plotthread::onAccDataReceieved(Points3D)));

    connect(thing_manager,&thingy_manager::onAccChanged,
            this,&MainWindow::onrecv);
}
void MainWindow::onrecv(Points3D data){
    plot_t->onAccDataReceieved(data);

    QString line;
    line = QString("%1, %2, %3\n").arg(data.x, 0, 'f', 2 ).arg(data.y, 0, 'f', 2 ).arg(data.z, 0, 'f', 2 );
    //qDebug()<<line;
    if(saveFileEnable){
        if(outFile!=nullptr){
            if (outFile->isOpen())
            {
                 outFile->write(line.toStdString().c_str());
            }
        }
    }


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
}


void MainWindow::on_sendButton_clicked()
{
    QByteArray data;
}
void MainWindow::dataReceived(QPair<QLowEnergyCharacteristic, QByteArray> data){

    thing_manager->handle(data);

   // plot_t->update_plot();
}

void MainWindow::on_servicesComboBox_currentIndexChanged(int index)
{
    m_bleInterface->setCurrentService(index);
}

void MainWindow::on_list_ch_Button_clicked()
{

}

void MainWindow::on_record_pushButton_clicked()
{
   saveFileEnable = true;
   if(saveFileEnable){
     outFile = new QFile("record.txt");
     if (outFile->open(QIODevice::WriteOnly))
     {




     }

   }
}



void MainWindow::on_recordSetButton_clicked()
{
    saveFilePath = QFileDialog::getExistingDirectory();

}
