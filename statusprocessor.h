#ifndef STATUSPROCESSOR_H
#define STATUSPROCESSOR_H

#include <QString>
#include <QList>
#include <QDateTime>
#include <QtCore/qtconcurrentexception.h>

enum ResultType {AC, WA, RE, GE, PE, TL, ML, CE, WAIT, RUN};

class StatusInfo
{
private:
    QString Subid;
    QString Id;
    QString Name;
    QString Lang;
    QDateTime Datetime;
    ResultType RT;
    int Testnumber;
    int Time;
    int Memory;
    int Problem;

public:
    StatusInfo() {}
    StatusInfo(const QString &a, const QString &b, const QString &c, const QString &d, const QDateTime &e,
               ResultType r, int t1, int t2, int m, int p);
    QString submitID() const {return Subid;}
    QString ID() const {return Id;}
    QString name() const {return Name;}
    QDateTime dateTime() const {return Datetime;}
    QString language() const {return Lang;}
    QString shortResult() const;
    QString longResult() const;
    ResultType resultType() const {return RT;}
    int testNumber() const {return Testnumber;}
    int time() const {return Time;}
    int memory() const {return Memory;}
    int problem() const {return Problem;}
    bool operator<(const StatusInfo &in) const {return Datetime>in.Datetime;}

    friend QDataStream & operator<<(QDataStream &ds, const StatusInfo &in);
    friend QDataStream & operator>>(QDataStream &ds, StatusInfo &in);
};

#ifndef INVALIDINPUT
#define INVALIDINPUT
class InvalidInput : QtConcurrent::Exception
{
public:
    void raise() const {throw *this;}
    InvalidInput *clone() const { return new InvalidInput(*this); }
};
#endif

class StatusProcessor
{
private:
    bool err;
    QList<StatusInfo> info;
    int diffsec;

    ResultType processType(const QString &in) const;
    int processNumb(const QString &in) const;
    QDateTime processDate(const QString &in) const;

public:
    StatusProcessor() {err = true;}
    StatusProcessor(const QString &src, int secs);
    QList<StatusInfo> data() const {if (err) (new InvalidInput)->raise(); return info;}
    bool isValid() const {return !err;}

    friend QDataStream & operator<<(QDataStream &ds, const StatusProcessor &in);
    friend QDataStream & operator>>(QDataStream &ds, StatusProcessor &in);
};

#endif // STATUSPROCESSOR_H
