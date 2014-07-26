#include "competitionview.h"
#include "competition.h"
#include "accountmanager.h"
#include "profileinfo.h"
#include "competitiondetail.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QString>
#include <QSize>
#include <QProcess>
#include <QtGui>

CompetitionView::CompetitionView(QWidget *parent, AccountManager *am,
								 Competition *com) :
    QTableWidget(parent)
{
    this->am = am;
	this->com = com;
    setColumnCount(3);
    setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Target")));
    setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Compared to")));
    setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Competition type")));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handleDoubleClick(int,int)));
    resizeColumnsToContents();
}

void CompetitionView::setData(const QList<CompetitionInfo> &in)
{
    setRowCount(0);
	setRowCount(in.size());

    foreach (CompetitionInfo ci, in)
    {
		if (!ids.contains(ci) && com->hasNoti() )
        {
            QString name = am->infoOf(ci.target()).name();

            QProcess::execute("notify-send", QStringList() << "--icon=info" << "--app-name='SGU Client'" << "SGU Client" << "You have some warnings about "+name);
        }
    }

    ids.clear();

    for (int i = 0; i < in.size(); i++)
        setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i+1)));

    bool hascrit = false;
    for (int i = 0; i < in.size(); i++)
    {
        ids.append(in[i]);

        bool critical = false;
        if (in[i].type() == DiffAc || in[i].type() == MoreAc)
        {
            hascrit = true;
            critical = true;
        }

        QString nam = am->infoOf(in[i].target()).name();
        QTableWidgetItem *name = new QTableWidgetItem;
        name->setText(nam);
        name->setFlags(Qt::ItemIsEnabled);
        if (critical)
        {
            name->setBackground(Qt::red);
            name->setForeground(Qt::yellow);
        }
        else
        {
            name->setBackground(Qt::yellow);
            name->setForeground(Qt::red);
        }
        setItem(i, 0, name);

        QTableWidgetItem *compto = new QTableWidgetItem;
        compto->setText(am->ownInfo().name());
        compto->setFlags(Qt::ItemIsEnabled);
        compto->setForeground(Qt::blue);
        if (critical)
            compto->setBackground(Qt::red);
        else
            compto->setBackground(Qt::yellow);
        setItem(i, 1, compto);

        QTableWidgetItem *compt = new QTableWidgetItem;
        compt->setText(reason(in[i].type()));
        compt->setFlags(Qt::ItemIsEnabled);
        if (critical)
            compt->setBackground(Qt::red);
        else
            compt->setBackground(Qt::yellow);
        setItem(i, 2, compt);
    }

	if (hascrit && com->hasNoti() )
        QProcess::execute("notify-send", QStringList() << "--icon=error" << "--app-name='SGU Client'" << "SGU Client" << "You have some critical warnings!");

    resizeColumnsToContents();
}

QSize CompetitionView::sizeHint() const
{
    return QSize(750, 500);
}

QString CompetitionView::reason(CompetitionType T) const
{
    if (T == MoreAc)
        return "Target has more accepts than you.";
    else if (T == NearAc)
        return "Target is reaching you.";
    else if (T == DiffAc)
        return "Target has some accepts that you don't have.";
    else
        return "Target tried to solve problems that you haven't accepted.";
}

void CompetitionView::handleDoubleClick(int row, int col)
{
    if (col == 2)
    {
        CompetitionDetail cd(am, ids[row], reason(ids[row].type()), this);

        if (cd.isValid())
            cd.exec();
    }
    else if (col == 1)
    {
        ProfileInfo pi(this, am, am->ID());

        if (pi.isValid())
            pi.exec();
    }
    else if (col == 0)
    {
        QString pid = ids[row].target();
        ProfileInfo pi(this, am, pid);

        if (pi.isValid())
            pi.exec();
    }
}
