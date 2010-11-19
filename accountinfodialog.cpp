#include "accountinfodialog.h"
#include "ui_accountinfodialog.h"

AccountInfoDialog::AccountInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountInfoDialog)
{
    ui->setupUi(this);
}

AccountInfoDialog::~AccountInfoDialog()
{
    delete ui;
}
