#ifndef SUGGESTION_H
#define SUGGESTION_H

#include <QWidget>
#include "suggestionview.h"

class AccountManager;
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

class Suggestion : public QWidget
{
	Q_OBJECT
private:
	AccountManager *am;
	SuggestionView *sv;

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

	friend QDataStream & operator<<(QDataStream &ds, const Suggestion &in);
	friend QDataStream & operator>>(QDataStream &ds, Suggestion &in);

public:
	explicit Suggestion(QWidget *parent = 0, AccountManager *am = nullptr );


public slots:
	void updateStatus();
private slots:
	void changeTime();
};

#endif // SUGGESTION_H
