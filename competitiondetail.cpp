#include "competitiondetail.h"
#include "accountmanager.h"
#include <QtGui>

CompetitionDetail::CompetitionDetail(AccountManager *am, const CompetitionInfo &inf, const QString &summary,
                                     QWidget *parent) :
    QDialog(parent)
{
    AccountProcessor ap = am->infoOf(inf.target());
    isVal = ap.isValid();

    if (isVal)
    {
        info = new QLabel(tr("<h2>Details about competition</h2>"));

        target = new QLabel(tr("<strong>Target:</strong> %1").arg("<font color=darkGreen>" + ap.name() + "</font>"));
        compto = new QLabel(tr("<strong>Compared</strong> To: %1").arg("<font color=blue>" + am->ownInfo().name() + "</font>"));

        hb = new QHBoxLayout;
        hb->addWidget(target);
        hb->addStretch();
        hb->addWidget(compto);

        summ = new QLabel(tr("<strong>Summary:</strong> %1").arg(summary));

        if (inf.type() == MoreAc || inf.type() == NearAc)
        {
            fir = new QLabel(tr("<strong>Your accepts:</strong> %1").arg(QString::number(am->ownInfo().numAccepts())));
            sec = new QLabel(tr("<strong>Target's accepts:</strong> %1").arg(QString::number(ap.numAccepts())));
        }
        else
        {
            fir = new QLabel(tr("<strong>Those problems are:</strong>"));

            if (inf.type() == DiffAc)
                sec = new QLabel("<font color=red>" + toStr(ap.acceptedProblems()-am->ownInfo().acceptedProblems()) + "</font>");
            else
                sec = new QLabel("<font color=red>" + toStr(am->submitsOf(inf.target())-am->ownInfo().acceptedProblems()) + "</font>");

            sec->setWordWrap(true);
        }

        lay = new QVBoxLayout;
        lay->addWidget(info);
        lay->addLayout(hb);
        lay->addWidget(summ);
        lay->addWidget(fir);
        lay->addWidget(sec);

        setLayout(lay);
        setWindowTitle(tr("Competition Details"));
    }
}

QString CompetitionDetail::toStr(const QSet<int> &in) const
{
    QString res;

    QList<int> kop = QList<int>::fromSet(in);
    qSort(kop);

    foreach (int i, kop)
        res += QString::number(i)+" ";

    return res;
}
