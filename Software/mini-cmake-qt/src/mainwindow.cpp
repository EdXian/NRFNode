#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    printf("tesffft\n");
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::handleButton);
}



MainWindow::~MainWindow()
{

}

void MainWindow::handleButton(){

    printf("test\n");
    ui->pushButton->setText("test");
}
