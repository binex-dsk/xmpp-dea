#ifndef DEACLIENT_H
#define DEACLIENT_H

#include <QXmppClient.h>

class DEAClient : public QXmppClient
{
    Q_OBJECT

public:
    DEAClient(QObject *parent = nullptr);
    ~DEAClient() override;

public slots:
    void messageReceived(const QXmppMessage &);
};

#endif  // DEACLIENT_H

