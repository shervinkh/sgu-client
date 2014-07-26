#ifndef COMPETITION_H
#define COMPETITION_H

#include <QWidget>
#include "competitionview.h"

class AccountManager;
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

class Competition : public QWidget
{
    Q_OBJECT
private:
    AccountManager *am;
    CompetitionView *cv;
    QList<CompetitionInfo> infos;

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

    friend QDataStream & operator<<(QDataStream &ds, const Competition &in);
    friend QDataStream & operator>>(QDataStream &ds, Competition &in);

public:
    explicit Competition(QWidget *parent = nullptr, AccountManager *am = nullptr);

public slots:
    void updateStatus();

private slots:
    void changeTime();
};

#endif // COMPETITION_H
