#ifndef SUGGESTIONVIEW_H
#define SUGGESTIONVIEW_H

#include <QTableWidget>

const int maxProbs = 600;
class AccountManager;

class SuggestionView : public QTableWidget
{
	Q_OBJECT
private:
	AccountManager *am;
	int numOfAcc[maxProbs];
	QString probName[maxProbs];

	QList< QPair<int,int> > probs;

	void loadProbNames();
	void initData();
public:
	explicit SuggestionView(QWidget *parent = 0, AccountManager *am = nullptr);
	void setData();
signals:

public slots:

};

#endif // SUGGESTIONVIEW_H
