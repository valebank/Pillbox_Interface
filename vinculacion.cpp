#include "vinculacion.h"
#include "ui_vinculacion.h"
#include <QMessageBox>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

vinculacion::vinculacion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vinculacion)
{
    ui->setupUi(this);
     mManagger = new QNetworkAccessManager(this);
   QObject::connect(mManagger, &QNetworkAccessManager::finished,this, [&](QNetworkReply *reply)
   {
       QByteArray data = reply->readAll();
       QString str = QString::fromLatin1(data);

   });
}
vinculacion::~vinculacion()
{
    delete ui;
}
void vinculacion::on_pushButtonConectar_clicked()
{
    // Lee la IP dada
    // La ip la almacena de forma global como atributo

    // Se prueba si existe o se puede conectar

    // mainWindows.limpiarTimers()

    // procesarDatos

    mManagger->get(QNetworkRequest(QUrl(ui->lineEdit->text())));
}

void vinculacion::setDB(QSqlDatabase & db)
{
    dbRef = db;
}

void vinculacion::procesarDatos(){
    int i = 1;
    QString consulta;
    consulta.append("SELECT nombre, horario FROM pastillas;");

    QSqlQuery tabla(dbRef);

    tabla.prepare(consulta);

    if ( !tabla.exec())
    {
      qDebug()<<"No se pudo consultar";
    }

    while(tabla.next() and i < 4)
    {
       QString nombre = tabla.value(0).toByteArray().constData(); // ibu cafia qura
       QString horario = tabla.value(1).toByteArray().constData(); // 1 hora 2horas

       int servo = i; // 1 2 3 4 5

       QString trama = "#" + (QString)servo + nombre + horario + "$";

       // mainMenu.generarTimer(trama);
       // Crear timer 3
       // Timer ejecuta enviarDatos
       // enviarDatos(dato);
       i++;
    }
}

