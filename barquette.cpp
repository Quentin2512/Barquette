#include "barquette.h"

Barquette::Barquette(const quint8 _destination, const QString _code, QObject *parent) :
    QThread(parent),code(_code),destination(_destination)
{
    synchro.release();
}

Barquette::~Barquette()
{

}

void Barquette::run()
{
    bool verif=false;
    do{
        synchro.acquire();
        if((capteurs&destination)==destination){
            verif=true;
        }
    }while(!verif);
    emit signalEjecteurTrouve(destination);
    emit signalBarquetteEjectee(destination,code);
}

void Barquette::on_changementEtatCapteurs(const quint8 capteur)
{
    this->capteurs=capteur;
    synchro.release();
}
