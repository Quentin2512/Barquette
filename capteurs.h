#ifndef CAPTEURS_H
#define CAPTEURS_H

#include <QObject>

class Capteurs : public QObject
{
    Q_OBJECT
public:
    Capteurs(quint16 _adresseCapteurs, QObject *parent = 0);
    void LireEtatCapteurs();

signals:
    void signalEtatCapteurs();

private:
    quint16 adresseCapteurs;
};

#endif // CAPTEURS_H
