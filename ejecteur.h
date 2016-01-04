#ifndef EJECTEUR_H
#define EJECTEUR_H

#include <QObject>
#include "partieoperative.h"

class Ejecteur : public QObject
{
    Q_OBJECT
public:
    Ejecteur(int _numEjecteur, ModBusTCP &_modBusTCP, QObject *parent = 0);
    ~Ejecteur();
    void piloter();

public slots:
    void on_finTimerEjecteur();

private:
    int numEjecteur;
    ModBusTCP &modBusTCP;
    QTimer *timerEjecteur;
};

#endif // EJECTEUR_H
