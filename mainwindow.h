#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bookscontroller.h"
#include "qpushbutton.h"
#include <QMainWindow>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButton();
    void handleFilterButton();
    void handleFavoriteButton();
    void handleUnFavoriteButton();
    void handleShowAllButton();
    void updateThumbnails(QString id);
    void updateDetails();
    void loadMoreBooks(int value);
private:
    Ui::MainWindow *ui;
    BooksController* mBookController;
};
#endif // MAINWINDOW_H
