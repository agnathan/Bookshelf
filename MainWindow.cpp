#include "MainWindow.h"
#include "accountinfodialog.h"
#include "bookdetailsview.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTableView>
#include <QString>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHeaderView>
#include <QSqlField>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QModelIndexList>
#include <QMainWindow>
#include <QMenuBar>


MainWindow::MainWindow(QSqlTableModel* model, QWidget *parent) :
        QMainWindow(parent),
        m_model(model)
{
    QWidget* w = new QWidget();
    setCentralWidget(w);

    createMenus();
    createActions();

    resize(650,300);
    QVBoxLayout* vLayout = new QVBoxLayout();
    w->setLayout(vLayout);

    m_bookDetailsView = new BookDetailsView();
    vLayout->addWidget(m_bookDetailsView);

    m_tableView = createView(model);
    vLayout->addWidget(m_tableView);

    QLabel* label = new QLabel();
    label->setText("Enter New ISBN:");

    m_lineEdit = new QLineEdit();
    vLayout->addWidget(m_lineEdit);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(label);
    hLayout->addWidget(m_lineEdit);

    vLayout->addLayout(hLayout);

    bool ok = connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(addNewRow()));
    Q_ASSERT(ok);
}

QTableView* MainWindow::createView(QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);

    view->horizontalHeader()->setStretchLastSection(true);
    view->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    // Setup the connections
    connect(view, SIGNAL(activated(QModelIndex)), this, SLOT(bookRowActivated(QModelIndex)));
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(bookRowClicked(QModelIndex)));

    connect(view->selectionModel(), SIGNAL(currentChanged(const QModelIndex& , const QModelIndex&)),
            this, SLOT(bookSelectionChanged(const QModelIndex& , const QModelIndex&)));

    return view;
}

void MainWindow::addNewRow()
{
    QString value = m_lineEdit->text();
    getIsbnInfo(value);
}

void MainWindow::removeRow()
{

}

void MainWindow::getIsbnInfo(const QString& isbn)
{
    // Example:   http://isbndb.com/api/books.xml?access_key=XXGAQFLG&index1=isbn&value1=0812550757

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    bool ok = connect(manager, SIGNAL(finished(QNetworkReply*)),
                      this, SLOT(replyFinished(QNetworkReply*)));
    Q_ASSERT(ok);

    QString isbnUrl = "http://isbndb.com/api/books.xml?access_key=";
    isbnUrl += "XXGAQFLG";  // Gary's key
    isbnUrl += "&index1=isbn&value1=";
    isbnUrl += isbn;
    manager->get(QNetworkRequest(QUrl(isbnUrl)));
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
        return;

    QByteArray data = reply->readAll();

    /* Example data returned

<ISBNdb server_time="2010-10-30T18:16:28Z">
<BookList total_results="1" page_size="10" page_number="1" shown_results="1">
    <BookData book_id="speaker_for_the_dead_a01" isbn="0812550757" isbn13="9780812550757">
        <Title>Speaker for the dead</Title>
        <TitleLong/>
        <AuthorsText>Orson Scott Card</AuthorsText>
        <PublisherText publisher_id="tor">New York, N.Y. : TOR, c1986.</PublisherText>
    </BookData>
</BookList>
</ISBNdb>

 */

    QDomDocument doc;
    if ( ! doc.setContent(data))
    {
        //error
        return;
    }
    QDomElement docElem = doc.documentElement();
    QDomElement bookListElem = docElem.firstChildElement("BookList");
    if (bookListElem.isNull())
        return;
    QString count = bookListElem.attribute("total_results");
    if (count.isEmpty() || count == "0")
    {
        QMessageBox::information(this, "Not Found", "The book was not found.");
        return;
    }

    QDomElement bookDataElem = bookListElem.firstChildElement("BookData");
    if (bookDataElem.isNull())
        return;
    QString isbn = bookDataElem.attribute("isbn");

    QDomElement titleElem = bookDataElem.firstChildElement("Title");
    if (titleElem.isNull())
        return;
    QString title = titleElem.text();

    QDomElement authorElem = bookDataElem.firstChildElement("AuthorsText");
    if (authorElem.isNull())
        return;
    QString author = authorElem.text();

    QDomElement publisherElem = bookDataElem.firstChildElement("PublisherText");
    if (publisherElem.isNull())
        return;
    QString publisher = publisherElem.text();

    QSqlRecord record = m_model->record();
    record.setValue(0,isbn);
    record.setValue(1,title);
    record.setValue(2,author);
    record.setValue(3,publisher);
    int row = m_model->rowCount();
    m_model->insertRecord(row,record);
    m_tableView->scrollToBottom();

    m_lineEdit->clear();
    m_lineEdit->setFocus();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QItemSelectionModel *selmodel = m_tableView->selectionModel();
    QModelIndex current = selmodel->currentIndex(); // the "current" item
    QModelIndexList selected = selmodel->selectedIndexes(); // list of "selected" items

    switch (event->key())
    {
    case Qt::Key_Delete:
        for (int i = 0; i < selected.size(); ++i) {
            qDebug() << selected.at(i).row();
            qDebug() << m_model->removeRows( selected.at(i).row(), 1);
        };
        break;
    }

}

void MainWindow::createActions() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(apiKeyDialogAct);
}

void MainWindow::createMenus() {
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    apiKeyDialogAct = new QAction(tr("API &Keys"), this);
    apiKeyDialogAct->setStatusTip(tr("Enter you book developer API keys"));

    // Setup the connections
    connect(apiKeyDialogAct, SIGNAL(triggered()), this, SLOT(apiKeyDialogShow()));
}

void MainWindow::apiKeyDialogShow()
{
    AccountInfoDialog* d = new AccountInfoDialog();
    d->exec();
}

void MainWindow::bookSelectionChanged(const QModelIndex& m, const QModelIndex& n)
{
    qDebug() << "Called bookSelectionChanged() --> (" << m.row() << "," << m.column() << ") : " << m_model->data(m);
}

