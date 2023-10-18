#include "bookscontroller.h"
#include "thumbnailcontroller.h"
#include "volume.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

BooksController::BooksController(QObject *parent)
    : QObject{parent},
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(nullptr),
    mDataBuffer(nullptr),
    mCurrentPage(0)
{
    readFavorites();
}

BooksController::~BooksController()
{
    delete mDataBuffer;
    delete mNetReply;
    delete mNetManager;
    for (QList<Book*>::const_iterator it = mBooks.cbegin(); it != mBooks.cend(); ++it)
    {
        delete *it;
    }
    mBooks.clear();
    for (QList<ThumbnailController*>::const_iterator it = mThumbnailControllers.cbegin(); it != mThumbnailControllers.cend(); ++it)
    {
        delete *it;
    }
    mThumbnailControllers.clear();
}

void BooksController::fetchBooks()
{
    mDataBuffer = new QByteArray;
    QList<QPair<QString, QString>> queryList;
    const QPair<QString, QString> qPair("q","mobile+development");
    queryList.append(qPair);
    const QPair<QString, QString> itemsPair("maxResults",QVariant(MAXRESULTS).toString());
    queryList.append(itemsPair);
    const QPair<QString, QString> pagePair("startIndex", QVariant(mCurrentPage).toString());
    queryList.append(pagePair);
    QUrlQuery query;
    query.setQueryItems(queryList);
    QUrl url(BASEAPI + BOOKS);
    url.setQuery(query);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    mNetReply = mNetManager->get(request);
    connect(mNetReply, &QIODevice::readyRead, this, &BooksController::dataReadyRead);
    connect(mNetReply, &QNetworkReply::finished, this, &BooksController::setBooks);
}

void BooksController::dataReadyRead()
{
    mDataBuffer->append(mNetReply->readAll());
}

void BooksController::setBooks()
{
    // Parse JSON
    if(mNetReply->error()){
        qDebug() << "There was an error: " << mNetReply->errorString();
    }else{
        mLastResults = 0;
        QJsonDocument doc = QJsonDocument::fromJson(*mDataBuffer);
        if(doc.isObject() == false) qDebug() << "It is not a JSON object";
        Volume* volume = new Volume();
        volume->kind = doc.object().value("kind").toString();
        volume->totalItems = doc.object().value("totalItems").toInteger();
        QJsonArray items = doc.object().value("items").toArray();
        for(auto jsonObj : items){
            QJsonObject object = jsonObj.toObject();
            Book* book = new Book();
            book->id = object.value("id").toString();
            QJsonObject info = object.value("volumeInfo").toObject();
            book->title = info.value("title").toString();
            QJsonArray authors = info.value("authors").toArray();
            for(auto author : authors){
                book->authors.append(author.toString());
            }
            QJsonObject thumbnails = info.value("imageLinks").toObject();
            book->thumbnail = thumbnails.value("thumbnail").toString();
            book->description = info.value("description").toString();
            QJsonObject saleInfo = object.value("saleInfo").toObject();
            book->buyLink = saleInfo.value("buyLink").toString();
            if (inFavorites(book->id))
                book->favorite = true;
            //qDebug() << book->toString();
            mBooks.append(book);
            books.append(book->id);
            ThumbnailController* thumbnailController = new ThumbnailController();
            thumbnailController->fetchThumbnail(book->thumbnail);
            thumbnailController->mBookId = book->id;
            mThumbnailControllers.append(thumbnailController);
            connect(thumbnailController, &ThumbnailController::thumbnailUpdated, this, &BooksController::thumbnailUpdate);
            ++mLastResults;
        }
        delete volume;
    }
    delete mDataBuffer;
    mDataBuffer=nullptr;
    if(mLastResults==MAXRESULTS)
        ++mCurrentPage;
}

bool BooksController::inFavorites(QString id){
    for (const QString& fav : favorites){
        if (fav == id) return true;
    }
    return false;
}

Book* BooksController::getBook(QString id)
{
    for (QList<Book*>::const_iterator it = mBooks.cbegin(); it != mBooks.cend(); ++it)
    {
        if ((*it)->id == id) return *it;
    }
    return nullptr;
}

Book* BooksController::getBookByTitle(QString title){
    for (QList<Book*>::const_iterator it = mBooks.cbegin(); it != mBooks.cend(); ++it)
    {
        if ((*it)->title == title) return *it;
    }
    return nullptr;
}

QPixmap* BooksController::getThumbnail(QString id)
{
    for (QList<ThumbnailController*>::const_iterator it = mThumbnailControllers.cbegin(); it != mThumbnailControllers.cend(); ++it)
    {
        if ((*it)->mBookId == id) return &(*it)->mThumbnail;
    }
    return nullptr;
}

void BooksController::setFavorite(QString id){
    Book* selectedBook = getBook(id);
    if (selectedBook)
    {
        selectedBook->favorite = true;
        favorites.append(selectedBook->id);
        writeFavorite(selectedBook->id);
    }
}

void BooksController::setUnFavorite(QString id){
    Book* selectedBook = getBook(id);
    if (selectedBook)
    {
        selectedBook->favorite = false;
        favorites.removeAll(selectedBook->id);
        writeAllFavorites();
    }
}

void BooksController::writeAllFavorites(){
    uint count = 1;
    mSettings->beginGroup("Favorites");
    for (const QString& id : favorites){
        mSettings->setValue(QVariant(count).toString(), id);
        ++count;
    }
    mSettings->setValue(QVariant(count).toString(), "");
    mSettings->endGroup();
}

void BooksController::writeFavorite(QString id)
{
    uint position = favorites.size();
    mSettings->beginGroup("Favorites");
    mSettings->setValue(QVariant(position).toString(), id);
    mSettings->endGroup();
}

void BooksController::readFavorites(){
    mSettings->beginGroup("Favorites");
    QString id;
    uint count = 1;
    do {
        id = mSettings->value(QVariant(count).toString()).toString();
        if (id != ""){
            favorites.append(id);
            ++count;
        } else {
            break;
        }
    }while (true);
    mSettings->endGroup();
}

void BooksController::thumbnailUpdate(QString id){
    emit thumbnailUpdated(id);
}
