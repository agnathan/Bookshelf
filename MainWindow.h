#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QString;
class QSqlTableModel;
class QTableView;
class QLineEdit;
class QNetworkReply;
class QKeyEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QSqlTableModel* model, QWidget* parent = 0);

    void keyPressEvent(QKeyEvent *event);
private:
    QTableView* createView(QSqlTableModel* model);
    void getIsbnInfo(const QString& isbn);

    QSqlTableModel* m_model;
    QLineEdit* m_lineEdit;
    QTableView* m_tableView;

private slots:
    void addNewRow();
    void removeRow();
    void replyFinished(QNetworkReply* reply);

signals:

public slots:

};

#endif // MAINWINDOW_H
