#include "suggestion.h"
#include "accountmanager.h"
#include <QtGui>

Suggestion::Suggestion(QWidget *parent, AccountManager *am) :
	QWidget(parent)
{
	this->am = am;
	sv = new SuggestionView(this, am);

	text = new QLabel(tr("<h1>Suggestions</h1>"));

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
	downBox->addStretch();
	downBox->addWidget(stat);
	downBox->addWidget(status);

	layou = new QVBoxLayout;
	layou->addWidget(text);
	layou->addWidget(sv);
	layou->addLayout(progress);
	layou->addLayout(downBox);

	setLayout(layou);

	QTimer::singleShot(0, this, SLOT(updateStatus()));
	connect(am, SIGNAL(changed()), this, SLOT(updateStatus()));
	timer->start(60000);
}

void Suggestion::updateStatus()
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

	sv->setData();

	if (res)
		status->setText("<font color=blue>Synced!</font>");
	else
		status->setText("<font color=red>Failed to sync!</font>");

	ovpb->reset();
	norpb->reset();
}

void Suggestion::changeTime()
{
	int interval = refresh->text().toInt();

	if (interval > 0)
		timer->start(interval*1000);
}

QDataStream & operator<<(QDataStream &ds, const Suggestion &in)
{
	ds << in.refresh->text();

	return ds;
}

QDataStream & operator>>(QDataStream &ds, Suggestion &in)
{
	QString tmp;
	ds >> tmp;
	in.refresh->setText(tmp);
	in.changeTime();

	return ds;
}