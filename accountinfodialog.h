#ifndef ACCOUNTINFODIALOG_H
#define ACCOUNTINFODIALOG_H

#include <QDialog>

namespace Ui {
    class AccountInfoDialog;
}

class AccountInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountInfoDialog(QWidget *parent = 0);
    ~AccountInfoDialog();

private:
    Ui::AccountInfoDialog *ui;
};

#endif // ACCOUNTINFODIALOG_H
