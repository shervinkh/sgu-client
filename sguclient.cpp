#include "sguclient.h"
#include "friendactivity.h"
#include "accountmanager.h"
#include "filedownloader.h"
#include "onlinestatus.h"
#include "competition.h"
#include "suggestion.h"
#include <QtGui>

SGUClient::SGUClient(QWidget *parent) :
    QMainWindow(parent)
{
    friendAct = new QPushButton(tr("Friends activity"));
    competit = new QPushButton(tr("Competitions"));
	sugg = new QPushButton(tr("Suggestions"));
    stat = new QPushButton(tr("Status"));
    accs = new QPushButton(tr("Accounts"));

	friendAct->setMinimumHeight( friendAct->sizeHint().height() * 1.3 );
	competit->setMinimumHeight( competit->sizeHint().height() * 1.3 );
	sugg->setMinimumHeight( sugg->sizeHint().height() * 1.3 );
	stat->setMinimumHeight( stat->sizeHint().height() * 1.3 );
	accs->setMinimumHeight( accs->sizeHint().height() * 1.3 );

    butts = new QVBoxLayout;
    butts->addWidget(friendAct);
    butts->addWidget(competit);
	butts->addWidget(sugg);
    butts->addWidget(stat);
    butts->addWidget(accs);

    sw = new QStackedWidget;

    layou = new QHBoxLayout;
    layou->addLayout(butts);
    layou->addWidget(sw);

    wid = new QWidget;
    wid->setLayout(layou);

    fd = new FileDownloader(this);
    am = new AccountManager(this, fd);
    fa = new FriendActivity(this, am);
    so = new OnlineStatus(this, am, fd);
    com = new Competition(this, am);
	sug = new Suggestion(this, am);

    sw->addWidget(fa);
    sw->addWidget(so);
    sw->addWidget(am);
    sw->addWidget(com);
	sw->addWidget(sug);

    sw->setCurrentWidget(fa);

    connect(accs, SIGNAL(clicked()), this, SLOT(accPage()));
    connect(friendAct, SIGNAL(clicked()), this, SLOT(faPage()));
    connect(stat, SIGNAL(clicked()), this, SLOT(statPage()));
    connect(competit, SIGNAL(clicked()), this, SLOT(comPage()));
	connect(sugg, SIGNAL(clicked()) , this, SLOT(sugPage()) );

    setCentralWidget(wid);
    setWindowTitle("SGU Client By Leo145");

    QTimer::singleShot(0, this, SLOT(loadCache()));
}

SGUClient::~SGUClient()
{
    QFile file(".SGU_Client_Cached_Data");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream ds(&file);
		ds << *am << *fa << *so << *com << *sug;
    }
    file.close();
}

void SGUClient::loadCache()
{
    QFile file(".SGU_Client_Cached_Data");
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream ds(&file);
		ds >> *am >> *fa >> *so >> *com >> *sug;
    }
    file.close();
}

void SGUClient::accPage()
{
    sw->setCurrentWidget(am);
}

void SGUClient::faPage()
{
    sw->setCurrentWidget(fa);
}

void SGUClient::statPage()
{
    sw->setCurrentWidget(so);
}

void SGUClient::comPage()
{
    sw->setCurrentWidget(com);
}

void SGUClient::sugPage(){
	sw->setCurrentWidget(sug);
}
