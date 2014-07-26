#ifndef COMPETITIONDETAIL_H
#define COMPETITIONDETAIL_H

#include <QDialog>
#include "competitionview.h"

class AccountManager;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class CompetitionDetail : public QDialog
{
    Q_OBJECT
private:
    bool isVal;

    QLabel *info;

    QLabel *target;
    QLabel *compto;
    QHBoxLayout *hb;

    QLabel *summ;

    QLabel *fir, *sec;

    QVBoxLayout *lay;

    QString toStr(const QSet<int> &in) const;

public:
    explicit CompetitionDetail(AccountManager *am, const CompetitionInfo &inf
                               , const QString &summary, QWidget *parent = nullptr);
    bool isValid() const {return isVal;}
};

#endif // COMPETITIONDETAIL_H
