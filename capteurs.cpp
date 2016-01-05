#include "capteurs.h"

Capteurs::Capteurs(quint16 _adresseCapteurs, ModBusTCP &_modBusTCP, QObject *parent) : QObject(parent),adresseCapteurs(_adresseCapteurs),modBusTCP(_modBusTCP)
{

}

void Capteurs::LireEtatCapteurs()
{
    modBusTCP.ReadInputRegister(0);
}

