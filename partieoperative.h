#ifndef PARTIEOPERATIVE_H
#define PARTIEOPERATIVE_H

#include <QObject>
#include <QTimer>
#include "modbustcp.h"
#include "tapis.h"

#define NB_CAPTEURS 4

enum{
    CAPTEUR1=1,
    CAPTEUR2,
    CAPTEUR3,
    CAPTEUR4,
    TAPIS
};

class PartieOperative: public QObject
{
    Q_OBJECT
public:
    PartieOperative(const int idEsclave, QObject *parent=0);
    ~PartieOperative();
    void ejecterBarquette(int numEjecteur);
    void demarrerTapis();
    void arreterTapis();
    void lancerProduction();

signals:
    void signalChangementEtatCapteurs(quint8);

private slots:
    void on_finTimer();

private:
    ModBusTCP *modbusTCP;
    QTimer *timerPO;
    std::string etatCapteurs;
    Tapis *leTapis;
    int capteurActuel;
    qint8 octet;
    bool capteurs[NB_CAPTEURS];
};

#endif // PARTIEOPERATIVE_H
