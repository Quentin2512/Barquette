#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int x=0;x<QSerialPortInfo::availablePorts().length();x++)
        ui->comboBox_portLecteur->addItem(QSerialPortInfo::availablePorts()[x].portName());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lancerProduction()
{

}

void MainWindow::connecterPO()
{
    laPO = new PartieOperative(ui->spinBox_idEsclave->value());
}

