#ifndef VINCULACION_H
#define VINCULACION_H

#include <QDialog>
#include<QTimer>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

namespace Ui {
class vinculacion;
}
class QNetworkAccessManager;

class vinculacion : public QDialog
{
    Q_OBJECT

public:
    explicit vinculacion(QWidget *parent = nullptr);
    ~vinculacion();
    void setDB(QSqlDatabase & db);


private slots:

    void procesarDatos();
    void on_pushButtonConectar_clicked();
    void enviarDatos(QString datoAEnviar);

private:
    Ui::vinculacion *ui;
    QNetworkAccessManager * mManagger;
    QSqlDatabase dbRef;
};

#endif // VINCULACION_H
