#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QWidget>
#include <QString>
#include <QSet>
#include <QMap>
#include <QListWidgetItem>
#include "accountprocessor.h"
#include "statusprocessor.h"
#include "filedownloader.h"

class QLabel;
class QPushButton;
class QListWidget;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;

class AccountManager : public QWidget
{
    Q_OBJECT
private:
    QString myID;
    QString myPass;
    AccountProcessor me;
    StatusProcessor mestat;
    QSet<QString> friendIDs;
    QMap<QString, AccountProcessor> friendInfos;
    QMap<QString, StatusProcessor> friendStats;
    QMap<QString, QSet<int> > friendSubmits;

    QLabel *l_id, *l_pass;
    QLineEdit *le_id, *le_pass;
    QPushButton *saveButton;
    QHBoxLayout *myBox;

    QLabel *title;

    QLabel *myinf;
    QLabel *me_name;
    QLabel *me_fullname;
    QLabel *me_email;
    QLabel *me_birth;
    QLabel *me_sub;
    QLabel *me_ac;
    QLabel *me_rat;
    QPushButton *myprof;
    QHBoxLayout *m1, *m2, *m3, *m4;
    QVBoxLayout *myInfoBox;

    QLabel *viewProfileOf;
    QLineEdit *viewProfileEdit;
    QPushButton *viewbut;
    QHBoxLayout *viewPlay;

    QLabel *friends;

    QListWidget *lw;
    QPushButton *ad;
    QPushButton *rem;
    QPushButton *vprof;
    QVBoxLayout *helpBox;
    QHBoxLayout *friendBox;

    QLabel *finf;
    QLabel *f_name;
    QLabel *f_fullname;
    QLabel *f_email;
    QLabel *f_birth;
    QLabel *f_sub;
    QLabel *f_ac;
    QLabel *f_rat;
    QHBoxLayout *f1, *f2, *f3;
    QVBoxLayout *fInfoBox;

    QVBoxLayout *box;

    FileDownloader *fd;

    bool lastUpdateStatSuccess;

    void updateMe();

signals:
    void changed();

public:
    explicit AccountManager(QWidget *parent = nullptr, FileDownloader *fdo = nullptr);
    bool isFriend(const QString &id) const;
    QString ID() const {return myID;}
    QString pass() const {return myPass;}
    const QSet<QString> & friendsID() const {return friendIDs;}
    void add(const QString &id);
    void remove(const QString &id);
    AccountProcessor data(const QString &id, QProgressBar *pb = nullptr) const;
    StatusProcessor statData(const QString &id, QProgressBar *pb = nullptr) const;
    bool updateData(QProgressBar *pb = nullptr, QProgressBar *ppb = nullptr);
    bool updateStatus(QProgressBar *pb = nullptr, QProgressBar *ppb = nullptr);
    bool updateSubmits(QProgressBar *pb = nullptr, QProgressBar *ppb = nullptr);
    AccountProcessor ownInfo() const {return me;}
    StatusProcessor ownStatus() const {return mestat;}
    AccountProcessor infoOf(const QString &id) const {return friendInfos[id];}
    StatusProcessor statOf(const QString &id) const {return friendStats[id];}
    QSet<int> submitsOf(const QString &id) const {return friendSubmits[id];}
    void emitChanged() {emit changed();}

    friend QDataStream & operator<<(QDataStream &ds, const AccountManager &in);
    friend QDataStream & operator>>(QDataStream &ds, AccountManager&in);

private slots:
    void saveme();
    void removebut();
    void addbut();
    void profMe();
    void profFir();
    void profOther();
    void profDC(QListWidgetItem *in);
    void setInfo(QListWidgetItem *cur, QListWidgetItem *);
};

#endif // ACCOUNTMANAGER_H
