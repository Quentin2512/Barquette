#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QQueue>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "barquette.h"
#include "partieoperative.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void signalChangementEtatCapteurs(quint8);
    void stateChanged(QAbstractSocket::SocketState);
    void etatCapteurChanged(qint8);

private slots:
    void connecterPO();

    void lancerProduction();

    void on_socketChanged(QAbstractSocket::SocketState state);

    void on_etatCapteurChanged(quint8 trame);

    void on_lineEdit_codeProduit_textChanged(const QString &arg1);

    void on_barquetteEjectee(quint8 ejecteur, QString nom);

private:
    Ui::MainWindow *ui;
    PartieOperative *laPO;
    QList<Barquette*> listeBarquettes;
    QQueue<Barquette*> fileBarquettes;
    QSqlDatabase db;
};

#endif // MAINWINDOW_H
