#include "bookdetailsview.h"
#include <QVBoxLayout>
#include <QLabel>

BookDetailsView::BookDetailsView(QWidget *parent) :
    QWidget(parent)
{
    QLabel* l = new QLabel("Book Details View");
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(l);
    setLayout(vLayout);
}
