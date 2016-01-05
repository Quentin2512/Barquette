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

void PartieOperative::ejecterBarquette(int numCapteur)
{
    lesEjecteurs[numCapteur]->piloter();
}

void PartieOperative::on_finTimerPO()
{

}

void PartieOperative::on_socketChanged(QAbstractSocket::SocketState socketEtat)
{
    emit stateChanged(socketEtat);
}
