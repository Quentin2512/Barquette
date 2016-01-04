#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QDataStream>

enum CODE_FONCTION
{
    FC_READ_COILS=1,
    FC_READ_INPUT_DISCRETES,
    FC_READ_MULTIPLE_REGISTERS,
    FC_READ_INPUT_REGISTERS,
    FC_WRITE_COILS,
    FC_WRITE_SINGLE_REGISTER
};

#define MODBUS_TCP_HEADER_LENGTH 6
#define CODE_EXCEPTION 0x80

class ModBusTCP : public QObject
{
    Q_OBJECT
public:
    explicit ModBusTCP(const int _idEsclave,QObject *parent = 0);
    ~ModBusTCP();
    void WriteSingleCoils(bool donnee, int reference);
    void ReadInputRegister(quint16 reference);
    void ConnecterEsclaveModBus(QString adresseIp, int port);
    void DeconnecterEsclaveModBus();
    void debutTrame();

signals:
    void stateChanged(QAbstractSocket::SocketState);

    void resultat(QString);

    void signalReponseWriteSingleCoils();

    void signalReponseReadInputRegister(QString);

private slots:
    void on_socketChanged(QAbstractSocket::SocketState socketEtat);

    void on_readyRead();

private:
    QTcpSocket *pSocket;
    QHostAddress adresseIp;
    quint16 port,idEsclave;
    static quint16 transactionId;
};

#endif // MODBUSTCP_H
