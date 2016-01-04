#include "PartieOperative.h"

PartieOperative::PartieOperative(QHostAddress _adresseIp, quint16 _port, quint8 _esclaveId, QObject *parent):QObject(parent)
{
    modBusTCP=new ModBusTCP(_adresseIp,_port,_esclaveId);
    connect(modBusTCP,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_socketChanged(QAbstractSocket::SocketState)));

    for(int x=0;x<NB_CAPTEURS;x++)
        lesEjecteurs[x]=new Ejecteur(x,*modBusTCP);

    timerPO=new QTimer;
    connect(timerPO,SIGNAL(timeout()),this,SLOT(on_finTimer()));
    capteurActuel=-1;
    octet=0;
    leTapis=new Tapis((quint16)TAPIS, *modBusTCP);
}

PartieOperative::~PartieOperative()
{
    for(int x=0;x<NB_CAPTEURS;x++)
        delete lesEjecteurs[x];
    delete timerPO;
    delete modBusTCP;
    delete leTapis;
}

void PartieOperative::ejecterBarquette(int numCapteur)
{
    lesEjecteurs[numCapteur]->piloter();
}

/*void PartieOperative::verifCapteurs()
{
    qint8 tempOctet=octet;
    //bool verif=false;
    if(capteurs[0])
    {
        octet |= 0x01;
    }
    else
    {
        octet &= ~0x01;
        if(tempOctet!=octet)
            verif=true;
    }

    if(capteurs[1])
         octet |= 0x02;
    else octet &= ~0x02;

    if(capteurs[2])
         octet |= 0x04;
    else octet &= ~0x04;

    if(capteurs[3])
         octet |= 0x08;
    else octet &= ~0x08;

    if(tempOctet!=octet){
        emit capteurChange(octet);
    }
}*/

void PartieOperative::on_finTimer()
{

}

void PartieOperative::on_socketChanged(QAbstractSocket::SocketState socketEtat)
{
    emit stateChanged(socketEtat);
}
