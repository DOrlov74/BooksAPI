#ifndef BOOKSCONTROLLER_H
#define BOOKSCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "book.h"
#include "qsettings.h"
#include "thumbnailcontroller.h"

class BooksController : public QObject
{
    Q_OBJECT
public:
    explicit BooksController(QObject *parent = nullptr);
    virtual ~BooksController();

    Q_INVOKABLE void fetchBooks();

    QStringList books;
    QStringList favorites;

    Book* getBook(QString id);
    Book* getBookByTitle(QString title);
    QPixmap* getThumbnail(QString id);
    bool inFavorites(QString id);
    void setFavorite(QString id);
    void setUnFavorite(QString id);

signals:
    void thumbnailUpdated(QString id);
private slots:
    void thumbnailUpdate(QString id);
    void dataReadyRead();
    void setBooks();
    void writeFavorite(QString id);
    void writeAllFavorites();
    void readFavorites();

private:
    const QString BASEAPI = "https://www.googleapis.com/books/v1";
    const QString BOOKS = "/volumes";
    const quint16 MAXRESULTS = 20;
    quint16 mLastResults=0;
    QNetworkAccessManager* mNetManager;
    QNetworkReply* mNetReply;
    QByteArray* mDataBuffer;
    QList<Book*> mBooks;
    QList<ThumbnailController*> mThumbnailControllers;
    quint16 mCurrentPage;
    QSettings* mSettings = new QSettings("BooksAPI", "BooksAPI");
};

#endif // BOOKSCONTROLLER_H
