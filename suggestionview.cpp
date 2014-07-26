#include "suggestionview.h"
#include "accountmanager.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QList>
#include <QString>

//Replace them if it's possible
#include <fstream>
#include <algorithm>

SuggestionView::SuggestionView(QWidget *parent, AccountManager *am) :
	QTableWidget(parent)
{
	this->am = am;

	setColumnCount(3);
	setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Prob No")));
	setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Prob Name")));
	setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Accepts")));

	loadProbNames();
}

void SuggestionView::setData()
{
	initData();

	setRowCount(0);
	setRowCount(probs.size());

	for(int i =0 ; i <  probs.size(); i++ )
	{
		setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i+1)));

		QTableWidgetItem *probNum = new QTableWidgetItem;
		probNum->setText(QString::number(probs[i].second));
		probNum->setFlags(Qt::ItemIsEnabled);

		QTableWidgetItem *name = new QTableWidgetItem;
		name->setText(probName[probs[i].second]);
		name->setFlags(Qt::ItemIsEnabled);

		QTableWidgetItem *probAcc = new QTableWidgetItem;
		probAcc->setText(QString::number(probs[i].first));
		probAcc->setFlags(Qt::ItemIsEnabled);

		if( probs[i].first >= 4 ) {
			probNum->setBackgroundColor(Qt::red);
			name->setBackgroundColor(Qt::red);
			probAcc->setBackgroundColor(Qt::red);
			probNum->setTextColor(Qt::yellow);
			name->setTextColor(Qt::yellow);
			probAcc->setTextColor(Qt::yellow);
		} else {
			probNum->setBackgroundColor(Qt::yellow);
			name->setBackgroundColor(Qt::yellow);
			probAcc->setBackgroundColor(Qt::yellow);
			probNum->setTextColor(Qt::red);
			name->setTextColor(Qt::red);
			probAcc->setTextColor(Qt::red);
		}

		setItem(i,0,probNum);
		setItem(i,1,name);
		setItem(i,2,probAcc);
	}

	resizeColumnsToContents();
}

void SuggestionView::initData()
{
	for(int i = 0 ; i < maxProbs; i++ )
		numOfAcc[i] = 0;

	foreach(QString str, am->friendsID())
	{
		AccountProcessor m = am->ownInfo();
		AccountProcessor h = am->infoOf(str);

		QSet<int> diff = h.acceptedProblems()-m.acceptedProblems();
		foreach(int prob, diff )
			numOfAcc[prob]++;
	}

	probs.clear();
	for(int i = 0 ; i < maxProbs ; i++ )
		if( numOfAcc[i] )
			probs.append(QPair<int,int>(numOfAcc[i], i));

	std::sort(probs.begin(), probs.end() );
	std::reverse( probs.begin() , probs.end() );
}

void SuggestionView::loadProbNames()
{
	std::ifstream fin(".probNames");

	int counter = 100;
	std::string s;
	while( getline(fin, s) )
		probName[counter++] = QString::fromStdString(s);

	fin.close();
}
