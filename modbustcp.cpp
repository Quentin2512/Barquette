#include "modbustcp.h"

quint16 ModBusTCP::transactionId=0;

ModBusTCP::ModBusTCP(QHostAddress _adresseIp, quint16 _port, quint8 _esclaveId, QObject *parent) : QObject(parent),
  adresseIp(_adresseIp),port(_port),esclaveId(_esclaveId)
{
    pSocket = new QTcpSocket(this);
    pSocket->connectToHost(adresseIp,port);
    if( !connect(pSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_socketChanged(QAbstractSocket::SocketState))) )
        qDebug() << "Erreur connexion slot on_socketChanged";
    if( !connect(pSocket,SIGNAL(readyRead()),this,SLOT(on_readyRead())) )
        qDebug() << "Erreur connexion slot on_readyRead";
}

ModBusTCP::~ModBusTCP()
{
    delete pSocket;
}

void ModBusTCP::WriteSingleCoils(qint16 _adresse, quint8 _valeur)
{
    QDataStream out(pSocket);
    debutTrame();
    out<<(quint8)FC_WRITE_COILS;
    out<<_adresse;
    if((bool)_valeur)
    {
        out<<(quint8)0xFF;
        out<<(quint8)0;
    }else{
        out<<(quint16)0;
    }
}

void ModBusTCP::ReadInputRegister(quint16 adresse)
{
    QDataStream out(pSocket);
    debutTrame();
    out<<(quint8)FC_READ_INPUT_REGISTERS;
    out<<adresse;
    out<<(quint16)1;
}

void ModBusTCP::debutTrame()
{
     QDataStream out(pSocket);
     out<<transactionId++;
     out<<(quint16)0;
     out<<(quint16)6;
     out<<(quint8)esclaveId;
}

void ModBusTCP::on_socketChanged(QAbstractSocket::SocketState socketEtat)
{
    emit stateChanged(socketEtat);
}

void ModBusTCP::on_readyRead()
{
    quint8 code;
    quint8 temp;
    quint8 taille;
    quint8 numEsclave;
    QDataStream in(pSocket);
    for(int x=0;x<5;x++){
        in >> temp;
    }
    in >> taille;
    in >> numEsclave;
    in >> code;
    for(int x=0;x<taille-2;x++){
        in >> temp;
    }

    switch (code) {
    case FC_READ_INPUT_REGISTERS:
        emit signalReponseReadInputRegister(QString::number(temp));
        break;
    case 0x81:
        if(temp==0x01)
            emit resultat("Erreur de read coils avec exception : Fonction illégale");
        else emit resultat("Erreur de read coils avec exception : Adresse illégale");
        break;
    case 0x84:
        if(temp==0x01)
            emit resultat("Erreur de read register avec exception : Fonction illégale");
        else emit resultat("Erreur de read register avec exception : Adresse illégale");
        break;
    case 0x85:
        if(temp==0x01)
            emit resultat("Erreur de write coils avec exception : Fonction illégale");
        else emit resultat("Erreur de write coils avec exception : Adresse illégale");
        break;
    case 0x86:
        if(temp==0x01)
            emit resultat("Erreur de write register avec exception : Fonction illégale");
        else emit resultat("Erreur de write register avec exception : Adresse illégale");
        break;
    default:
        break;
    }
}
