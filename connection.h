#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.sqllite");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "This example needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QStringList tables = db.tables();
    if ( ! tables.contains("books"))
    {
        QSqlQuery query;
        bool ok = query.exec("create table books (isbn varchar(13) primary key, "
                             "title varchar(100), author varchar(100), publisher varchar(100))");
        if ( ! ok)
            qDebug() << "Failed to create table:" << query.lastError();
    }
    return true;
}
//! [0]

#endif
