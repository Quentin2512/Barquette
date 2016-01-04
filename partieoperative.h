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
    void ejecterBarquette(int numEjecteur);
    void lancerProduction(){leTapis->commander(true);timerPO->start(250);}
    void arreterProduction(){leTapis->commander(false);timerPO->stop();}

signals:
    void signalChangementEtatCapteurs(quint8);
    void stateChanged(QAbstractSocket::SocketState);

private slots:
    void on_finTimerPO();

    void on_socketChanged(QAbstractSocket::SocketState socketEtat);
private:
    ModBusTCP *modBusTCP;
    QTimer *timerPO;
    std::string etatCapteurs;
    Tapis *leTapis;
    int capteurActuel;
    qint8 octet;
    bool capteurs[NB_CAPTEURS];
    Ejecteur *lesEjecteurs[NB_CAPTEURS];
    Capteurs *lesCapteurs;
};

#endif // PARTIEOPERATIVE_H
