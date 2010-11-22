#include <QtGui>
#include <QtSql>
#include "MainWindow.h"

#include "connection.h"

void initializeModel(QSqlTableModel *model)
{
    model->setTable("books");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ISBN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Title"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Author"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Publisher"));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!createConnection())
        return 1;

    QSqlTableModel model;
    initializeModel(&model);

    MainWindow win(&model);
    win.show();

    return app.exec();
}
