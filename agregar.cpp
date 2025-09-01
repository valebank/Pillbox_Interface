#include "agregar.h"
#include "ui_Agregar.h"

Agregar::Agregar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Agregar)
{
    ui->setupUi(this);
}

Agregar::~Agregar()
{
    delete ui;
}
void Agregar::on_pushButtonVolver_clicked()
{
    this->close();
}
void Agregar::on_pushButtonAgregar_clicked()
{
    QString consulta;
     consulta.append("INSERT INTO pastillas("
                     "nombre,"
                     "cantidad,"
                     "horario) "
                     "VALUES("
                     "'"+getNom()+"',"
                     "'"+getCant()+"',"
                     "'"+getHor()+ "'"
                     ");");

     QSqlQuery tabla(dbRef);
     tabla.prepare(consulta);

     if (tabla.exec())
     {
       QMessageBox::information(this, "Base de Datos", "La pastilla se agrego satisfactoriamente!");
     } else {
         QMessageBox::warning(this, "Base de Datos", "La pastilla ya existe en la base de datos");
     }
}

void Agregar::setDB(QSqlDatabase & db)
{
    dbRef = db;
}
QString Agregar::getNom(void)
{
    return(ui->lineEditNombre->text());
}
QString Agregar::getCant(void)
{
    return(ui->lineEditCantidad->text());
}
QString Agregar::getHor(void)
{
    return(ui->lineEditHorario->text());
}
