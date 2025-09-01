#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QString>
#include "agregar.h"
#include <QTcpSocket>
#include <QTimer>
#include <QList>
#define SEG 1000
#define MIN 60000
#define HORA 3600000

typedef struct{
    QTimer *timer1;
    QString nombrePastilla;
}tiempo;

typedef struct{
    QString nombre;
    int cantidad;
    int servo;
    int horario;
} Pastilla;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonAgregar_clicked();

    void on_pushButtonCerrar_clicked();

    void crearTablaPastillas();

    int mostrarDatos();


    void on_pushButtonEliminar_clicked();

    void on_pushButton_connect_clicked();
    void procesarDatos();
    void actualizarCantidad(QString nombreP);
    void readyRead(void);

    void debugRapidoPastillas();

    QString generarTrama(Pastilla p);
    Pastilla obtenerDatosPastilla(QString nombreP);
    void enviarDatos(Pastilla p);
    void generarTimer(QString pastilla, int servo);

    void procesarPastilla(const Pastilla p);
    void finalizarTimer(Pastilla p);
    tiempo buscarTimerDe(Pastilla p);

    void on_pushButtonDesconetar_clicked();

private:

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QList<tiempo> timers;
    QTcpSocket *msocket;

    QString response;

    int cantPastillas;




};
#endif // MAINWINDOW_H
