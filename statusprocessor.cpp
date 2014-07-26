#include "statusprocessor.h"
#include <QString>
#include <QDateTime>
#include <QDataStream>

StatusInfo::StatusInfo(const QString &a, const QString &b, const QString &c, const QString &d, const QDateTime &e,
           ResultType r, int t1, int t2, int m, int p)
{
    Subid = a;
    Id = b;
    Name = c;
    Datetime = e;
    Lang = d;
    RT = r;
    Testnumber = t1;
    Time = t2;
    Memory = m;
    Problem = p;
}

QString StatusInfo::shortResult() const
{
    QString res;

    if (RT == AC)
        res = "AC";
    else if (RT == WA)
        res = "WA";
    else if (RT == RE)
        res = "RE";
    else if (RT == GE)
        res = "GE";
    else if (RT == PE)
        res = "PE";
    else if (RT == TL)
        res = "TL";
    else if (RT == ML)
        res = "ML";
    else if (RT == CE)
        res = "CE";
    else if (RT == WAIT)
        res = "WAIT";
    else
        res = "RUN";

    if (RT != AC && RT != GE && RT != CE && RT != WAIT)
        res += " "+QString::number(Testnumber);

    return res;
}

QString StatusInfo::longResult() const
{
    QString res;

    if (RT == AC)
        res = QObject::tr("Accepted");
    else if (RT == WA)
        res = QObject::tr("Wrong answer on test");
    else if (RT == RE)
        res = QObject::tr("Runtime error on test");
    else if (RT == GE)
        res = QObject::tr("Global error");
    else if (RT == PE)
        res = QObject::tr("Presentation error on test");
    else if (RT == TL)
        res = QObject::tr("Time limit exceeded on test");
    else if (RT == ML)
        res = QObject::tr("Memory limit exceeded on test");
    else if (RT == CE)
        res = QObject::tr("Compilation Error");
    else if (RT == WAIT)
        res = QObject::tr("Waiting");
    else
        res = QObject::tr("Running:");

    if (RT != AC && RT != GE && RT != CE && RT != WAIT)
        res += " "+QString::number(Testnumber);

    return res;
}

QDataStream & operator<<(QDataStream &ds, const StatusInfo &in)
{
    ds << in.Subid << in.Id << in.Name << in.Lang << in.Datetime;
    ds << static_cast<int>(in.RT);
    ds << in.Testnumber << in.Time << in.Memory << in.Problem;

    return ds;
}

QDataStream & operator>>(QDataStream &ds, StatusInfo &in)
{
    ds >> in.Subid >> in.Id >> in.Name >> in.Lang >> in.Datetime;
    int tmp;
    ds >> tmp;
    in.RT = static_cast<ResultType>(tmp);
    ds >> in.Testnumber >> in.Time >> in.Memory >> in.Problem;

    return ds;
}

StatusProcessor::StatusProcessor(const QString &src, int secs)
{
    err = false;
    diffsec = secs;

    int startPos = src.indexOf("    <TD width=\"7%\">Memory: </TD>")+QString("    <TD width=\"7%\">Memory: </TD>").size();
    int endPos = src.indexOf("</TR></TABLE><table align=right width=20%><tr><td>");

    if (startPos == -1 || endPos == -1)
        err = true;
    else
    {
        int next = src.indexOf("</TR><TR class=", startPos);
        while (next < endPos && next != -1)
        {
            QString subi = src.mid(next+23, 7);

            next = src.indexOf("<TD style=\"FONT-SIZE: 7pt;\">", next)+QString("<TD style=\"FONT-SIZE: 7pt;\">").size();
            QDateTime datet = processDate(src.mid(next, 14));

            next = src.indexOf("<a href=teaminfo.php?id=", next)+QString("<a href=teaminfo.php?id=").size();
            QString iD = src.mid(next, 6);

            next = src.indexOf(";\">", next)+QString(";\">").size();
            int until = src.indexOf("</a></TD><TD>", next);
            if (src.mid(next, 2) == "<f")
            {
                next = src.indexOf(">", next)+1;
                until = src.indexOf("</font></a></TD><TD>", next);
            }
            QString nam = src.mid(next, until-next);

            next = src.indexOf("</a></TD><TD>", next)+QString("</a></TD><TD>").size();
            until = src.indexOf("</TD><TD>", next);
            int prob = src.mid(next, until-next).trimmed().toInt();

            next = src.indexOf("</TD><TD>", next)+QString("</TD><TD>").size();
            until = src.indexOf("</TD><TD class=btab>", next);
            QString lng = src.mid(next+1, until-next-1);

            next = src.indexOf("</TD><TD class=btab>", next)+QString("</TD><TD class=btab>").size();
            until = src.indexOf("</TD>", next);
            QString resu;

            if (src.mid(next, 2) == "<f")
                resu = "Accepted";
            else if (src.mid(next, 2) == "<a")
                resu = "Compilation Error";
            else
                resu = src.mid(next, until-next);
            ResultType RTT = processType(resu);
            int TestN = processNumb(resu);

            next = src.indexOf("<TD>", next)+QString("<TD>").size();
            until = src.indexOf("</TD>", next)-3;
            int Tim = src.mid(next, until-next).toInt();

            next = src.indexOf("<TD>", next)+QString("<TD>").size();
            until = src.indexOf("</TD>", next)-3;
            int Mem = src.mid(next, until-next).toInt();

            StatusInfo inf(subi, iD, nam, lng, datet, RTT, TestN, Tim, Mem, prob);
            info.append(inf);

            next = src.indexOf("</TR><TR class=", next);
        }
    }
}

ResultType StatusProcessor::processType(const QString &in) const
{
    QString str = in.mid(0, 4);

    if (str == "Acce")
        return AC;
    else if (str == "Wron")
        return WA;
    else if (str == "Runt")
        return RE;
    else if (str == "Glob")
        return GE;
    else if (str == "Pres")
        return PE;
    else if (str == "Time")
        return TL;
    else if (str == "Memo")
        return ML;
    else if (str == "Comp")
        return CE;
    else if (str == "Wait")
        return WAIT;
    else
        return RUN;
}

int StatusProcessor::processNumb(const QString &in) const
{
    ResultType T = processType(in);
    if (T == AC || T == GE || T == CE || T == WAIT)
        return 0;

    int en = in.size();
    int st = in.size()-1;
    while (QChar('0') <= in[st] && in[st] <= QChar('9'))
        st--;
    st++;

    return in.mid(st, en-st).toInt();
}

QDateTime StatusProcessor::processDate(const QString &in) const
{
    int hour = in.mid(9, 2).toInt();
    int minute = in.mid(12, 2).toInt();

    int day = in.mid(0, 2).toInt();
    int month = in.mid(3, 2).toInt();
    int year = in.mid(6, 2).toInt()+2000;

    QTime tim(hour, minute);
    QDate dat(year, month, day);

    QDateTime datim(dat, tim);
    datim = datim.addSecs(diffsec);

    return datim;
}

QDataStream & operator<<(QDataStream &ds, const StatusProcessor &in)
{
    ds << in.info << in.err;

    return ds;
}

QDataStream & operator>>(QDataStream &ds, StatusProcessor &in)
{
    ds >> in.info >> in.err;
    return ds;
}
