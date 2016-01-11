#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Barquette.sqlite");
    if( db.open() )
        qDebug() << "Ouverture de la base de donnees avec succes";
    else qDebug() << "Echec d'ouverture de la base de donnees";

    laPO = NULL;
}


MainWindow::~MainWindow()
{
    if(laPO != NULL)
        delete laPO;
    qDeleteAll(listeBarquettes);
    listeBarquettes.clear();
    qDeleteAll(fileBarquettes);
    fileBarquettes.clear();
    delete ui;
}

void MainWindow::lancerProduction()
{
    if( ui->pushButton_lancerProd->text() == "Lancer la production" ){
        ui->progressBar->setMaximum(0);
        ui->pushButton_lancerProd->setText("Arrêter la production");
        laPO->lancerProduction();
        ui->groupBox_connexion->setEnabled(false);
        ui->lineEdit_codeProduit->setFocus();
        if( !connect(laPO,SIGNAL(signalChangementEtatCapteurs(quint8)),this,SLOT(on_etatCapteurChanged(quint8))) )
            qDebug() << "Erreur connexion slot on_etatCapteurChanged";
    }else{
        ui->pushButton_lancerProd->setText("Lancer la production");
        ui->progressBar->setMaximum(1);
        ui->lineEdit_codeProduit->clear();
        ui->listWidget_barquettes->clear();
        laPO->arreterProduction();
        ui->groupBox_connexion->setEnabled(true);
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
        ui->pushButton_connexion->setText("Connexion");
        ui->pushButton_lancerProd->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::on_etatCapteurChanged(quint8 trame)
{
   if((trame&0x01)==0x01){
       if(ui->checkBox_capteur1->isChecked()){
           if(!fileBarquettes.isEmpty()){
               listeBarquettes.append(fileBarquettes.dequeue());
               listeBarquettes.last()->start();
           }
       }else{
           ui->checkBox_capteur1->setChecked(true);
       }
   }else ui->checkBox_capteur1->setChecked(false);

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

void MainWindow::on_lineEdit_codeProduit_textChanged(const QString &arg1)
{
    if(arg1.length()==13){
        ui->lineEdit_codeProduit->setSelection(0,13);
        QSqlQuery query;
        query.prepare("select idBarquette from Ejection where CodeBarre like :codebarre");
        query.bindValue(":codebarre", ui->lineEdit_codeProduit->text());
        quint8 retourEjecteurBD;
        if(query.exec()){
            while(query.next()){
                switch (query.value(0).toInt()) {
                case 1:
                    retourEjecteurBD = 0x01;
                    break;
                case 2:
                    retourEjecteurBD = 0x02;
                    break;
                case 3:
                    retourEjecteurBD = 0x04;
                    break;
                case 4:
                    retourEjecteurBD = 0x08;
                    break;
                default:
                    break;
                }
            }
        } else qDebug() << "Echec de la requete";
        Barquette *pBarquette=new Barquette(retourEjecteurBD,arg1);
        if( !connect(pBarquette,SIGNAL(signalEjecteurTrouve(quint8)),laPO,SLOT(ejecterBarquette(quint8))) )
            qDebug() << "Erreur connect ejecterBarquette";
        if( !connect(laPO,SIGNAL(signalChangementEtatCapteurs(quint8)),pBarquette,SLOT(on_changementEtatCapteurs(quint8))) )
            qDebug() << "Erreur connect on_changementEtatCapteurs";
        if( !connect(pBarquette,SIGNAL(signalBarquetteEjectee(quint8,QString)),this,SLOT(on_barquetteEjectee(quint8,QString))) )
            qDebug() << "Erreur connect on_barquetteEjectee";
        ui->listWidget_barquettes->addItem(arg1);
        fileBarquettes.enqueue(pBarquette);
    }
}

void MainWindow::on_barquetteEjectee(quint8 ejecteur, QString nom)
{
    int x=0;
    bool verif=false;
    while(x<ui->listWidget_barquettes->count() || !verif){
        if(ui->listWidget_barquettes->item(x)->text()==nom){
            delete ui->listWidget_barquettes->item(x);
            verif=true;
        }
    }

    switch (ejecteur) {
    case 0x01:
        ui->lcdNumber_sortie1->display(ui->lcdNumber_sortie1->value()+1);
        break;
    case 0x02:
        ui->lcdNumber_sortie2->display(ui->lcdNumber_sortie2->value()+1);
        break;
    case 0x04:
        ui->lcdNumber_sortie3->display(ui->lcdNumber_sortie3->value()+1);
        break;
    case 0x08:
        ui->lcdNumber_sortie4->display(ui->lcdNumber_sortie4->value()+1);
        break;
    default:
        break;
    }
    listeBarquettes.removeOne((Barquette*)sender());
}
