#ifndef VOLUME_H
#define VOLUME_H

#include <QObject>
#include "book.h"

class Volume : public QObject
{
    Q_OBJECT
public:
    explicit Volume(QObject *parent = nullptr);

    QString kind;
    qint64 totalItems;
    QList<Book> items;

    QString toString();

signals:

};

#endif // VOLUME_H
