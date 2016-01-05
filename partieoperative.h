#ifndef PARTIEOPERATIVE_H
#define PARTIEOPERATIVE_H

#include <QObject>
#include <QTimer>
#include "modbustcp.h"
#include "tapis.h"
#include "ejecteur.h"
#include "capteurs.h"

#define NB_CAPTEURS 4

enum{
    EJECTEUR1,
    EJECTEUR2,
    EJECTEUR3,
    EJECTEUR4,
    TAPIS
};

class PartieOperative: public QObject
{
    Q_OBJECT
public:
    PartieOperative(QHostAddress _adresseIp, quint16 _port,quint8 _esclaveId, QObject *parent=0);
    ~PartieOperative();
    void lancerProduction(){leTapis->commander(true);timerPO->start(250);}
    void arreterProduction(){leTapis->commander(false);timerPO->stop();}

signals:
    void signalChangementEtatCapteurs(quint8);
    void stateChanged(QAbstractSocket::SocketState);

private slots:
    void on_finTimerPO();

    void on_socketChanged(QAbstractSocket::SocketState socketEtat);

    void on_signalChangementEtatCapteursReceived(quint8 trame);

    void ejecterBarquette(quint8 numEjecteur);

private:
    ModBusTCP *modBusTCP;
    QTimer *timerPO;
    Tapis *leTapis;
    Ejecteur *lesEjecteurs[NB_CAPTEURS];
    Capteurs *lesCapteurs;
    quint8 etatCapteurs;
};

#endif // PARTIEOPERATIVE_H
