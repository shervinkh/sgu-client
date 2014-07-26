#include "profilestatus.h"
#include "accountmanager.h"
#include "statusprocessor.h"
#include "statusview.h"
#include <QtGui>

ProfileStatus::ProfileStatus(QWidget *parent, AccountManager *am, const QString &id) :
    QDialog(parent)
{
    StatusProcessor sp;
    if (id == am->ID())
        sp = am->ownStatus();
    else if (am->isFriend(id) && am->statOf(id).isValid())
        sp = am->statOf(id);
    else
        sp = am->statData(id);

    isVal = sp.isValid();

    if (isVal)
    {
        ID = new QLabel(tr("<h2>Submit status for ID %1:</h2>").arg(id));

        sv = new StatusView(this, am);
        sv->setData(sp.data());

        lay = new QVBoxLayout;
        lay->addWidget(ID);
        lay->addWidget(sv);

        setLayout(lay);
        setWindowTitle(tr("Submit Status"));
    }
}
