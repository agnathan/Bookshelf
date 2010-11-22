#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookdetailsview.h"

class QString;
class QSqlTableModel;
class QTableView;
class QLineEdit;
class QNetworkReply;
class QKeyEvent;
class QMenu;
class QAction;
class QModelIndex;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QSqlTableModel* model, QWidget* parent = 0);

    void keyPressEvent(QKeyEvent *event);
private:
    QTableView* createView(QSqlTableModel* model);
    void getIsbnInfo(const QString& isbn);

    BookDetailsView* m_bookDetailsView;
    QSqlTableModel* m_model;
    QLineEdit* m_lineEdit;
    QTableView* m_tableView;

    QMenu* fileMenu;
    QMenu* optionsMenu;

    QAction* exitAct;
    QAction* apiKeyDialogAct;

    QLabel* infoLabel;

private:
    void createActions();
    void createMenus();

private slots:
    void addNewRow();
    void removeRow();
    void replyFinished(QNetworkReply* reply);

    void apiKeyDialogShow();

signals:

public slots:
    void bookSelectionChanged(const QModelIndex& m, const QModelIndex& n);
};

#endif // MAINWINDOW_H
