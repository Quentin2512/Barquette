#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
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
    void lancerProduction();
    void connecterPO();

private:
    Ui::MainWindow *ui;
    PartieOperative *laPO;
    QList<Barquette> listeBarquettes;
};

#endif // MAINWINDOW_H
