#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
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

private slots:
    void connecterPO();

    void lancerProduction();

    void on_socketChanged(QAbstractSocket::SocketState socketEtat);

private:
    Ui::MainWindow *ui;
    PartieOperative *laPO;
    QList<Barquette> listeBarquettes;
};

#endif // MAINWINDOW_H
