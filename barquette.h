#ifndef BARQUETTE_H
#define BARQUETTE_H

#include <QThread>
#include <QSemaphore>
#include <math.h>

class Barquette : public QThread
{
    Q_OBJECT
public:
    explicit Barquette(const qint8 _destination, const QString _code, QObject *parent = 0);
    ~Barquette();
    void run();
    QString code;

public slots:
    void on_changementEtatCapteurs(const qint8 capteur);

signals:
    void signalBarquetteEjectee();

private:
    qint8 capteurs;
    int destination;
    QSemaphore synchro;
};

#endif // BARQUETTE_H
