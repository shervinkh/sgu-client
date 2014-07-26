#ifndef SGUCLIENT_H
#define SGUCLIENT_H

#include <QMainWindow>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;
class AccountManager;
class FileDownloader;
class FriendActivity;
class OnlineStatus;
class Competition;
class Suggestion;

class SGUClient : public QMainWindow
{
    Q_OBJECT
private:
    QPushButton *friendAct;
    QPushButton *competit;
	QPushButton *sugg;
    QPushButton *stat;
    QPushButton *accs;
    QVBoxLayout *butts;

    QStackedWidget *sw;
    QHBoxLayout *layou;

    QWidget *wid;

    AccountManager *am;
    FileDownloader *fd;
    FriendActivity *fa;
    OnlineStatus *so;
    Competition *com;
	Suggestion *sug;

public:
    explicit SGUClient(QWidget *parent = nullptr);
    ~SGUClient();

private slots:
    void accPage();
    void faPage();
    void statPage();
    void comPage();
	void sugPage();
    void loadCache();
};

#endif // SGUCLIENT_H
