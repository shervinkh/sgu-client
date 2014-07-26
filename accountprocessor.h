#ifndef ACCOUNTPROCESSOR_H
#define ACCOUNTPROCESSOR_H


#include <QString>
#include <QSet>
#include <QtCore/qtconcurrentexception.h>

#ifndef INVALIDINPUT
#define INVALIDINPUT
class InvalidInput : QtConcurrent::Exception
{
public:
    void raise() const {throw *this;}
    InvalidInput *clone() const { return new InvalidInput(*this); }
};
#endif

class AccountProcessor
{
private:
    QString a_name;
    QString a_email;
    QString a_fullname;
    QString a_country;
    QString a_studyplace;
    QString a_birthday;
    QString a_info;

    int num_submits;
    int num_accepts;
    QSet<int> problems;
    QSet<int> lastDay;
    QSet<int> lastWeek;
    QSet<int> lastMonth;

    bool err;

    void check() const {if (err) (new InvalidInput)->raise();}

public:
    AccountProcessor() {err = true;}
    AccountProcessor(const QString &src);
    bool isValid() const {return !err;}
    QString name() const {check(); return a_name;}
    QString email() const {check(); return a_email;}
    QString fullName() const {check(); return a_fullname;}
    QString country() const {check(); return a_country;}
    QString studyPlace() const {check(); return a_studyplace;}
    QString birthday() const {check(); return a_birthday;}
    QString information() const {check(); return a_info;}
    int numSubmits() const {check(); return num_submits;}
    int numAccepts() const {check(); return num_accepts;}
    QSet<int> acceptedProblems() const {check(); return problems;}
    QSet<int> acceptedLastDay() const {check(); return lastDay;}
    QSet<int> acceptedLastWeek() const {check(); return lastWeek;}
    QSet<int> acceptedLastMonth() const {check(); return lastMonth;}

    friend QDataStream & operator<<(QDataStream &ds, const AccountProcessor &in);
    friend QDataStream & operator>>(QDataStream &ds, AccountProcessor &in);
};

#endif // ACCOUNT_PROCESSOR_H
