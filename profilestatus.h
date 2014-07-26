#ifndef PROFILESTATUS_H
#define PROFILESTATUS_H

#include <QDialog>

class AccountManager;
class QLabel;
class StatusView;
class QVBoxLayout;

class ProfileStatus : public QDialog
{
    Q_OBJECT
private:
    bool isVal;

    QLabel *ID;

    StatusView *sv;

    QVBoxLayout *lay;

public:
    explicit ProfileStatus(QWidget *parent = nullptr, AccountManager *am = nullptr, const QString &id = nullptr);
    bool isValid() const {return isVal;}
};

#endif // PROFILESTATUS_H
