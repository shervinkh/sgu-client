#include "sguclient.h"
#include "friendactivity.h"
#include "accountmanager.h"
#include "filedownloader.h"
#include "onlinestatus.h"
#include "competition.h"
#include <QtGui>

SGUClient::SGUClient(QWidget *parent) :
    QMainWindow(parent)
{
    friendAct = new QPushButton(tr("Friends activity"));
    competit = new QPushButton(tr("Competitions"));
    stat = new QPushButton(tr("Status"));
    accs = new QPushButton(tr("Accounts"));

    butts = new QVBoxLayout;
    butts->addWidget(friendAct);
    butts->addWidget(competit);
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

    sw->addWidget(fa);
    sw->addWidget(so);
    sw->addWidget(am);
    sw->addWidget(com);

    sw->setCurrentWidget(fa);

    connect(accs, SIGNAL(clicked()), this, SLOT(accPage()));
    connect(friendAct, SIGNAL(clicked()), this, SLOT(faPage()));
    connect(stat, SIGNAL(clicked()), this, SLOT(statPage()));
    connect(competit, SIGNAL(clicked()), this, SLOT(comPage()));

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
        ds << *am << *fa << *so << *com;
    }
    file.close();
}

void SGUClient::loadCache()
{
    QFile file(".SGU_Client_Cached_Data");
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream ds(&file);
        ds >> *am >> *fa >> *so >> *com;
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
