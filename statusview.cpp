#include "statusview.h"
#include "accountmanager.h"
#include "profileinfo.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QString>
#include <QSize>

StatusView::StatusView(QWidget *parent, AccountManager *am) :
    QTableWidget(parent)
{
    this->am = am;
    setColumnCount(8);
    setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Date-Time")));
    setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Name")));
    setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Prob")));
    setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Lang")));
    setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Result")));
    setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Time")));
    setHorizontalHeaderItem(7, new QTableWidgetItem(tr("Memory")));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handleDoubleClick(int,int)));
    resizeColumnsToContents();
}

void StatusView::setData(const QList<StatusInfo> &in)
{
    setRowCount(0);
    setRowCount(in.size());
    ids.clear();

    for (int i = 0; i < in.size(); i++)
        setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i+1)));

    for (int i = 0; i < in.size(); i++)
    {
        ids.append(in[i].ID());

        QTableWidgetItem *id = new QTableWidgetItem;
        id->setText(in[i].submitID());
        id->setFlags(Qt::ItemIsEnabled);
        setItem(i, 0, id);

        QTableWidgetItem *date = new QTableWidgetItem;
        date->setText(in[i].dateTime().toString("yyyy/MM/dd hh:mm"));
        date->setFlags(Qt::ItemIsEnabled);
        setItem(i, 1, date);

        QTableWidgetItem *nam = new QTableWidgetItem;
        nam->setText(in[i].name());
        nam->setFlags(Qt::ItemIsEnabled);
        if (in[i].ID() == am->ID())
            nam->setForeground(Qt::darkBlue);
        else if (am->isFriend(in[i].ID()))
            nam->setForeground(Qt::darkGreen);
        setItem(i, 2, nam);

        QTableWidgetItem *prob = new QTableWidgetItem;
        prob->setText(QString::number(in[i].problem()));
        prob->setFlags(Qt::ItemIsEnabled);
        if (am->ID() != "Not Set")
        {
            QSet<int> tmp = am->ownInfo().acceptedProblems();
            if (tmp.find(in[i].problem()) == tmp.end())
                prob->setForeground(Qt::red);
        }
        setItem(i, 3, prob);

        QTableWidgetItem *lang = new QTableWidgetItem;
        lang->setText(in[i].language());
        lang->setFlags(Qt::ItemIsEnabled);
        setItem(i, 4, lang);

        QTableWidgetItem *res = new QTableWidgetItem;
        res->setText(in[i].longResult());
        res->setFlags(Qt::ItemIsEnabled);
        if (in[i].resultType() == AC)
            res->setForeground(Qt::red);
        setItem(i, 5, res);

        QTableWidgetItem *tim = new QTableWidgetItem;
        tim->setFlags(Qt::ItemIsEnabled);
        tim->setText(QString::number(in[i].time()) + " ms");
        setItem(i, 6, tim);

        QTableWidgetItem *mem = new QTableWidgetItem;
        mem->setFlags(Qt::ItemIsEnabled);
        mem->setText(QString::number(in[i].memory()) + " kb");
        setItem(i, 7, mem);
    }

    resizeColumnsToContents();
}

QSize StatusView::sizeHint() const
{
    return QSize(750, 500);
}

void StatusView::handleDoubleClick(int row, int col)
{
    if (col == 2)
    {
        QString pid = ids[row];
        ProfileInfo pi(this, am, pid);

        if (pi.isValid())
            pi.exec();
    }
}
