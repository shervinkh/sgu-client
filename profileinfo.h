#ifndef PROFILEINFO_H
#define PROFILEINFO_H

#include <QDialog>

class AccountManager;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;

class ProfileInfo : public QDialog
{
    Q_OBJECT
private:
    bool isVal;

    QString SID;
    AccountManager *am;

    QLabel *ID;

    QLabel *name;
    QLabel *fname;
    QHBoxLayout *f1;

    QLabel *email;
    QLabel *birth;
    QLabel *country;
    QHBoxLayout *f2;

    QLabel *study;

    QLabel *info;

    QLabel *sub;
    QLabel *ac;
    QLabel *rat;
    QHBoxLayout *f3;

    QLabel *ac1;
    QLabel *ac2;
    QLabel *ac3;
    QLabel *ac4;

    QPushButton *substat;
    QHBoxLayout *sublay;

    QVBoxLayout *lay;

    QString toStr(const QSet<int> &in) const;

public:
    explicit ProfileInfo(QWidget *parent = nullptr, AccountManager *am = nullptr, const QString &id = nullptr);
    bool isValid() const {return isVal;}

private slots:
    void submitStat();
};

#endif // PROFILEINFO_H
