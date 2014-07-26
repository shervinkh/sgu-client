#ifndef COMPETITIONVIEW_H
#define COMPETITIONVIEW_H

#include <QTableWidget>
#include <QList>
#include <QSize>
#include "statusprocessor.h"

enum CompetitionType {MoreAc, NearAc, DiffAc, DiffSub};

class CompetitionInfo
{
private:
    CompetitionType CType;
    QString TargetID;

public:
    CompetitionInfo() {}
    CompetitionInfo(CompetitionType T, const QString &str)
    {
        CType = T;
        TargetID = str;
    }
    bool operator==(const CompetitionInfo &ci) const
    {
        return CType == ci.CType && TargetID == ci.TargetID;
    }

    CompetitionType type() const {return CType;}
    QString target() const {return TargetID;}
};

class AccountManager;

class CompetitionView : public QTableWidget
{
    Q_OBJECT
private:
    AccountManager *am;
    QList<CompetitionInfo> ids;

    QString reason(CompetitionType T) const;

protected:
    QSize sizeHint() const;

public:
    explicit CompetitionView(QWidget *parent = nullptr, AccountManager *am = nullptr);
    void setData(const QList<CompetitionInfo> &in);

private slots:
    void handleDoubleClick(int row, int col);
};

#endif // COMPETITIONVIEW_H
