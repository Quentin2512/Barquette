#include "ejecteur.h"

Ejecteur::Ejecteur(int _numEjecteur, ModBusTCP &_modBusTCP, QObject *parent) :QObject(parent),numEjecteur(_numEjecteur),modBusTCP(_modBusTCP)
{
    timerEjecteur=new QTimer();
    connect(timerEjecteur,SIGNAL(timeout()),this,SLOT(on_finTimerEjecteur()));
}

Ejecteur::~Ejecteur()
{
    delete timerEjecteur;
}

void Ejecteur::piloter()
{
    modBusTCP.WriteSingleCoils(numEjecteur, ON);
    timerEjecteur->start(500);
}

void Ejecteur::on_finTimerEjecteur()
{
    timerEjecteur->stop();
    modBusTCP.WriteSingleCoils(numEjecteur, OFF);
}

