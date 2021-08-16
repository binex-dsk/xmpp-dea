#include "client.h"

#include <QXmppLogger.h>
#include <QXmppMessage.h>
#include <QXmppMucManager.h>

#include <QCoreApplication>
#include <QDebug>

DEAClient::DEAClient(QObject *parent)
    : QXmppClient(parent)
{
    QXmppMucManager *mucManager = new QXmppMucManager();
    this->addExtension(mucManager);
    connect(this, &QXmppClient::messageReceived, this, &DEAClient::messageReceived);
}

DEAClient::~DEAClient()
{
}

void DEAClient::messageReceived(const QXmppMessage &message)
{
    QString from = message.from();
    QString msg = message.body();

    qInfo() << message.type();
    if (!message.stamp().isNull()) {
        // if the message contains a timestamp, this means it has been "delayed"
        // as per XEP-0203, "delayed" messages are those that have been stored on
        // the server for any reason. Usually, this means any message sent while the
        // bot was offline, OR any message sent before the bot joined the room.
        // We don't want to process these.
        return;
    }
    if (message.type() == QXmppMessage::Normal) {
        QXmppMucManager *mucManager = this->findExtension<QXmppMucManager>();
        if (mucManager) {
            const QString roomJid = message.mucInvitationJid();
            // process room invitations
            if (!roomJid.isEmpty()) {
                QXmppMucRoom *room = mucManager->addRoom(roomJid);
                if (!mucManager->rooms().contains(room) || !room->isJoined()) {
                    room->setNickName("DEA");
                    room->join();
                }
            }
        }
    } else if (message.type() == QXmppMessage::GroupChat) {
        QXmppMucManager *mucManager = this->findExtension<QXmppMucManager>();
        if (mucManager) {
            const QString roomJid = message.stanzaIdBy();
            QXmppMucRoom *room = mucManager->addRoom(roomJid);
            const QString myJid = room->jid() + "/" + room->nickName();
            if (from != myJid) { // message was not sent by the bot
                room->sendMessage("zz hi you said: " + msg);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QFile f("account.txt");
    QString user, password;
    if (f.open(QFile::ReadOnly)) {
        QTextStream pw(&f);
        user = pw.readLine();
        password = pw.readLine();
    } else {
        qCritical() << "account.txt not found or can not be read";
        qInfo() << "account.txt should be in this format:\n<bot JID>\n<bot password>";
        return 1;
    }
    f.close();
    DEAClient client;
    client.logger()->setLoggingType(QXmppLogger::StdoutLogging);
    client.connectToServer(user, password);

    return app.exec();
}
