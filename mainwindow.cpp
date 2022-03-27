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
