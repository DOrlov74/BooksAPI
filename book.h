#ifndef BOOK_H
#define BOOK_H

#include <QObject>

class Book : public QObject
{
    Q_OBJECT
public:
    explicit Book(QObject *parent = nullptr);

    QString id;
    QString title;
    QList<QString> authors;
    QString thumbnail;
    QString description;
    QString buyLink;
    bool favorite = false;

    QString toString();

signals:

};

#endif // BOOK_H
