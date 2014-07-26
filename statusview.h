#ifndef STATUSVIEW_H
#define STATUSVIEW_H

#include <QTableWidget>
#include <QList>
#include <QSize>
#include "statusprocessor.h"

class AccountManager;

class StatusView : public QTableWidget
{
    Q_OBJECT
private:
    AccountManager *am;
    QList<QString> ids;

protected:
    QSize sizeHint() const;

public:
    explicit StatusView(QWidget *parent = nullptr, AccountManager *am = nullptr);
    void setData(const QList<StatusInfo> &in);

private slots:
    void handleDoubleClick(int row, int col);
};

#endif // STATUSVIEW_H
