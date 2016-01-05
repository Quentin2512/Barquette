#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int x=0;x<QSerialPortInfo::availablePorts().length();x++)
        ui->comboBox_portLecteur->addItem(QSerialPortInfo::availablePorts()[x].description()+" - "+QSerialPortInfo::availablePorts()[x].portName());

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
        if( !connect(laPO,SIGNAL(signalChangementEtatCapteurs(quint8)),this,SLOT(on_etatCapteurChanged(qint8))) )
            qDebug() << "Erreur connexion slot on_etatCapteurChanged";
    }else{
        ui->pushButton_lancerProd->setText("Lancer la production");
        laPO->arreterProduction();
    }
}

void MainWindow::on_socketChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::ConnectingState:
        ui->listWidget_etatConnexion->addItem("Connexion en cours...");
        break;
    case QAbstractSocket::ConnectedState:
        ui->listWidget_etatConnexion->addItem("Connecté");
        ui->pushButton_connexion->setText("Déconnexion");
        ui->pushButton_lancerProd->setEnabled(true);
        break;
    case QAbstractSocket::UnconnectedState:
        ui->listWidget_etatConnexion->addItem("Déconnecté");
        ui->pushButton_lancerProd->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::on_etatCapteurChanged(quint8 trame)
{
   if((trame&0x01)==0x01)
       ui->checkBox_capteur1->setChecked(true);
   else ui->checkBox_capteur1->setChecked(false);

   if((trame&0x02)==0x02)
       ui->checkBox_capteur2->setChecked(true);
   else ui->checkBox_capteur2->setChecked(false);

   if((trame&0x04)==0x04)
       ui->checkBox_capteur3->setChecked(true);
   else ui->checkBox_capteur3->setChecked(false);

   if((trame&0x08)==0x08)
       ui->checkBox_capteur4->setChecked(true);
   else ui->checkBox_capteur4->setChecked(false);
}

void MainWindow::connecterPO()
{
    if( ui->pushButton_connexion->text() == "Connexion" )
    {
        if(laPO==NULL){
            laPO = new PartieOperative(QHostAddress(ui->lineEdit_ipEsclave->text()),ui->spinBox_portTCP->value(),ui->spinBox_idEsclave->value());
            if( !connect(laPO,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_socketChanged(QAbstractSocket::SocketState))) )
                qDebug() << "Erreur connexion slot on_socketChanged";
        }
    }else{
        if(ui->pushButton_lancerProd->text()=="Arrêter la production"){
            lancerProduction();
        }
        laPO->arreterProduction();
        ui->pushButton_connexion->setText("Connexion");
        delete laPO;
        laPO=NULL;
    }
}
