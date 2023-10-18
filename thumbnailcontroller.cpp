#include "thumbnailcontroller.h"
#include "qimage.h"
#include "qpixmap.h"

ThumbnailController::ThumbnailController(QObject *parent)
    : QObject{parent},
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(nullptr),
    mDataBuffer(nullptr)
{

}

ThumbnailController::~ThumbnailController()
{
    delete mDataBuffer;
    delete mNetReply;
    delete mNetManager;
}

void ThumbnailController::fetchThumbnail(QString url){
    mDataBuffer = new QByteArray;
    QUrl thumbUrl(url);
    QNetworkRequest request;
    request.setUrl(thumbUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");
    mNetReply = mNetManager->get(request);
    connect(mNetReply, &QIODevice::readyRead, this, &ThumbnailController::dataReadyRead);
    connect(mNetReply, &QNetworkReply::finished, this, &ThumbnailController::setThumbnail);
}

void ThumbnailController::dataReadyRead()
{
    mDataBuffer->append(mNetReply->readAll());
}

void ThumbnailController::setThumbnail(){
    // Parse JSON
    if(mNetReply->error()){
        qDebug() << "There was an error: " << mNetReply->errorString();
    }else{
        QPixmap returnImage;
        if (returnImage.loadFromData(*mDataBuffer))
        {
            mThumbnail = returnImage;
            emit thumbnailUpdated(mBookId);
        }
    }
    delete mDataBuffer;
    mDataBuffer=nullptr;
}
