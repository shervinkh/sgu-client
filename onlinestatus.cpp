#include "onlinestatus.h"
#include "accountmanager.h"
#include "statusview.h"
#include <QtGui>

OnlineStatus::OnlineStatus(QWidget *parent, AccountManager *am, FileDownloader *fd) :
    QWidget(parent)
{
    this->am = am;
    this->fd = fd;
    sv = new StatusView(this, am);

    text = new QLabel(tr("<h1>Online Status</h1>"));

    ovp = new QLabel(tr("Overal Progress: "));
    ovpb = new QProgressBar;
    progress = new QHBoxLayout;

    progress->addWidget(ovp);
    progress->addWidget(ovpb);
    progress->addStretch();

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

void OnlineStatus::updateStatus()
{
    if (sender() == am)
    {
        sv->setData(infos);
        return;
    }

    status->setText(tr("<font color=green>Syncing...</font>"));

    QNetworkReply *nr = fd->getURL(QString("http://acm.sgu.ru/status.php"), ovpb);

    while (!fd->isDone(nr))
        qApp->processEvents();

    if (!fd->isSuccessful(nr))
        status->setText("<font color=red>Failed to sync!</font>");
    else
    {
        StatusProcessor *sp = new StatusProcessor(fd->popResult(nr), -1800);
        infos = sp->data();
        sv->setData(infos);
        status->setText("<font color=blue>Synced!</font>");
    }

    ovpb->reset();
}

void OnlineStatus::changeTime()
{
    int interval = refresh->text().toInt();

    if (interval > 0)
        timer->start(interval*1000);
}

QDataStream & operator<<(QDataStream &ds, const OnlineStatus &in)
{
    ds << in.infos << in.refresh->text();

    return ds;
}

QDataStream & operator>>(QDataStream &ds, OnlineStatus &in)
{
    QString tmp;
    ds >> in.infos >> tmp;
    in.refresh->setText(tmp);
    in.changeTime();
    in.sv->setData(in.infos);

    return ds;
}
