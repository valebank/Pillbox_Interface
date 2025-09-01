#ifndef BORRAR_H
#define BORRAR_H


#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>

#include <QString>
#include <QDebug>
#include <QMessageBox>

#include <QDialog>


namespace Ui {
class Borrar;
}

class Borrar : public QDialog
{
    Q_OBJECT

public:
    explicit Borrar(QWidget *parent = nullptr);
    ~Borrar();

    void setDB(QSqlDatabase & db);

private slots:
    void on_pushButtonCancelar_clicked();

    void on_pushButtonEliminar_clicked();

private:
    Ui::Borrar *ui;
    QSqlDatabase dbRef;

};

#endif // BORRAR_H
