#ifndef BARQUETTE_H
#define BARQUETTE_H

#include <QThread>
#include <QSemaphore>
#include <math.h>

class Barquette : public QThread
{
    Q_OBJECT
public:
    explicit Barquette(const quint8 _destination, const QString _code, QObject *parent = 0);
    ~Barquette();
    void run();
    QString code;

public slots:
    void on_changementEtatCapteurs(const quint8 capteur);

signals:
    void signalBarquetteEjectee(quint8,QString);
    void signalEjecteurTrouve(quint8);

private:
    quint8 capteurs;
    quint8 destination;
    QSemaphore synchro;
    bool verif;
};

#endif // BARQUETTE_H
