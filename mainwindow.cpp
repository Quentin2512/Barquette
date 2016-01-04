#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int x=0;x<QSerialPortInfo::availablePorts().length();x++)
        ui->comboBox_portLecteur->addItem(QSerialPortInfo::availablePorts()[x].portName());

    laPO=NULL;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lancerProduction()
{
    laPO->lancerProduction();
}

void MainWindow::connecterPO()
{
    if(laPO==NULL)
        laPO = new PartieOperative(QHostAddress(ui->lineEdit_ipEsclave->text()),ui->spinBox_portTCP->value(),ui->spinBox_idEsclave->value());
}

