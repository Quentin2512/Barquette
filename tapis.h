#ifndef TAPIS_H
#define TAPIS_H

#include <QObject>

class Tapis:public QObject
{
    Q_OBJECT
public:
    Tapis(quint16 _adresseTapis, QObject *parent=0);
    void commander(bool _marche);

private:
    quint16 adresseTapis;
};

#endif // TAPIS_H
