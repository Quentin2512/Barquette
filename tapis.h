#ifndef TAPIS_H
#define TAPIS_H

#include <QObject>
#include "modbustcp.h"

#define OFF 0x00
#define ON 0xFF

class Tapis:public QObject
{
    Q_OBJECT
public:
    Tapis(quint16 _adresseTapis, ModBusTCP &_modBusTCP, QObject *parent=0);
    void commander(bool _marche);

private:
    quint16 adresseTapis;
    ModBusTCP &modBusTCP;
};

#endif // TAPIS_H
