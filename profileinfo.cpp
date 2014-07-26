#include "profileinfo.h"
#include "accountmanager.h"
#include "profilestatus.h"
#include <QtGui>

ProfileInfo::ProfileInfo(QWidget *parent, AccountManager *am, const QString &id) :
    QDialog(parent)
{
    AccountProcessor ap;
    if (id == am->ID())
        ap = am->ownInfo();
    else if (am->isFriend(id))
        ap = am->infoOf(id);
    else
        ap = am->data(id);

    isVal = ap.isValid();

    if (isVal)
    {
        SID = id;
        this->am = am;

        ID = new QLabel(tr("<h2>Information for ID %1:</h2>").arg(id));

        name = new QLabel(tr("<strong>Name:</strong> %1").arg(ap.name()));
        fname = new QLabel(tr("<strong>Fullname:</strong> %1").arg(ap.fullName()));
        f1 = new QHBoxLayout;
        f1->addWidget(name);
        f1->addStretch();
        f1->addWidget(fname);

        email = new QLabel(tr("<strong>E-Mail:</strong> %1").arg(ap.email()));
        birth = new QLabel(tr("<strong>Birthday:</strong> %1").arg(ap.birthday()));
        country = new QLabel(tr("<strong>Country:</strong> %1").arg(ap.country()));
        f2 = new QHBoxLayout;
        f2->addWidget(email);
        f2->addStretch();
        f2->addWidget(birth);
        f2->addStretch();
        f2->addWidget(country);

        study = new QLabel(tr("<strong>Study place:</strong> %1").arg(ap.studyPlace()));
        study->setWordWrap(true);

        info = new QLabel(tr("<strong>Informations:</strong> %1").arg(ap.information()));
        info->setWordWrap(true);

        sub = new QLabel(tr("<strong>Submits:</strong> %1").arg(QString::number(ap.numSubmits())));
        ac = new QLabel(tr("<strong>Accepts:</strong> %1").arg(QString::number(ap.numAccepts())));
        double su = ap.numSubmits();
        su /= ap.numAccepts();
        rat = new QLabel(tr("(<strong>Ratio:</strong> %1)").arg(QString::number(su)));
        f3 = new QHBoxLayout;
        f3->addWidget(sub);
        f3->addStretch();
        f3->addWidget(ac);
        f3->addStretch();
        f3->addWidget(rat);

        ac1 = new QLabel(tr("<strong>Old accepts:</strong> %1").arg(toStr(ap.acceptedProblems()-ap.acceptedLastWeek()-ap.acceptedLastMonth()-ap.acceptedLastDay())));
        ac2 = new QLabel(tr("<strong>Last month:</strong> %1").arg(toStr(ap.acceptedLastMonth())));
        ac3 = new QLabel(tr("<strong>Last week:</strong> %1").arg(toStr(ap.acceptedLastWeek())));
        ac4 = new QLabel(tr("<strong>Last day:</strong> %1").arg(toStr(ap.acceptedLastDay())));

        ac1->setWordWrap(true);
        ac2->setWordWrap(true);
        ac3->setWordWrap(true);
        ac4->setWordWrap(true);

        substat = new QPushButton(tr("Submit details"));
        sublay = new QHBoxLayout;
        sublay->addStretch();
        sublay->addWidget(substat);
        sublay->addStretch();
        connect(substat, SIGNAL(clicked()), this, SLOT(submitStat()));

        lay = new QVBoxLayout;
        lay->addWidget(ID);
        lay->addLayout(f1);
        lay->addLayout(f2);
        lay->addWidget(study);
        lay->addWidget(info);
        lay->addLayout(f3);
        lay->addWidget(ac1);
        lay->addWidget(ac2);
        lay->addWidget(ac3);
        lay->addWidget(ac4);
        lay->addLayout(sublay);

        setLayout(lay);
        setWindowTitle(tr("Account Information"));
    }
}

QString ProfileInfo::toStr(const QSet<int> &in) const
{
    QString res;

    QList<int> kop = QList<int>::fromSet(in);
    qSort(kop);


    QSet<int> tmp;
    if (am->ID() != "Not Set")
        tmp = am->ownInfo().acceptedProblems();

    foreach (int i, kop)
    {
        bool pnew = false;
        if (am->ID() != "Not Set" && tmp.find(i) == tmp.end())
            pnew = true;

        if (pnew)
            res += "<font color=red>";
        res += QString::number(i);
        if (pnew)
            res += "</font>";

        res += " ";
    }

    return res;
}

void ProfileInfo::submitStat()
{
    ProfileStatus ps(this, am, SID);

    if (ps.isValid())
        ps.exec();
}
