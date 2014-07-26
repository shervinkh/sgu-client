#ifndef ONLINESTATUS_H
#define ONLINESTATUS_H

#include <QWidget>
#include "statusprocessor.h"
#include "filedownloader.h"

class StatusView;
class AccountManager;
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

class OnlineStatus : public QWidget
{
    Q_OBJECT
private:
    AccountManager *am;
    FileDownloader *fd;
    StatusView *sv;
    QList<StatusInfo> infos;

    QLabel *text;

    QLabel *ovp;
    QProgressBar *ovpb;
    QHBoxLayout *progress;

    QTimer *timer;
    QLabel *refrate;
    QLineEdit *refresh;
    QPushButton *setbut;
    QPushButton *refnow;
    QLabel *persec;
    QLabel *stat;
    QLabel *status;
    QHBoxLayout *downBox;

    QVBoxLayout *layou;

    friend QDataStream & operator<<(QDataStream &ds, const OnlineStatus &in);
    friend QDataStream & operator>>(QDataStream &ds, OnlineStatus &in);

public:
    explicit OnlineStatus(QWidget *parent = nullptr, AccountManager *am = nullptr, FileDownloader *fd = nullptr);

public slots:
    void updateStatus();

private slots:
    void changeTime();
};

#endif // ONLINESTATUS_H
