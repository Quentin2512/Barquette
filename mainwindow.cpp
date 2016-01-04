#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int x=0;x<QSerialPortInfo::availablePorts().length();x++)
        ui->comboBox_portLecteur->addItem(QSerialPortInfo::availablePorts()[x].portName());

    laPO = NULL;
}


MainWindow::~MainWindow()
{
    if(laPO != NULL)
        delete laPO;
    delete ui;
}

void MainWindow::lancerProduction()
{
    if( ui->pushButton_lancerProd->text() == "Lancer la production" ){
        ui->pushButton_lancerProd->setText("Arrêter la production");
        laPO->lancerProduction();
    }
    else{
        ui->pushButton_lancerProd->setText("Lancer la production");
        laPO->arreterProduction();
    }
}

void MainWindow::on_socketChanged(QAbstractSocket::SocketState socketEtat)
{
    switch (socketEtat) {
    case QAbstractSocket::ConnectingState:
        ui->listWidget_etatConnexion->addItem("Connexion en cours...");
        break;
    case QAbstractSocket::ConnectedState:
        ui->listWidget_etatConnexion->addItem("Connecté");
        ui->pushButton_connexion->setText("Déconnexion");
        break;
    case QAbstractSocket::UnconnectedState:
        ui->listWidget_etatConnexion->addItem("Déconnecté");
        break;
    default:
        break;
    }
}

void MainWindow::connecterPO()
{
    if(laPO==NULL){
        laPO = new PartieOperative(QHostAddress(ui->lineEdit_ipEsclave->text()),ui->spinBox_portTCP->value(),ui->spinBox_idEsclave->value());
        connect(laPO,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_socketChanged(QAbstractSocket::SocketState)));
    }
    if( ui->pushButton_connexion->text() == "Connexion" )
    {

        if(laPO==NULL)
            laPO = new PartieOperative(QHostAddress(ui->lineEdit_ipEsclave->text()),ui->spinBox_portTCP->value(),ui->spinBox_idEsclave->value());
        else
            QMessageBox::critical(this,"Erreur connexion","Déjà connecté");
    }else{
        laPO->arreterProduction();
        ui->pushButton_connexion->setText("Connexion");
        delete laPO;
        laPO=NULL;
    }
}
