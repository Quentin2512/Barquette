#ifndef CAPTEURS_H
#define CAPTEURS_H

#include <QObject>
#include "modbustcp.h"

class Capteurs : public QObject
{
    Q_OBJECT
public:
    Capteurs(quint16 _adresseCapteurs, ModBusTCP &_modBusTCP, QObject *parent = 0);
    void LireEtatCapteurs();

private:
    quint16 adresseCapteurs;
    ModBusTCP &modBusTCP;
};

#endif // CAPTEURS_H
