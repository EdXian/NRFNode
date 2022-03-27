#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bleinterface.h"

#include "bleservices/motion_service.h"
#include "bleservices/thingy_manager.h"
#include "plotthread/plotthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //motion_service motion_ser;
private slots:
    void on_bleScanButton_clicked();

    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_servicesComboBox_currentIndexChanged(int index);

    void on_list_ch_Button_clicked();
    void onrecv(Points3D data);
private:
    uint32_t count;
    Ui::MainWindow *ui;
    BLEInterface *m_bleInterface;
    void dataReceived(QPair<QLowEnergyCharacteristic, QByteArray> data);
    //motion_service motion_ser;
    thingy_manager* thing_manager;
    plotthread *plot_t;
    QThread *thread;
};

#endif // MAINWINDOW_H
