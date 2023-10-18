#include "mainwindow.h"
#include "bookscontroller.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add additional feature so that
    // we can manually modify the data in ListView
    // It may be triggered by hitting any key or double-click etc.
    //ui->listView->
    //    setEditTriggers(QAbstractItemView::AnyKeyPressed |
    //                    QAbstractItemView::DoubleClicked);

    mBookController = new BooksController();
    mBookController->fetchBooks();

    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setIconSize(QSize(150,200));
    ui->listWidget->setResizeMode(QListWidget::Adjust);

    ui->pushButton_4->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleFilterButton);
    connect(ui->pushButton_3, &QPushButton::released, this, &MainWindow::handleFavoriteButton);
    connect(ui->pushButton_4, &QPushButton::released, this, &MainWindow::handleShowAllButton);
    connect(ui->pushButton_5, &QPushButton::released, this, &MainWindow::handleUnFavoriteButton);
    connect(mBookController, &BooksController::thumbnailUpdated, this, &MainWindow::updateThumbnails);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updateDetails);
    connect(ui->listWidget->verticalScrollBar(), &QAbstractSlider::valueChanged, this, &MainWindow::loadMoreBooks);
}

void MainWindow::handleButton(){
    mBookController->fetchBooks();
}

void MainWindow::handleFilterButton(){
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(true);
    ui->listWidget->clear();
    for (const QString& id : mBookController->favorites)
    {
        updateThumbnails(id);
    }
}

void MainWindow::handleFavoriteButton(){
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_5->setEnabled(true);
    const QString& s = ui->listWidget->currentItem()->text();
    mBookController->setFavorite(s);
    updateDetails();
}

void MainWindow::handleUnFavoriteButton(){
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    const QString& s = ui->listWidget->currentItem()->text();
    mBookController->setUnFavorite(s);
    updateDetails();
}

void MainWindow::handleShowAllButton(){
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->listWidget->clear();
    for (const QString& id : mBookController->books){
        updateThumbnails(id);
    }
}

void MainWindow::updateThumbnails(QString id){
    QPixmap* image = mBookController->getThumbnail(id);
    if(image != nullptr)
    {
        ui->listWidget->addItem(new QListWidgetItem(QIcon(*image), mBookController->getBook(id)->id));
    }
}

void MainWindow::updateDetails(){
    const QString& s = ui->listWidget->currentItem()->text();
    Book* selectedBook = mBookController->getBook(s);
    if (selectedBook)
    {
        QString details = "<h5>Title: </h5><p>" + selectedBook->title + "</p>";
        if (selectedBook->authors.size() == 1)
            details += "<h5>Author: </h5><p>" + selectedBook->authors[0] + "</p>";
        else
        {
            details += "<h5>Authors: </h5><p>";
            for (QString& author : selectedBook->authors){
                details += author + "<br> ";
            }
            details += "</p>";
        }
        details += "<h5>Description: </h5><p>" + selectedBook->description + "</p>";
        if (selectedBook->buyLink != "")
            details += "<h5>Buy book: </h5><a href=\"" + selectedBook->buyLink + "\">link</a>";
        if (selectedBook->favorite)
        {
            details += "<h5>Favorite</h5>";
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_5->setEnabled(true);
        }else{
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_5->setEnabled(false);
        }
        ui->textBrowser->setHtml(details);
    }
}

void MainWindow::loadMoreBooks(int value){
    if (value >= ui->listWidget->verticalScrollBar()->maximum()){
        mBookController->fetchBooks();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

