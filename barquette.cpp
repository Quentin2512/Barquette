#include "barquette.h"

Barquette::Barquette(const qint8 _destination, const QString _code, QObject *parent) :
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
    qint8 masque;
    do{
        synchro.acquire();
        for(int x=0;x<4;x++){
            masque=pow(2,x-1);
            if((capteurs&masque)==masque){
                if(masque==destination)
                    verif=true;
                masque<<=1;
            }
        }
    }while(!verif);
    emit signalBarquetteEjectee();
}

void Barquette::on_changementEtatCapteurs(const qint8 capteur)
{
    this->capteurs=capteur;
    synchro.release();
}
