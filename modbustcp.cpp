#include "modbustcp.h"

quint16 ModBusTCP::transactionId=0;

ModBusTCP::ModBusTCP(int _idEsclave, QObject *parent) : QObject(parent),
  idEsclave(_idEsclave)
{
    pSocket = new QTcpSocket();
    connect(pSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(on_socketChanged(QAbstractSocket::SocketState)));
    connect(pSocket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
}

ModBusTCP::~ModBusTCP()
{
    delete pSocket;
}

void ModBusTCP::WriteSingleCoils(bool donnee, int reference)
{
    QDataStream out(pSocket);
    debutTrame();
    out<<(quint8)FC_WRITE_COILS;
    out<<(quint16)reference;
    if(donnee)
    {
        out<<(quint8)0xFF;
        out<<(quint8)0;
    }else{
        out<<(quint16)0;
    }
}

void ModBusTCP::ReadInputRegister(quint16 reference)
{
    QDataStream out(pSocket);
    debutTrame();
    out<<(quint8)FC_READ_INPUT_REGISTERS;
    out<<reference;
    out<<(quint16)1;
}

void ModBusTCP::DeconnecterEsclaveModBus()
{
    pSocket->disconnectFromHost();
}

void ModBusTCP::debutTrame()
{
     QDataStream out(pSocket);
     out<<transactionId++;
     out<<(quint16)0;
     out<<(quint16)6;
     out<<(quint8)idEsclave;
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

void ModBusTCP::ConnecterEsclaveModBus(QString adresseIp, int port)
{
    pSocket->connectToHost(adresseIp, port);
}

