#ifndef SUGGESTION_H
#define SUGGESTION_H

#include <QWidget>

class AccountManager;
class Suggestion : public QWidget
{
	Q_OBJECT
public:
	explicit Suggestion(QWidget *parent = 0, AccountManager *am = nullptr );

signals:

public slots:

};

#endif // SUGGESTION_H
