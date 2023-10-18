#ifndef THUMBNAILCONTROLLER_H
#define THUMBNAILCONTROLLER_H

#include "qnetworkreply.h"
#include "qpixmap.h"
#include <QObject>

class ThumbnailController : public QObject
{
    Q_OBJECT
public:
    explicit ThumbnailController(QObject *parent = nullptr);
    virtual ~ThumbnailController();

    Q_INVOKABLE void fetchThumbnail(QString url);
    QPixmap mThumbnail;
    QString mBookId;

signals:
    void thumbnailUpdated(QString id);

private slots:
    void dataReadyRead();
    void setThumbnail();

private:
    QNetworkAccessManager* mNetManager;
    QNetworkReply* mNetReply;
    QByteArray* mDataBuffer;
};

#endif // THUMBNAILCONTROLLER_H
