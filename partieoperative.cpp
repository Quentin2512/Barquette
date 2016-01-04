#include "PartieOperative.h"

PartieOperative::PartieOperative(const int idEsclave, QObject *parent):QObject(parent)
{
    modbusTCP=new ModBusTCP(idEsclave);
    timerPO=new QTimer;
    connect(timerPO,SIGNAL(timeout()),this,SLOT(on_finTimer()));
    capteurActuel=-1;
    octet=0;
    leTapis=new Tapis((quint16)TAPIS);
}

PartieOperative::~PartieOperative()
{
    delete timerPO;
    delete modbusTCP;
}

void PartieOperative::ejecterBarquette(int numCapteur)
{
    numCapteur=capteurActuel;
    modbusTCP->WriteSingleCoils(true,numCapteur);
}

void PartieOperative::lancerProduction()
{
    leTapis->commander(true);
    timerPO->start(250);
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
