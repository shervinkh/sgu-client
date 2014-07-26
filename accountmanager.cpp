#include "accountmanager.h"
#include "accountprocessor.h"
#include "filedownloader.h"
#include "profileinfo.h"
#include <QtGui>

AccountManager::AccountManager(QWidget *parent, FileDownloader *fdo) :
    QWidget(parent)
{
    myID = "Not Set";
    myPass = "Password";

    fd = fdo;

    title = new QLabel(tr("<h1>Accounts Manager</h1>"));
    l_id = new QLabel(tr("My ID:"));
    le_id = new QLineEdit;
    l_id->setBuddy(le_id);

    l_pass = new QLabel(tr("My Pass:"));
    le_pass = new QLineEdit;
    le_pass->setEchoMode(QLineEdit::Password);
    l_pass->setBuddy(le_pass);

    saveButton = new QPushButton(tr("Save"));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveme()));

    myBox = new QHBoxLayout;
    myBox->addWidget(l_id);
    myBox->addWidget(le_id);
    myBox->addStretch();
    myBox->addWidget(l_pass);
    myBox->addWidget(le_pass);
    myBox->addStretch();
    myBox->addWidget(saveButton);

    myinf = new QLabel(tr("<h3>My Informations:</h3>"));
    me_name = new QLabel(tr("<strong>Name:</strong>"));
    me_fullname = new QLabel(tr("<strong>Full Name:</strong>"));
    m1 = new QHBoxLayout;
    m1->addWidget(me_name);
    m1->addStretch();
    m1->addWidget(me_fullname);
    m1->addStretch();
    me_email = new QLabel(tr("<strong>E-Mail:</strong>"));
    me_birth = new QLabel(tr("<strong>Birthday:</strong>"));
    m2 = new QHBoxLayout;
    m2->addWidget(me_email);
    m2->addStretch();
    m2->addWidget(me_birth);
    m2->addStretch();
    me_sub = new QLabel(tr("<strong>Submits:</strong>"));
    me_ac = new QLabel(tr("<strong>Accepts:</strong>"));
    me_rat = new QLabel(tr("(<strong>Ratio:</strong> )"));
    m3 = new QHBoxLayout;
    m3->addWidget(me_sub);
    m3->addStretch();
    m3->addWidget(me_ac);
    m3->addStretch();
    m3->addWidget(me_rat);
    myprof = new QPushButton(tr("My Profile"));
    connect(myprof, SIGNAL(clicked()), this, SLOT(profMe()));
    m4 = new QHBoxLayout;
    m4->addStretch();
    m4->addWidget(myprof);
    m4->addStretch();
    myInfoBox = new QVBoxLayout;
    myInfoBox->addWidget(myinf);
    myInfoBox->addLayout(m1);
    myInfoBox->addLayout(m2);
    myInfoBox->addLayout(m3);
    myInfoBox->addLayout(m4);

    viewProfileOf = new QLabel(tr("View profile of: "));
    viewProfileEdit = new QLineEdit;
    viewbut = new QPushButton(tr("View"));
    viewPlay = new QHBoxLayout;
    connect(viewbut, SIGNAL(clicked()), this, SLOT(profOther()));

    viewPlay->addWidget(viewProfileOf);
    viewPlay->addWidget(viewProfileEdit);
    viewPlay->addWidget(viewbut);
    viewPlay->addStretch();

    friends = new QLabel(tr("<h3>Friends List:</h3>"));

    lw = new QListWidget;
    connect(lw, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(setInfo(QListWidgetItem*,QListWidgetItem*)));
    connect(lw, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(profDC(QListWidgetItem*)));
    ad = new QPushButton(tr("Add"));
    connect(ad, SIGNAL(clicked()), this, SLOT(addbut()));
    rem = new QPushButton(tr("Remove"));
    connect(rem, SIGNAL(clicked()), this, SLOT(removebut()));
    vprof = new QPushButton(tr("View Profile"));
    connect(vprof, SIGNAL(clicked()), this, SLOT(profFir()));
    helpBox = new QVBoxLayout;
    helpBox->addWidget(ad);
    helpBox->addSpacing(5);
    helpBox->addWidget(rem);
    helpBox->addSpacing(5);
    helpBox->addWidget(vprof);
    helpBox->addStretch();
    friendBox = new QHBoxLayout;
    friendBox->addWidget(lw);
    friendBox->addLayout(helpBox);

    finf = new QLabel(tr("<h3>Friend Informations:</h3>"));
    f_name = new QLabel(tr("<strong>Name:</strong>"));
    f_fullname = new QLabel(tr("<strong>Full Name:</strong>"));
    f1 = new QHBoxLayout;
    f1->addWidget(f_name);
    f1->addStretch();
    f1->addWidget(f_fullname);
    f1->addStretch();
    f_email = new QLabel(tr("<strong>E-Mail:</strong>"));
    f_birth = new QLabel(tr("<strong>Birthday:</strong>"));
    f2 = new QHBoxLayout;
    f2->addWidget(f_email);
    f2->addStretch();
    f2->addWidget(f_birth);
    f2->addStretch();
    f_sub = new QLabel(tr("<strong>Submits:</strong>"));
    f_ac = new QLabel(tr("<strong>Accepts:</strong>"));
    f_rat = new QLabel(tr("(<strong>Ratio:</strong> )"));
    f3 = new QHBoxLayout;
    f3->addWidget(f_sub);
    f3->addStretch();
    f3->addWidget(f_ac);
    f3->addStretch();
    f3->addWidget(f_rat);
    fInfoBox = new QVBoxLayout;
    fInfoBox->addWidget(finf);
    fInfoBox->addLayout(f1);
    fInfoBox->addLayout(f2);
    fInfoBox->addLayout(f3);

    box = new QVBoxLayout;
    box->addWidget(title);
    box->addLayout(myBox);
    box->addSpacing(10);
    box->addLayout(myInfoBox);
    box->addStretch();
    box->addLayout(viewPlay);
    box->addStretch();
    box->addWidget(friends);
    box->addLayout(friendBox);
    box->addSpacing(10);
    box->addLayout(fInfoBox);

    setLayout(box);
}

AccountProcessor AccountManager::data(const QString &id, QProgressBar *pb) const
{
    QString url = "http://acm.sgu.ru/teaminfo.php?id="+id;

    QNetworkReply *nr = fd->getURL(url, pb);

    while (!fd->isDone(nr))
        qApp->processEvents();

    if (!fd->isSuccessful(nr))
    {
        fd->popResult(nr);
        return AccountProcessor();
    }

    AccountProcessor ap(fd->popResult(nr));

    return ap;
}

StatusProcessor AccountManager::statData(const QString &id, QProgressBar *pb) const
{
    QString url = "http://acm.sgu.ru/status.php?id=" + id + "&idmode=1";

    QNetworkReply *nr = fd->getURL(url, pb);

    while (!fd->isDone(nr))
        qApp->processEvents();

    if (!fd->isSuccessful(nr))
    {
        fd->popResult(nr);
        return StatusProcessor();
    }

    return StatusProcessor(fd->popResult(nr), -1800);
}

void AccountManager::add(const QString &id)
{
    if (friendIDs.find(id) != friendIDs.end() || id == myID)
        return;

    AccountProcessor ap = data(id);

    if (ap.isValid())
    {
        friendStats[id] = statData(id);
        friendIDs.insert(id);
        lw->addItem(id);
        friendInfos[id] = ap;
        lw->setCurrentRow(lw->count()-1);
        lastUpdateStatSuccess = false;
        emit changed();
    }
}

void AccountManager::remove(const QString &id)
{
    friendIDs.remove(id);
    QList<QListWidgetItem *> ptrs = lw->findItems(id, Qt::MatchExactly);
    friendInfos.remove(id);
    friendStats.remove(id);
    friendSubmits.remove(id);
    delete ptrs.first();
    emit changed();
}

bool AccountManager::isFriend(const QString &id) const
{
    return friendIDs.find(id) != friendIDs.end();
}

void AccountManager::saveme()
{
    QString myi = le_id->text();
    QString myp = le_pass->text();

    if (friendIDs.find(myi) != friendIDs.end())
        return;

    AccountProcessor ap = data(myi);

    if (ap.isValid())
    {
        myID = myi;
        myPass = myp;
        me = ap;
        mestat = statData(myi);
        lastUpdateStatSuccess = false;
        updateMe();
        QMessageBox::information(this, tr("Information"), tr("Your account settings changed successfully."));
        emit changed();
    }
    else
        QMessageBox::warning(this, tr("Invalid ID"), tr("Entered ID is invalid!"));
}

void AccountManager::removebut()
{
    QListWidgetItem *cur = lw->currentItem();
    if (cur)
        remove(cur->text());
}

void AccountManager::addbut()
{
    QString ID = QInputDialog::getText(this, tr("Add friend"), tr("Enter User ID:"));

    if (friendIDs.find(ID) == friendIDs.end())
        add(ID);
}

void AccountManager::updateMe()
{
    me_name->setText(tr("<strong>Name:</strong> %1").arg(me.name()));
    me_fullname->setText(tr("<strong>Full name:</strong> %1").arg(me.fullName()));
    me_email->setText(tr("<strong>E-Mail:</strong> %1").arg(me.email()));
    me_birth->setText(tr("<strong>Birthday:</strong> %1").arg(me.birthday()));
    me_sub->setText(tr("<strong>Submits:</strong> %1").arg(me.numSubmits()));
    me_ac->setText(tr("<strong>Accepts:</strong> %1").arg(me.numAccepts()));
    double su = me.numSubmits();
    su /= me.numAccepts();
    QString ratio = QString::number(su);
    me_rat->setText(tr("(<strong>Ratio:</strong> %1)").arg(ratio));
}

void AccountManager::setInfo(QListWidgetItem *cur, QListWidgetItem *)
{
    if (cur)
    {
        AccountProcessor c = friendInfos[cur->text()];

        f_name->setText(tr("<strong>Name:</strong> %1").arg(c.name()));
        f_fullname->setText(tr("<strong>Full name:</strong> %1").arg(c.fullName()));
        f_email->setText(tr("<strong>E-Mail:</strong> %1").arg(c.email()));
        f_birth->setText(tr("<strong>Birthday:</strong> %1").arg(c.birthday()));
        f_sub->setText(tr("<strong>Submits:</strong> %1").arg(c.numSubmits()));
        f_ac->setText(tr("<strong>Accepts:</strong> %1").arg(c.numAccepts()));
        double su = c.numSubmits();
        su /= c.numAccepts();
        QString ratio = QString::number(su);
        f_rat->setText(tr("(<strong>Ratio:</strong> %1)").arg(ratio));
    }
    else
    {
        f_name->setText(tr("<strong>Name:</strong>"));
        f_fullname->setText(tr("<strong>Full name:</strong>"));
        f_email->setText(tr("<strong>E-Mail:</strong>"));
        f_birth->setText(tr("<strong>Birthday:</strong>"));
        f_sub->setText(tr("<strong>Submits:</strong>"));
        f_ac->setText(tr("<strong>Accepts:</strong>"));
        f_rat->setText(tr("(<strong>Ratio:</strong> )"));
    }
}

bool AccountManager::updateData(QProgressBar *pb, QProgressBar *ppb)
{
    bool res = true;

    if (pb)
    {
        pb->setMaximum(friendIDs.size()+1);
        pb->setValue(0);
    }

    if (myID != "Not Set")
    {
        AccountProcessor tmp = data(myID, ppb);
        if (tmp.isValid())
        {
            me = tmp;
            updateMe();
        }
        else
        {
            if (pb)
                pb->reset();

            if (ppb)
                ppb->reset();

            res = false;
        }
    }

    if (pb)
        pb->setValue(1);

    foreach (QString str, friendIDs)
    {
        AccountProcessor tmp = data(str, ppb);
        if (tmp.isValid())
            friendInfos[str] = tmp;
        else
        {
            if (pb)
                pb->reset();

            if (ppb)
                ppb->reset();

            res = false;
        }

        if (pb)
            pb->setValue(pb->value()+1);
    }

    setInfo(lw->currentItem(), nullptr);

    if (pb)
        pb->reset();

    if (ppb)
        ppb->reset();

    if (!res)
        return false;

    return true;
}

bool AccountManager::updateStatus(QProgressBar *pb, QProgressBar *ppb)
{
    bool res = true;

    if (pb)
    {
        pb->setMaximum(friendIDs.size()+1);
        pb->setValue(0);
    }

    if (myID != "Not Set")
    {
        StatusProcessor me = statData(myID, ppb);
        if (me.isValid())
            mestat = me;
        else
        {
            if (pb)
                pb->reset();

            if (ppb)
                ppb->reset();

            res = false;
        }
    }

    if (pb)
        pb->setValue(1);

    foreach (QString str, friendIDs)
    {
        StatusProcessor tmp = statData(str, ppb);
        if (tmp.isValid())
            friendStats[str] = tmp;
        else
        {
            if (pb)
                pb->reset();

            if (ppb)
                ppb->reset();

            res = false;
        }

        if (pb)
            pb->setValue(pb->value()+1);
    }

    if (pb)
        pb->reset();

    if (ppb)
        ppb->reset();

    if (!res)
        return false;

    lastUpdateStatSuccess = true;

    return true;
}

bool AccountManager::updateSubmits(QProgressBar *pb, QProgressBar *ppb)
{
    bool res = true;

    if (pb)
    {
        pb->setMaximum(3);
        pb->setValue(0);
    }

    if (!updateData(ppb))
    {
        if (pb)
            pb->reset();
        res = false;
    }

    if (pb)
        pb->setValue(1);

    if (!lastUpdateStatSuccess)
        if (!updateStatus(ppb))
        {
            if (pb)
                pb->reset();
            res = false;
        }

    if (pb)
        pb->setValue(2);

    foreach(QString str, friendIDs)
    {
        foreach(StatusInfo si, friendStats[str].data())
            friendSubmits[str].insert(si.problem());
        friendSubmits[str] -= friendInfos[str].acceptedProblems();
        friendSubmits[str] -= me.acceptedProblems();
    }

    if (pb)
        pb->setValue(3);

    return res;
}

QDataStream & operator<<(QDataStream &ds, const AccountManager &in)
{
    ds << in.myID << in.myPass;
    ds << in.me << in.mestat;
    ds << in.friendIDs;
    ds << in.friendInfos << in.friendStats << in.friendSubmits;
    ds << in.lastUpdateStatSuccess;

    return ds;
}

QDataStream & operator>>(QDataStream &ds, AccountManager&in)
{
    ds >> in.myID >> in.myPass;
    ds >> in.me >> in.mestat;
    ds >> in.friendIDs;
    ds >> in.friendInfos >> in.friendStats >> in.friendSubmits;
    ds >> in.lastUpdateStatSuccess;

    if (in.myID != "Not Set")
    {
        in.le_id->setText(in.myID);
        in.le_pass->setText(in.myPass);
        if (in.me.isValid())
            in.updateMe();
    }

    foreach (QString str, in.friendIDs)
        in.lw->addItem(str);

    in.lw->setCurrentRow(in.lw->count()-1);

    in.emitChanged();

    return ds;
}

void AccountManager::profMe()
{
    if (myID != "Not Set")
    {
        ProfileInfo pi(this, this, myID);

        if (pi.isValid())
            pi.exec();
    }
}

void AccountManager::profFir()
{
    if (lw->currentItem())
    {
        ProfileInfo pi(this, this, lw->currentItem()->text());

        if (pi.isValid())
            pi.exec();
    }
}

void AccountManager::profOther()
{
    ProfileInfo pi(this, this, viewProfileEdit->text());

    if (pi.isValid())
        pi.exec();
}

void AccountManager::profDC(QListWidgetItem *in)
{
    ProfileInfo pi(this, this, in->text());

    if (pi.isValid())
        pi.exec();
}
