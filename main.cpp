#include <QApplication>
#include "sguclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SGUClient sguc;
    sguc.show();

    return app.exec();
}
