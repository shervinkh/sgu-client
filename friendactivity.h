#ifndef FRIENDACTIVITY_H
#define FRIENDACTIVITY_H

#include <QWidget>
#include "statusprocessor.h"

class StatusView;
class AccountManager;
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

class FriendActivity : public QWidget
{
    Q_OBJECT
private:
    AccountManager *am;
    StatusView *sv;

    QLabel *text;

    QLabel *ovp;
    QProgressBar *ovpb;
    QLabel *pb;
    QProgressBar *norpb;
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

    friend QDataStream & operator<<(QDataStream &ds, const FriendActivity &in);
    friend QDataStream & operator>>(QDataStream &ds, FriendActivity &in);

public:
    explicit FriendActivity(QWidget *parent = nullptr, AccountManager *am = nullptr);

public slots:
    void updateStatus();

private slots:
    void changeTime();
};

#endif // FRIENDACTIVITY_H
