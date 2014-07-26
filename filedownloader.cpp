#include "filedownloader.h"
#include <QtGui>
#include <QtNetwork>

FileDownloader::FileDownloader(QObject *parent) :
    QObject(parent)
{
    network = new QNetworkAccessManager;
}

FileDownloader::~FileDownloader()
{
    foreach(QNetworkReply *th, res.keys())
        th->abort();
}

QNetworkReply * FileDownloader::getURL(const QUrl &url, QProgressBar *prb)
{
    QNetworkReply *control = network->get(QNetworkRequest(url));
    connect(control, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
    connect(control, SIGNAL(finished()), this, SLOT(operationFinished()));
    connect(control, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorOccured(QNetworkReply::NetworkError)));
    connect(control, SIGNAL(readyRead()), this, SLOT(readData()));

    timers[control] = new QTimer;
    connect(timers[control], SIGNAL(timeout()), this, SLOT(timedOut()));
    timers2[timers[control]] = control;
    timers[control]->start(10000);
    err[control] = false;
    done[control] = false;
    pb[control] = prb;
    res[control] = QByteArray();

    return control;
}

void FileDownloader::updateProgress(qint64 cur, qint64 tot)
{
    QNetworkReply *nr = qobject_cast<QNetworkReply *>(sender());

    if (nr && pb[nr])
    {
        pb[nr]->setMaximum(tot);
        pb[nr]->setValue(cur);
    }
}

void FileDownloader::operationFinished()
{
    QNetworkReply *nr = qobject_cast<QNetworkReply *>(sender());

    if (nr)
    {
        nr->close();
        done[nr] = true;
    }
}

void FileDownloader::errorOccured(QNetworkReply::NetworkError error)
{
    QNetworkReply *nr = qobject_cast<QNetworkReply *>(sender());

    if (nr && error)
    {
        err[nr] = true;
        nr->abort();
        done[nr] = true;
    }
}

void FileDownloader::readData()
{
    QNetworkReply *nr = qobject_cast<QNetworkReply *>(sender());

    if (nr)
        res[nr] += nr->readAll();
}

QByteArray FileDownloader::popResult(QNetworkReply *nr)
{
    if (res.find(nr) == res.end())
        (new NoResultException)->raise();

    QByteArray ret = res[nr];
    res.remove(nr);
    err.remove(nr);
    done.remove(nr);
    pb.remove(nr);
    timers2.remove(timers[nr]);
    delete timers[nr];
    timers.remove(nr);
    delete nr;

    return ret;
}

void FileDownloader::timedOut()
{
    QTimer *tr = qobject_cast<QTimer *>(sender());
    QNetworkReply *nr = timers2[tr];

    if (nr)
    {
        err[nr] = true;
        nr->abort();
        done[nr] = true;
    }
}
