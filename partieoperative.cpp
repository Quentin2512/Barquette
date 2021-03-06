#include "PartieOperative.h"

PartieOperative::PartieOperative(QHostAddress _adresseIp, quint16 _port, quint8 _esclaveId, QObject *parent):QObject(parent)
{
    modBusTCP=new ModBusTCP(_adresseIp,_port,_esclaveId);
    if( !connect(modBusTCP,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_socketChanged(QAbstractSocket::SocketState))) )
        qDebug() << "Erreur connexion slot on_socketChanged";

    for(int x=0;x<NB_CAPTEURS;x++)
        lesEjecteurs[x]=new Ejecteur(x,*modBusTCP);

    timerPO=new QTimer;
    if( !connect(timerPO,SIGNAL(timeout()),this,SLOT(on_finTimerPO())) )
        qDebug() << "Erreur connexion slot on_finTimerPO";
    leTapis=new Tapis((quint16)TAPIS, *modBusTCP);
    lesCapteurs=new Capteurs(0, *modBusTCP);
    connect(modBusTCP,SIGNAL(signalReponseReadInputRegister(quint8)),this,SLOT(on_signalChangementEtatCapteursReceived(quint8)));
}

PartieOperative::~PartieOperative()
{
    for(int x=0;x<NB_CAPTEURS;x++)
        delete lesEjecteurs[x];
    delete timerPO;
    delete modBusTCP;
    delete leTapis;
    delete lesCapteurs;
}

void PartieOperative::ejecterBarquette(quint8 numCapteur)
{
    switch (numCapteur) {
    case 0x01:
        lesEjecteurs[0]->piloter();
        break;
    case 0x02:
        lesEjecteurs[1]->piloter();
        break;
    case 0x04:
        lesEjecteurs[2]->piloter();
        break;
    case 0x08:
        lesEjecteurs[3]->piloter();
        break;
    default:
        break;
    }
}

void PartieOperative::on_finTimerPO()
{
    lesCapteurs->LireEtatCapteurs();
}

void PartieOperative::on_socketChanged(QAbstractSocket::SocketState socketEtat)
{
    emit stateChanged(socketEtat);
}

void PartieOperative::on_signalChangementEtatCapteursReceived(quint8 trame)
{
    emit signalChangementEtatCapteurs(trame);
}
