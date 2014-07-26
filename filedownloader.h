#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QtCore/qtconcurrentexception.h>
#include <QNetworkReply>
#include <QMap>

class QUrl;
class QProgressBar;
class QNetworkAccessManager;
class QTimer;

class NoResultException : QtConcurrent::Exception
{
public:
    void raise() const {throw *this;}
    NoResultException *clone() const { return new NoResultException(*this); }
};

class FileDownloader : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *network;
    QMap<QNetworkReply *, QProgressBar *> pb;
    QMap<QNetworkReply *, QByteArray> res;
    QMap<QNetworkReply *, bool> err;
    QMap<QNetworkReply *, bool> done;
    QMap<QNetworkReply *, QTimer *> timers;
    QMap<QTimer *, QNetworkReply *> timers2;

public:
    explicit FileDownloader(QObject *parent = nullptr);
    ~FileDownloader();
    QNetworkReply * getURL(const QUrl &url, QProgressBar *prb = nullptr);
    QByteArray popResult(QNetworkReply *nr);
    bool isDone(QNetworkReply *nr) const {if (done.find(nr) == done.end()) (new NoResultException)->raise(); return done[nr];}
    bool isSuccessful(QNetworkReply *nr) const {if (err.find(nr) == err.end()) (new NoResultException)->raise(); return !err[nr];}
    
private slots:
    void updateProgress(qint64 cur, qint64 tot);
    void operationFinished();
    void errorOccured(QNetworkReply::NetworkError error);
    void readData();
    void timedOut();
};

#endif // FILEDOWNLOADER_H
