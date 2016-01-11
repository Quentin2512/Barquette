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
    bool verifDescendent=false;
    do{
        synchro.acquire();
        if((capteurs&destination)!=destination && verif){
            verifDescendent=true;
        }
    }while(!verifDescendent);
    emit signalEjecteurTrouve(destination);
    emit signalBarquetteEjectee(destination,code);
}

void Barquette::on_changementEtatCapteurs(const quint8 capteur)
{
    this->capteurs=capteur;
    if((capteurs&destination)==destination)
        verif=true;
    synchro.release();
}
