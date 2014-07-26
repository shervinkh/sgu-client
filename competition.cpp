#include "competition.h"
#include "accountmanager.h"
#include "competitionview.h"
#include <QtGui>

Competition::Competition(QWidget *parent, AccountManager *am) :
    QWidget(parent)
{
    this->am = am;
	cv = new CompetitionView(this, am, this);

    text = new QLabel(tr("<h1>Competitions</h1>"));

    ovp = new QLabel(tr("Overal Progress: "));
    ovpb = new QProgressBar;
    pb = new QLabel(tr("Partial Progress: "));
    norpb = new QProgressBar;
    progress = new QHBoxLayout;

    progress->addWidget(ovp);
    progress->addWidget(ovpb);
    progress->addStretch();
    progress->addWidget(pb);
    progress->addWidget(norpb);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    refrate = new QLabel(tr("Refresh every "));
    refresh = new QLineEdit("60");
    refresh->setValidator(new QIntValidator(1, 3600));
    refrate->setBuddy(refresh);
    persec = new QLabel(tr("seconds"));
    setbut = new QPushButton(tr("Set"));
    refnow = new QPushButton(tr("Refresh Now!"));

	notiCheck = new QCheckBox("System Notification");
	notiCheck->setChecked(1);

    connect(refnow, SIGNAL(clicked()), this, SLOT(updateStatus()));
    connect(setbut, SIGNAL(clicked()), this, SLOT(changeTime()));
    stat = new QLabel(tr("Status: "));
    status = new QLabel(tr("<font color=blue>Synced!</font>"));
    downBox = new QHBoxLayout;
    downBox->addWidget(refrate);
    downBox->addWidget(refresh);
    downBox->addWidget(persec);
    downBox->addWidget(setbut);
    downBox->addWidget(refnow);
	downBox->addWidget(notiCheck);
    downBox->addStretch();
    downBox->addWidget(stat);
    downBox->addWidget(status);

    layou = new QVBoxLayout;
    layou->addWidget(text);
    layou->addWidget(cv);
    layou->addLayout(progress);
    layou->addLayout(downBox);

    setLayout(layou);

    QTimer::singleShot(0, this, SLOT(updateStatus()));
    connect(am, SIGNAL(changed()), this, SLOT(updateStatus()));
    timer->start(60000);
}

void Competition::updateStatus()
{
    status->setText(tr("<font color=green>Syncing...</font>"));

    if (am->ID() == "Not Set")
    {
        status->setText("<font color=red>Failed to sync!</font>");
        return;
    }

    bool res = true;
    if (!am->updateSubmits(ovpb, norpb))
        res = false;

    infos.clear();

    foreach(QString str, am->friendsID())
    {
        AccountProcessor m = am->ownInfo();
        AccountProcessor h = am->infoOf(str);

        if (h.numAccepts() > m.numAccepts())
            infos.append(CompetitionInfo(MoreAc, str));
        else if (h.numAccepts() >= m.numAccepts()-10)
            infos.append(CompetitionInfo(NearAc, str));

        QSet<int> diff = h.acceptedProblems()-m.acceptedProblems();

        if (diff.size() > 0)
            infos.append(CompetitionInfo(DiffAc, str));

        if (am->submitsOf(str).size() > 0)
            infos.append(CompetitionInfo(DiffSub, str));
    }

    cv->setData(infos);

    if (res)
        status->setText("<font color=blue>Synced!</font>");
    else
        status->setText("<font color=red>Failed to sync!</font>");

    ovpb->reset();
    norpb->reset();
}

void Competition::changeTime()
{
    int interval = refresh->text().toInt();

    if (interval > 0)
        timer->start(interval*1000);
}

bool Competition::hasNoti()
{
	return ( notiCheck->checkState() == Qt::Checked ) ;
}

QDataStream & operator<<(QDataStream &ds, const Competition &in)
{
	ds << in.refresh->text() << ( in.notiCheck->checkState() == Qt::Checked ) ;

    return ds;
}

QDataStream & operator>>(QDataStream &ds, Competition &in)
{
	QString tmp;
    ds >> tmp;
    in.refresh->setText(tmp);
    in.changeTime();

	bool ch;
	ds >> ch;
	in.notiCheck->setChecked(ch);

    return ds;
}
