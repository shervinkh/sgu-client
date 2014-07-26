#include "accountprocessor.h"
#include <QString>

QDataStream & operator<<(QDataStream &ds, const AccountProcessor &in)
{
    ds << in.a_name << in.a_email << in.a_fullname << in.a_country;
    ds << in.a_studyplace << in.a_birthday << in.a_info;
    ds << in.num_submits << in.num_accepts;
    ds << in.problems << in.lastDay << in.lastWeek << in.lastMonth;
    ds << in.err;

    return ds;
}

QDataStream & operator>>(QDataStream &ds, AccountProcessor &in)
{
    ds >> in.a_name >> in.a_email >> in.a_fullname >> in.a_country;
    ds >> in.a_studyplace >> in.a_birthday >> in.a_info;
    ds >> in.num_submits >> in.num_accepts;
    ds >> in.problems >> in.lastDay >> in.lastWeek >> in.lastMonth;
    ds >> in.err;

    return ds;
}

AccountProcessor::AccountProcessor(const QString &src)
{
    err = false;

    int st = src.indexOf("<td>Name</td><td>:&#160;")+QString("<td>Name</td><td>:&#160;").size();
    int en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>E-Mail");
    if (st == -1 || en == -1)
        err = true;
    else
    {
        if (src.mid(st, 2) == "<f")
        {
            st = src.indexOf(">", st)+1;
            en = src.indexOf("</font>", st);
        }
        a_name = src.mid(st, en-st);
    }

    if (!err)
    {
        st = src.indexOf("E-Mail</td><td>:&#160;")+QString("E-Mail</td><td>:&#160;").size();
        en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>Country");
        a_email = src.mid(st, en-st);
        a_email.replace("[at]", "@");
        a_email.replace("[dot]", ".");

        st = src.indexOf("Country</td><td>:&#160;")+QString("Country</td><td>:&#160;").size();
        en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>Full");
        a_country = src.mid(st, en-st);

        st = src.indexOf("Full Name</td><td>:&#160;")+QString("Full Name</td><td>:&#160;").size();
        en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>Study");
        a_fullname = src.mid(st, en-st);

        st = src.indexOf("Study Place</td><td>:&#160;")+QString("Study Place</td><td>:&#160;").size();
        en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>Birth");
        a_studyplace = src.mid(st, en-st);

        st = src.indexOf("Birthday</td><td>:&#160;")+QString("Birthday</td><td>:&#160;").size();
        en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>Infor");
        a_birthday = src.mid(st, en-st);

        st = src.indexOf("Information</td><td>:&#160;")+QString("Information</td><td>:&#160;").size();
        en = src.indexOf("</td></tr><tr><td colspan=2><HR></td></tr><tr><td>Stat");
        a_info = src.mid(st, en-st);

        st = src.indexOf("Submitted: ")+QString("Submitted: ").size();
        en = src.indexOf("; Accepted:");
        num_submits = src.mid(st, en-st).toInt();

        st = src.indexOf("Accepted: ")+QString("Accepted: ").size();
        en = src.indexOf("<br>&nbsp;&nbsp;[<a href='status.php");
        num_accepts = src.mid(st, en-st).toInt();

        st = src.indexOf("Accepted</td><td>:&#160")+QString("Accepted</td><td>:&#160").size();
        en = src.indexOf("</td></tr><tr><td></td><td>");

        int next = src.indexOf("&#160", st);
        while (next < en && next != -1)
        {
            int cur = src.mid(next-3, 3).toInt();
            problems.insert(cur);

            QString color = src.mid(next-10, 6);
            if (color == "CC0000")
                lastDay.insert(cur);
            else if (color == "008800")
                lastWeek.insert(cur);
            else if (color == "0000AA")
                lastMonth.insert(cur);

            next = src.indexOf("&#160", next+1);
        }
    }
}
