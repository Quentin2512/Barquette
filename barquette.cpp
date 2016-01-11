#include "barquette.h"

Barquette::Barquette(const quint8 _destination, const QString _code, QObject *parent) :
    QThread(parent),code(_code),destination(_destination)
{
    synchro.release();
    verif=false;
}

Barquette::~Barquette()
{

}

void Barquette::run()
{
    bool verifDescendentDeOuf=false;
    do{
        synchro.acquire();
        if((capteurs&destination)!=destination && verif){
            verifDescendentDeOuf=true;
        }
    }while(!verifDescendentDeOuf);
    emit signalEjecteurTrouve(destination);
    emit signalBarquetteEjectee(destination,code);
}

void Barquette::on_changementEtatCapteurs(const quint8 capteur)
{
    if((capteurs&destination)==destination)
        verif=true;
    this->capteurs=capteur;
    synchro.release();
}
