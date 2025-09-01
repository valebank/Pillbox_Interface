#include "borrar.h"
#include "ui_borrar.h"

Borrar::Borrar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Borrar)
{
    ui->setupUi(this);
}
Borrar::~Borrar()
{
    delete ui;
}
void Borrar::setDB(QSqlDatabase & db)
{
    dbRef = db;
}

void Borrar::on_pushButtonCancelar_clicked()
{
    this->close();
}
void Borrar::on_pushButtonEliminar_clicked()
{
    QString consulta;
    consulta.append("DELETE FROM pastillas WHERE nombre='"
                    +ui->lineEditNombre->text()+
                    "' ");

    QSqlQuery tabla(dbRef);

    tabla.prepare(consulta);

    if (tabla.exec() && tabla.numRowsAffected())
    {
        QMessageBox::information(this, "Prueba", "Pastilla borrada satisfactoriamente! :)");

        this->close();
    }
}

