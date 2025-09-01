#ifndef AGREGAR_H
#define AGREGAR_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>

#include <QString>
#include <QDebug>
#include <QMessageBox>

#include <QDialog>

namespace Ui {
class Agregar;
}

class Agregar : public QDialog
{
    Q_OBJECT

public:
    explicit Agregar(QWidget *parent = nullptr);
    ~Agregar();

    void setDB(QSqlDatabase & db);
    QString getNom(void);
    QString getCant(void);
    QString getHor(void);

private slots:
    void on_pushButtonVolver_clicked();
    void on_pushButtonAgregar_clicked();

private:

    Ui::Agregar *ui;
    QSqlDatabase dbRef;

};

#endif // AGREGAR_H
