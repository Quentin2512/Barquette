#include "tapis.h"

Tapis::Tapis(quint16 _adresseTapis, ModBusTCP *_modBusTCP, QObject *parent):QObject(parent),adresseTapis(_adresseTapis), modBusTCP(_modBusTCP)
{

}

void Tapis::commander(bool _marche)
{
    if (_marche)
        modBusTCP->WriteSingleCoils( adresseTapis, 0xFF );
    else
        modBusTCP->WriteSingleCoils( adresseTapis, 0x00 );
}
