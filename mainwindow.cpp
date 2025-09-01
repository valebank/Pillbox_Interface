#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "agregar.h"
#include "borrar.h"
#include "vinculacion.h"
#include <QTcpSocket>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    msocket=new QTcpSocket(this);
    QString nombre;
    nombre = "DatosPastillas.sqlite";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(nombre);

   if(! db.open() )
   {
      QMessageBox::critical(this, "Base de Datos", "Error en la apertura de la base!");

   }

   crearTablaPastillas();
   cantPastillas = mostrarDatos();

   QString response = "";
   connect(msocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}



void MainWindow::readyRead()
{
    response += msocket->read(msocket->bytesAvailable());
    if (response.contains('\n')){
        QStringList respList = response.split('\n');
        for(int i = 0; i < respList.length(); i++){
            QString trama = respList[i];
            if (not trama.isEmpty())
            {
                actualizarCantidad(trama);
            }
        }
        response.clear();
    }

}

void MainWindow::crearTablaPastillas()
{
    QString consulta;
    consulta.append("CREATE TABLE IF NOT EXISTS pastillas("
                    "nombre VARCHAR(100) PRIMARY KEY,"
                    "cantidad VARCHAR(100),"
                    "horario VARCHAR(100)"
                    ");");

    QSqlQuery tabla(db);

    tabla.prepare(consulta);
    if (!tabla.exec())
        QMessageBox::critical(this, "Base de Datos", "No se pudo crear la tabla");

}
MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::mostrarDatos()
{
    int i = 0;
    QString consulta;
    consulta.append("SELECT * FROM pastillas");

    QSqlQuery tabla(db);

    tabla.prepare(consulta);

    if ( !tabla.exec())
    {
      qDebug()<<"No se pudo consultar";
    }

    ui->tableWidgetDatos->setRowCount(0);

    while(tabla.next())
    {
       ui->tableWidgetDatos->insertRow(i);
       ui->tableWidgetDatos->setItem(i,0,new QTableWidgetItem (tabla.value(0).toByteArray().constData()) );
       ui->tableWidgetDatos->setItem(i,1,new QTableWidgetItem (tabla.value(1).toByteArray().constData()) );
       ui->tableWidgetDatos->setItem(i,2,new QTableWidgetItem (tabla.value(2).toByteArray().constData()) );
       i++;
    }

    return i;
}
void MainWindow::on_pushButtonCerrar_clicked()
{
    this->close();
}
void MainWindow::on_pushButtonAgregar_clicked()
{
    Agregar * formAgregar = new Agregar;
    formAgregar->setDB(db);//con esto obtengo la base de datos usar para obtener nombre y horario..etc
    formAgregar->exec();
    mostrarDatos();
}
void MainWindow::on_pushButtonEliminar_clicked()
{
    Borrar * formBorrar = new Borrar;
    formBorrar->setDB(db);
    formBorrar->exec();
    mostrarDatos();
}

void MainWindow::on_pushButton_connect_clicked()
{
    msocket->connectToHost(ui->lineEdit->text(),8888);
    procesarDatos();
    ui->pushButton_connect->setText("Conectado");
    ui->pushButton_connect->setStyleSheet("background-color: rgb(0,255,0);");
}

void MainWindow::procesarDatos()
{
    int servo = 1;
    QString consulta;
    consulta.append("SELECT nombre FROM pastillas;");
    QSqlQuery tabla(db);
    tabla.prepare(consulta);
    if ( !tabla.exec())
        qDebug()<<"No se pudo consultar";
    while(tabla.next())
    {
        QString nombre = tabla.value(0).toByteArray().constData(); // ibu cafia qura
        generarTimer(nombre, servo);
        servo++;
    }
}

void MainWindow::generarTimer(QString pastilla, int servo)
{
    Pastilla p = obtenerDatosPastilla(pastilla);
    if (p.cantidad > 0){
        p.servo = servo;
        tiempo t;
        t.nombrePastilla = pastilla;
        t.timer1 = new QTimer(this);
        connect(t.timer1,
                &QTimer::timeout,
                this,
                [=](){ procesarPastilla(p); } //[=](){enviarDatos(trama);}almacena la trama que estaba
        );
        timers<<t;
        t.timer1->start(p.horario * SEG);
    } else
        qDebug()<<"NO TENES MAS CANTIDAD";
}

void MainWindow::procesarPastilla(const Pastilla p)//viene aca cuando se vence el timer
{
    Pastilla pT = obtenerDatosPastilla(p.nombre);//obtiene los datos de la pastilla que quedo guardada cuando se genero el timer
    pT.servo = p.servo;

    if (pT.cantidad == 0)
    {
        finalizarTimer(pT);
 /*
        QString consulta;
        consulta.append("DELETE FROM pastillas WHERE nombre='"
                        + pT.nombre +
                        "' ");
        QSqlQuery tabla(db);
        tabla.prepare(consulta);

        if (tabla.exec() && tabla.numRowsAffected())
        {
            mostrarDatos();
        }
*/
    }
    else
        enviarDatos(pT);
}
void MainWindow::finalizarTimer(Pastilla p)//para el timer cuando timer es cero
{
    tiempo t = buscarTimerDe(p);
    t.timer1->stop();
}

tiempo MainWindow::buscarTimerDe(Pastilla p)//compara con la estrctura cual fue el timer que tiene cantidad=0
{
    for (int i = 0; i < timers.length(); i++)
        if (timers[i].nombrePastilla == p.nombre)
            return timers[i];
}

QString MainWindow::generarTrama(Pastilla p)
{
    return "#" + QLocale().toString(p.servo) + QLocale().toString(p.cantidad) + p.nombre + "$";
}

Pastilla MainWindow::obtenerDatosPastilla(QString nombreP)//saco de la tabala los datos de las pastillas
{
    QString consulta;
    consulta.append("SELECT nombre, cantidad, horario  FROM pastillas WHERE nombre = \"" + nombreP + "\";");
    QSqlQuery tabla(db);
    tabla.prepare(consulta);
    Pastilla p;
    if (tabla.exec())
    {
        while(tabla.next())
        {
           QString nombre = tabla.value(0).toByteArray().constData(); // ibu cafia qura
           QString cantidad = tabla.value(1).toByteArray().constData(); // 1 pastilla
           QString horario = tabla.value(2).toByteArray().constData(); // 1 hora 2horas
           p.nombre = nombre;
           p.cantidad = cantidad.toInt();
           p.horario = horario.toInt();
        }
    } else {
        qDebug()<<"No se pudo consultar la pastilla";
    }

    return p;
}

void MainWindow::enviarDatos(Pastilla p)
{
    QTextStream T(msocket);
    QString trama = generarTrama(p);
    T<<trama;
    T<<'\0';
    msocket->flush();
    qDebug()<<"Pastilla: "<<p.nombre<<" - Trama: "<<trama;
}

void MainWindow::actualizarCantidad(QString nombreP)
{
    QString consulta;
    consulta.append("UPDATE pastillas SET cantidad = cantidad - 1 WHERE nombre = \"" + nombreP + "\" AND cantidad <> 0");//actualizamos la tabla
    consulta.append("");
    QSqlQuery tabla(db);
    tabla.prepare(consulta);

    if (tabla.exec())
    {
      //QMessageBox::information(this, "Base de Datos", "La pastilla se actualizo satisfactoriamente!");
        qDebug()<<"La pastilla "<<nombreP<<" se actualizo satisfactoriamente!";
    } else {
        qDebug()<<"La pastilla "<<nombreP<<" no se ha podido actualizar.";
    }
    mostrarDatos();
}

void MainWindow::on_pushButtonDesconetar_clicked()
{

    QString consulta;
    consulta.append("SELECT nombre FROM pastillas;");
    QSqlQuery tabla(db);
    tabla.prepare(consulta);
    if ( !tabla.exec())
        qDebug()<<"No se pudo consultar";
    while(tabla.next())
    {
        QString nombre = tabla.value(0).toByteArray().constData();

        Pastilla p = obtenerDatosPastilla(nombre);
        finalizarTimer(p);
    }

     msocket->close();
     ui->pushButton_connect->setStyleSheet("background-color: rgb(255,0,0);");
}

